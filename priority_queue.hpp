#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include "exceptions.hpp"
#include <cstddef>
#include <functional>

namespace sjtu
{

/**
 * a container like std::priority_queue which is a heap internal.
 */
    template <typename T, class Compare = std::less<T>> class priority_queue
    {
        struct Node
        {
            T val;
            Node *left, *right;
            Node(T value, Node *l = nullptr, Node *r = nullptr) : val(value), left(l), right(r)
            {
            }
        };

    private:
        Node *root;
        size_t siz;
        void dfs(Node *&a, const Node *b)
        {
            if (b == nullptr)
            {
                a = nullptr;
                return;
            }
            a = new Node(b->val);
            dfs(a->left, b->left);
            dfs(a->right, b->right);
        }
        void destory(Node *&nod)
        {
            if (nod == nullptr)
                return;
            if(nod -> left != nullptr)
                destory(nod->left);
            if(nod -> right != nullptr)
                destory(nod->right);
            delete nod;
        }

    public:
        /**
         * TODO constructors
         */
        priority_queue() : root(nullptr), siz(0)
        {
        }
        priority_queue(const priority_queue &other)
        {
            siz = other.siz;
            dfs(root, other.root);
        }

        /**
         * TODO deconstructor
         */
        ~priority_queue()
        {
            if (siz > 0)
                destory(root);
        }

        /**
         * TODO Assignment operator
         */
        priority_queue &operator=(const priority_queue &other)
        {
            if (&other == this)
                return *this;
            destory(root);
            siz = other.siz;
            dfs(root, other.root);
            return *this;
        }
        /**
         * get the top of the queue.
         * @return a reference of the top element.
         * throw container_is_empty if empty() returns true;
         */
        const T &top() const
        {
            if (siz == 0)
            {
                throw container_is_empty();
            }
            return root->val;
        }
        /**
         * TODO
         * push new element to the priority queue.
         */
        void push(const T &e)
        {
            Node *t = new Node(e);
            siz++;
            root = merge(root, t);
        }
        /**
         * TODO
         * delete the top element.
         * throw container_is_empty if empty() returns true;
         */
        void pop()
        {
            if (siz == 0)
            {
                throw container_is_empty();
            }
            Node *l = root->left;
            Node *r = root->right;
            delete root;
            root = merge(l, r);
            siz--;
        }
        /**
         * return the number of the elements.
         */
        size_t size() const
        {
            return siz;
        }
        /**
         * check if the container has at least an element.
         * @return true if it is empty, false if it has at least an element.
         */
        bool empty() const
        {
            return siz == 0;
        }
        /**
         * merge two priority_queues with at least O(logn) complexity.
         * clear the other priority_queue.
         */
        void merge(priority_queue &other)
        {
            root = merge(root, other.root);
            siz += other.siz;
            other.siz = 0;
        }
        Node *merge(Node *&x, Node *&y)
        {
            if (x == nullptr)
                return y;
            if (y == nullptr)
                return x;
            Compare com;
            if (com(x->val, y->val))
            {
                Node *t = x;
                x = y;
                y = t;
            }

            Node *tmp = merge(x->right, y);
            x->right = x->left;
            x->left = tmp;

            return x;
        }
    };

} // namespace sjtu

#endif