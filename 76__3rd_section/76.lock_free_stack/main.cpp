#include <iostream>
#include <thread>
#include <atomic>
#include <memory>

template<typename T>
class lock_free_stack
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        node* next;
        node(const T& _data) : data(std::make_shared<T>(_data)) {}
    };

    std::atomic<node*> head; 
    // make atomic in order accelerate threads and do not create a risk of race

    std::atomic<int> threads_in_pop;
    std::atomic<node*> to_be_deleted; // list

    void try_reclaim(node* old_head) {
        if (threads_in_pop == 1) {
            //delete node pointed by old_head
            delete old_head;

            node* claimed_list = to_be_deleted.exchange(nullptr);
            if (!--threads_in_pop) {
                delete_nodes(claimed_list);
            } else if (claimed_list) {
                node* last = claimed_list;
                while (node* const next = last->next) {
                    last = next;
                }
                last->next = to_be_deleted;
                while(!(to_be_deleted.compare_exchange_weak(last->next
                                    , claimed_list)));
            }

        } else {
            //add node pointed by old_head to the to_be_deleted list
            old_head->next = to_be_deleted;
            while(!to_be_deleted.compare_exchange_weak(old_head->next, old_head));
            --threads_in_pop;
        }
    }

    void delete_nodes(node* nodes) {
        while (nodes)
        {
            node* next = nodes->next;
            delete nodes;
            nodes = next;
        }
        
    }    

public:
    void push(const T& _data) {
        node* const new_node = new node(_data);
        new_node->next = head.load(); // atomic operation
        while (!head.compare_exchange_weak(new_node->next, new_node));
    }

    /*void pop(T& result) { // result is a value of deleted node 
        node* old_head = head.load();
        while( old_head && !(head.compare_exchange_weak(old_head, old_head->next)));
        result = old_head->data;
        //delete old_head;
        //with race of threads delete will erase the memory and another thread will try to achieve its value - undefined behaviour!
    }*/

    std::shared_ptr<T> pop() { // result is a value of deleted node 
        ++threads_in_pop;
        node* old_head = head.load();
        while( old_head && !(head.compare_exchange_weak(old_head, old_head->next)));
        
        std::shared_ptr<T> res;
        if (old_head) {
            res.swap(old_head->data);
        }
        try_reclaim(old_head);
        return res;
    }
};