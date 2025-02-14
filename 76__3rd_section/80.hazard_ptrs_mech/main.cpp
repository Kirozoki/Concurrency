#include <iostream>
#include <thread>
#include <atomic>
#include <memory>

template<typename T>
class lock_free_stack_hazard
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        node* next;
        node(const T& _data) : data(std::make_shared<T>(_data)) {}
    };

    struct hazard_pointer
    {
        std::atomic<std::thread::id> id;
        std::atomic<void*> pointer; 
    };

    std::atomic<node*> head; 
    // make atomic in order accelerate threads and do not create a risk of race

    static const int max_hazard_ptrs = 100;
     //list
    hazard_pointer hazard_pointers[max_hazard_ptrs];
    std::atomic<node*> nodes_to_reclaim; // list

    class hp_manager
    {
    private:
        hazard_pointer* hp;
    public:
        hp_manager() : hp(nullptr) {
            for (unsigned i = 0; i < max_hazard_ptrs; ++i) {
                std::thread::id old_id;
                if (hazard_pointers[i].id.compare_exchange_strong(old_id, std::this_thread::get_id()))
                {
                    hp = &hazard_pointers[i];
                    break;
                }
            }
            if (!hp) {
                throw std::runtime_error("no hazard pointers available");
            }
        }

        ~hp_manager(){
            hp->pointer.store(nullptr);
            hp->id.store(std::thread::id());
        }

        std::atomic<void*> &get_pointer() {
            return hp->pointer;
        }
    };    

    std::atomic<void*>& get_hazard_pointer_for_current_thread() {
        static thread_local hp_manager hz_owner; // in order to avoid of creating hpm for each thread
        return hz_owner.get_pointer();
    }

    bool any_outstanding_hazards(node* p) {
        for (unsigned i = 0; i < max_hazard_ptrs; ++i) {
            if (hazard_pointers[i].pointer.load() == p) {
                return true;
            }
        }
        return false;
    }

    void reclaim_later(node* node) {
        node->next = nodes_to_reclaim.load();
        while(!nodes_to_reclaim.compare_exchange_weak(node->next, node));
    }

    void delete_nodes_with_no_hazards() {
        node* current = nodes_to_reclaim.exchange(nullptr);

        while (current) {
            const node* next = current->next;
            if (!any_outstanding_hazards(current)) {
                delete current;
            } else {
                reclaim_later(current);
            }
            current = next;
        }
    }

public:

    lock_free_stack_hazard() {}

    void push(const T& _data) {
        node* const new_node = new node(_data);
        new_node->next = head.load(); // atomic operation
        while (!head.compare_exchange_weak(new_node->next, new_node));
    }

    std::shared_ptr<T> pop() { // result is a value of deleted node 
        std::atomic<void*>& hp = get_hazard_pointer_for_current_thread();
        node* old_head = head.load();

        do{
            hp.store(old_head); // set hazard ptr
        } while (old_head && head.compare_exchange_weak(old_head, old_head->next));
        
        hp.store(nullptr);

        std::shared_ptr<T> res;
        if (old_head) {
            res.swap(old_head->data);

            if (any_outstanding_hazards(old_head)) {
                reclaim_later(old_head);
            } else {
                delete old_head;
            }

            delete_nodes_with_no_hazards();
        }

        return res;
    }
};