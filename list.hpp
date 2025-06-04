#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "algorithm.hpp"
#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
    template <typename T> class list
    {

    protected:
        class fake_node
        {
        public:
            /**
             * add data members and constructors & destructor
             */
            fake_node *pre, *nxt;
            fake_node(fake_node *prev, fake_node *next) : pre(prev), nxt(next)
            {
            }
            virtual ~fake_node()
            {
            }
        };
        // using value_type = T;
        class node : public fake_node
        {
        public:
            /**
             * add data members and constructors & destructor
             */
            T val;
            node()
            {
            }
            node(const T &value) : val(value), fake_node(nullptr, nullptr)
            {
            }
            node(const T &value, fake_node *prev, fake_node *next) : val(value), fake_node(prev, next)
            {
            }
        };

    protected:
        /**
         * add data members for linked list as protected members
         */

        /**
         * insert node cur before node pos
         * return the inserted node cur
         */
        fake_node *insert(fake_node *pos, fake_node *cur)
        {
            if (pos == nullptr)
            {
                return cur;
            }
            if (pos->pre != nullptr)
            {
                cur->pre = pos->pre;
                pos->pre->nxt = cur;
            }
            pos->pre = cur;
            cur->nxt = pos;
            return cur;
        }
        /**
         * remove node pos from list (no need to delete the node)
         * return the removed node pos
         */
        fake_node *erase(node *pos)
        {
            if (pos->pre != nullptr)
                pos->pre->nxt = pos->nxt;
            return pos;
        }
        fake_node *myerase(fake_node *pos)
        {
            auto tmp = pos->nxt;
            if (pos->pre != nullptr)
            {
                pos->pre->nxt = tmp;
                tmp->pre = pos->pre;
            }
            node *derived_ptr = dynamic_cast<node *>(pos);
            if (derived_ptr)
            {
                delete derived_ptr;
            }
            else
            {
                delete pos;
            }
            return tmp;
        }

    public:
        using pointer = fake_node *;
        class const_iterator;
        class iterator
        {
            friend const_iterator;
            friend list;

        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            pointer ptr;
            pointer head;
            pointer tail;

        public:
            /**
             *  constructor
             */
            explicit iterator(pointer p, pointer h, pointer t) : ptr(p), head(h), tail(t)
            {
            }
            /**
             * destructor
             */

            /**
             * iter++
             */
            iterator operator++(int)
            {
                if (ptr == nullptr || ptr == tail)
                {
                    throw invalid_iterator();
                }
                iterator tmp = *this;
                ptr = ptr->nxt;
                return tmp;
            }
            /**
             * ++iter
             */
            iterator &operator++()
            {
                if (ptr == nullptr || ptr == tail)
                {
                    throw invalid_iterator();
                }
                ptr = ptr->nxt;
                return *this;
            }
            /**
             * iter--
             */
            iterator operator--(int)
            {
                if (ptr == nullptr || ptr == head->nxt || ptr == head)
                {
                    throw invalid_iterator();
                }
                iterator tmp = *this;
                ptr = ptr->pre;
                return tmp;
            }
            /**
             * --iter
             */
            iterator &operator--()
            {
                if (ptr == nullptr || ptr == head || ptr == head->nxt)
                {
                    throw invalid_iterator();
                }
                ptr = ptr->pre;
                return *this;
            }
            /**
             * TODO *it
             * remember to throw if iterator is invalid
             */
            T &operator*() const
            {
                if (ptr == tail)
                {
                    throw invalid_iterator();
                }
                return static_cast<node *>(ptr)->val;
            }
            /**
             * TODO it->field
             * remember to throw if iterator is invalid
             */
            T *operator->() const noexcept
            {
                if (ptr == tail)
                {
                    throw invalid_iterator();
                }
                return &(static_cast<node *>(ptr)->val);
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const
            {
                return ptr == rhs.ptr;
            }
            bool operator==(const const_iterator &rhs) const
            {
                return ptr == rhs.ptr;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const
            {
                return ptr != rhs.ptr;
            }
            bool operator!=(const const_iterator &rhs) const
            {
                return ptr != rhs.ptr;
            }
        };
        /**
         * TODO
         * has same function as iterator, just for a const object.
         * should be able to construct from an iterator.
         */
        class const_iterator
        {
            friend iterator;

        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            const fake_node *ptr;
            const fake_node *head, *tail;

        public:
            /**
             *  constructor
             */
            explicit const_iterator(pointer p, pointer h, pointer t) : ptr(p), head(h), tail(t)
            {
            }
            explicit const_iterator(iterator p) : ptr(p.ptr), head(p.head), tail(p.tail)
            {
            }
            /**
             * destructor
             */

            /**
             * iter++
             */
            const_iterator operator++(int)
            {
                if (ptr == nullptr || ptr == tail)
                {
                    throw invalid_iterator();
                }
                const_iterator tmp = *this;
                ptr = ptr->nxt;
                return tmp;
            }
            /**
             * ++iter
             */
            const_iterator &operator++()
            {
                if (ptr == nullptr || ptr == tail)
                {
                    throw invalid_iterator();
                }
                ptr = ptr->nxt;
                return *this;
            }
            /**
             * iter--
             */
            const_iterator operator--(int)
            {
                if (ptr == nullptr || ptr == head || ptr == head->nxt)
                {
                    throw invalid_iterator();
                }
                const_iterator tmp = *this;
                ptr = ptr->pre;
                return tmp;
            }
            /**
             * --iter
             */
            const_iterator &operator--()
            {
                if (ptr == nullptr || ptr == head || ptr == head->nxt)
                {
                    throw invalid_iterator();
                }
                ptr = ptr->pre;
                return *this;
            }
            /**
             * TODO *it
             * remember to throw if iterator is invalid
             */
            const T &operator*() const
            {
                if (ptr == tail)
                {
                    throw invalid_iterator();
                }
                return static_cast<const node *>(ptr)->val;
            }
            /**
             * TODO it->field
             * remember to throw if iterator is invalid
             */
            const T *operator->() const noexcept
            {
                if (ptr == tail)
                {
                    throw invalid_iterator();
                }
                return &(static_cast<const node *>(ptr)->val);
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
             /**
              * 
              * @param rhs
              * @return
              */
            bool operator==(const const_iterator &rhs) const
            {
                return ptr == rhs.ptr;
            }
            bool operator==(const iterator &rhs) const
            {
                return ptr == rhs.ptr;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const const_iterator &rhs) const
            {
                return ptr != rhs.ptr;
            }
            bool operator!=(const iterator &rhs) const
            {
                return ptr != rhs.ptr;
            }
        };

    private:
        pointer head;
        pointer tail;
        size_t list_size;

    public:
        /**
         * TODO Constructs
         * Atleast two: default constructor, copy constructor
         */

        list()
        {
            head = new fake_node(nullptr, nullptr);
            tail = new fake_node(head, nullptr);
            head->nxt = tail;
            list_size = 0;
        }
        list(const list &other)
        {
            head = new fake_node(nullptr, nullptr);
            tail = new fake_node(head, nullptr);
            head->nxt = tail;
            list_size = 0;
            node *cnt = static_cast<node *>(other.head->nxt);
            while (cnt != other.tail)
            {
                push_back(cnt->val);
                cnt = static_cast<node *>(cnt->nxt);
            }
        }
        /**
         * TODO Destructor
         */
        virtual ~list()
        {

            while (head != nullptr)
            {
                auto tmp = head->nxt;
                node *derived_ptr = dynamic_cast<node *>(head);
                if (derived_ptr)
                {
                    delete derived_ptr;
                }
                else
                {
                    delete head;
                }
                head = tmp;
            }

            list_size = 0;
        }
        /**
         * TODO Assignment operator
         */
        list &operator=(const list &other)
        {
            if (&other == this)
            {
                return *this;
            }
            clear();
            head = new fake_node(nullptr, nullptr);
            tail = new fake_node(head, nullptr);
            head->nxt = tail;
            list_size = 0;
            node *cnt = static_cast<node *>(other.head->nxt);
            while (cnt != other.tail)
            {
                push_back(cnt->val);
                cnt = static_cast<node *>(cnt->nxt);
            }
            return *this;
        }
        /**
         * access the first / last element
         * throw container_is_empty when the container is empty.
         */
        const T &front() const
        {
            if (list_size == 0)
            {
                throw container_is_empty();
            }
            return static_cast<node *>(head->nxt)->val;
        }
        const T &back() const
        {
            if (list_size == 0)
            {
                throw container_is_empty();
            }
            return static_cast<node *>(tail->pre)->val;
        }
        /**
         * returns an iterator to the beginning.
         */
        iterator begin()
        {
            return iterator(head->nxt, head, tail);
        }
        const_iterator cbegin() const
        {
            return const_iterator(head->nxt, head, tail);
        }
        /**
         * returns an iterator to the end.
         */
        iterator end()
        {
            return iterator(tail, head, tail);
        }
        const_iterator cend() const
        {
            return const_iterator(tail, head, tail);
        }
        /**
         * checks whether the container is empty.
         */
        virtual bool empty() const
        {
            return list_size == 0;
        }
        /**
         * returns the number of elements
         */
        virtual size_t size() const
        {
            return list_size;
        }

        /**
         * clears the contents
         */
        virtual void clear()
        {
            while (head != nullptr)
            {
                auto tmp = head->nxt;
                node *derived_ptr = dynamic_cast<node *>(head);
                if (derived_ptr)
                {
                    delete derived_ptr;
                }
                else
                {
                    delete head;
                }
                head = tmp;
            }
            list_size = 0;
        }
        /**
         * insert value before pos (pos may be the end() iterator)
         * return an iterator pointing to the inserted value
         * throw if the iterator is invalid
         */
        virtual iterator insert(iterator pos, const T &value)
        {
            if (pos.head != head || pos.tail != tail || pos.ptr == head)
            {
                throw invalid_iterator();
            }
            fake_node *tmp = new node(value);
            insert(pos.ptr, tmp);
            list_size++;
            return iterator(tmp, head, tail);
        }
        /**
         * remove the element at pos (the end() iterator is invalid)
         * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be
         * returned. throw if the container is empty, the iterator is invalid
         */
        virtual iterator erase(iterator pos)
        {
            if (head->nxt == tail)
            {
                throw container_is_empty();
            }
            if (pos.ptr == head || pos.ptr == tail || pos.head != head || pos.tail != tail)
            {
                throw invalid_iterator();
            }
            pointer tmp = myerase(pos.ptr);
            --list_size;

            return iterator(tmp, head, tail);
        }
        /**
         * adds an element to the end
         */
        void push_back(const T &value)
        {
            insert(end(), value);
        }
        /**
         * removes the last element
         * throw when the container is empty.
         */
        void pop_back()
        {
            if (size() == 0)
            {
                throw container_is_empty();
            }
            erase(iterator(tail->pre, head, tail));
        }
        /**
         * inserts an element to the beginning.
         */
        void push_front(const T &value)
        {
            insert(begin(), value);
        }
        /**
         * removes the first element.
         * throw when the container is empty.
         */
        void pop_front()
        {
            if (size() == 0)
            {
                throw container_is_empty();
            }
            erase(begin());
        }
        /**
         * sort the values in ascending order with operator< of T
         */
        void sort()
        {
            quick_sort(static_cast<node *>(head->nxt), static_cast<node *>(tail->pre), 0, list_size - 1);
        }

        void insert_sort(node *l, node *r)
        {
            node *i, *j;
            for (j = static_cast<node *>(l->nxt); j != static_cast<node *>(r->nxt); j = static_cast<node *>(j->nxt))
            {
                T key(j->val);
                i = static_cast<node *>(j->pre);

                while (i != static_cast<node *>(l->pre) && key < i->val)
                {
                    static_cast<node *>(i->nxt)->val = i->val;
                    i = static_cast<node *>(i->pre);
                }
                static_cast<node *>(i->nxt)->val = key;
            }
        }

        void quick_sort(node *l, node *r, long long st, long long ed)
        {
            long long now_len = ed - st + 1;
            if (now_len <= 1)
                return;
            if (now_len <= 10)
            {
                insert_sort(l, r);
                return;
            }
            long long start = st - 1;
            long long end = ed + 1;
            node *i = static_cast<node *>(l->pre);
            node *j = static_cast<node *>(r->nxt);
            T k(l->val);
            while (start < end)
            {
                do
                    start++, i = static_cast<node *>(i->nxt);
                while (i->val < k);
                do
                    end--, j = static_cast<node *>(j->pre);
                while (k < j->val);
                if (start < end)
                {
                    T t = i->val;
                    i->val = j->val;
                    j->val = t;
                }
            }
            quick_sort(l, j, st, end);
            quick_sort(static_cast<node *>(j->nxt), r, end + 1, ed);
        }
        /**
         * merge two sorted lists into one (both in ascending order)
         * compare with operator< of T
         * container other becomes empty after the operation
         * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
         * the order of equivalent elements of *this and other does not change.
         * no elements are copied or moved
         */
        void merge(list &other)
        {
            node *i = static_cast<node *>(head->nxt);
            node *j = static_cast<node *>(other.head->nxt);

            if (list_size == 0 && other.list_size != 0)
            {
                *this = other;
                return;
            }
            else if (other.list_size == 0)
            {
                return;
            }
            pointer rhead = new fake_node(nullptr, nullptr);
            pointer cur;
            cur = rhead;
            while (i != tail && j != other.tail)
            {
                if (i->val < j->val || i->val == j->val)
                {
                    cur->nxt = i;
                    cur->nxt->pre = cur;
                    cur = cur->nxt;
                    i = static_cast<node *>(i->nxt);
                }
                else
                {
                    cur->nxt = j;
                    cur->nxt->pre = cur;
                    cur = cur->nxt;
                    j = static_cast<node *>(j->nxt);
                }
            }
            while (i != tail)
            {
                cur->nxt = i;
                cur->nxt->pre = cur;
                cur = cur->nxt;
                i = static_cast<node *>(i->nxt);
            }
            while (j != other.tail)
            {
                cur->nxt = j;
                cur->nxt->pre = cur;
                cur = cur->nxt;
                j = static_cast<node *>(j->nxt);
            }
            cur->nxt = tail;
            delete head;
            head = rhead;
            list_size += other.list_size;
            other.head->nxt = other.tail;
            other.tail->pre = other.head;
            other.list_size = 0;
        }
        /**
         * reverse the order of the elements
         * no elements are copied or moved
         */
        void reverse()
        {
            tail->pre->nxt = nullptr;
            delete tail;
            tail = head;
            head = head->nxt;
            tail->pre = head;
            tail->nxt = nullptr;
            while (head->nxt != nullptr)
            {
                pointer tmp = head->nxt;
                head->nxt = head->pre;
                head->pre = tmp;
                head = tmp;
            }
            head->nxt = head->pre;
            head->pre = new fake_node(nullptr, head);
            head = head->pre;
        }
        /**
         * remove all consecutive duplicate elements from the container
         * only the first element in each group of equal elements is left
         * use operator== of T to compare the elements.
         */
        void unique()
        {
            pointer i = head->nxt;
            pointer j;
            while (i != tail)
            {
                j = i->nxt;
                while (j != tail && static_cast<node *>(i)->val == static_cast<node *>(j)->val && j != tail)
                {
                    j = myerase(j);
                    --list_size;
                }
                i = j;
            }
        }
    };
/**
 * friend function
 */
    template <typename U> bool isequal(const typename list<U>::iterator &it1, const typename list<U>::const_iterator &it2)
    {
        return it1.ptr == it2.ptr;
    }
} // namespace sjtu

#endif // SJTU_LIST_HPP
