#include <iostream>
#include <thread>
#include <atomic>
#include <memory>

template<typename T>
class lock_free_stack_ref_counting
{
private:
    struct node;

    struct node_wrapper{
        int external_count;
        node* ptr;
    };

    struct node
    {
        std::shared_ptr<T> data;
        std::atomic<int> internal_count;
        node_wrapper next;

        node(const T& _data) : data(std::make_shared<T>(_data))
            ,internal_count(0) {}
    };

    void increment_head_ref_count(node_wrapper& old_counter) {
        node_wrapper new_counter;

        do
        {
            new_counter = old_counter;
            ++new_counter.external_count;
        } while (!head.compare_exchange_strong(old_counter, new_counter));

        old_counter.external_count = new_counter.external_count;
        
    }

    std::atomic<node_wrapper> head;

public:

    lock_free_stack_ref_counting() {}

    void push(const T& data) {
        node_wrapper new_node;
        new_node.ptr = new node(data);
        new_node.external_count = 1;
        new_node.ptr->next = head.load();
        while(!head.compare_exchange_weak(new_node.ptr->next, new_node));
    }

    std::shared_ptr<T> pop() { 
        node_wrapper old_head = head.load();

        for(;;) {
            increment_head_ref_count(old_head);
            const node* ptr = old_head.ptr;

            if (!ptr) {
                return std::shared_ptr<T>();
            } 

            if (head.compare_exchange_strong(old_head, ptr->next)) {
                std::shared_ptr<T> res;
                res.swap(ptr->data);

                const int current_external_count = old_head.external_count - 2;

                if (ptr->internal_count.fetch_add(current_external_count) == 0) {
                    delete ptr;
                }

                return res;
            } else if (ptr->internal_count.fetch_sub(1) == 1) {
                delete ptr;
            }
        }

    }
};