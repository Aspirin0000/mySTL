#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
    template <typename T> class vector
    {
    public:
        /**
         * TODO
         * a type for actions of the elements of a vector, and you should write
         *   a class named const_iterator with same interfaces.
         */
        /**
         * you can see RandomAccessIterator at CppReference for help.
         */
        class const_iterator;
        class iterator
        {
            // The following code is written for the C++ type_traits library.
            // Type traits is a C++ feature for describing certain properties of a type.
            // For instance, for an iterator, iterator::value_type is the type that the
            // iterator points to.
            // STL algorithms and containers may use these type_traits (e.g. the following
            // typedef) to work properly. In particular, without the following code,
            // @code{std::sort(iter, iter1);} would not compile.
            // See these websites for more information:
            // https://en.cppreference.com/w/cpp/header/type_traits
            // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
            // About iterator_category: https://en.cppreference.com/w/cpp/iterator
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::output_iterator_tag;

        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            pointer ptr;     // 指向当前元素的指针
            pointer vec_ptr; // 指向数组的指针
            size_t index;    // 当前元素的索引
            size_t vec_size; // 当前数组的大小
        public:
            /**
             *
             * Construct
             */
            explicit iterator(pointer p, pointer vec_p, size_t idx, size_t vec_s)
                    : ptr(p), vec_ptr(vec_p), index(idx), vec_size(vec_s)
            {
            }
            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            iterator operator+(const int &n) const
            {
                // TODO
                if (index + n < vec_size)
                {
                    return iterator(ptr + n, vec_ptr, index + n, vec_size);
                }
                else // 返回end()
                {
                    return iterator(vec_ptr + vec_size, vec_ptr, vec_size, vec_size);
                }
            }
            iterator operator-(const int &n) const
            {
                // TODO
                if (index - n >= 0)
                {
                    return iterator(ptr - n, vec_ptr, index - n, vec_size);
                }
                else // 返回begin()
                {
                    return iterator(vec_ptr, vec_ptr, 0, vec_size);
                }
            }
            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const
            {
                // TODO
                if (rhs.vec_ptr != vec_ptr)
                {
                    throw invalid_iterator();
                }
                return ptr - rhs.ptr;
            }
            iterator &operator+=(const int &n)
            {
                // TODO
                if (index + n < vec_size)
                {
                    ptr += n;
                    index += n;
                }
                else // 赋值end()
                {
                    ptr = vec_ptr + vec_size;
                    index = vec_size;
                }
                return *this;
            }
            iterator &operator-=(const int &n)
            {
                // TODO
                if (index - n >= 0)
                {
                    ptr -= n;
                    index -= n;
                }
                else // 赋值begin()
                {
                    ptr = vec_ptr;
                    index = 0;
                }
                return *this;
            }
            /**
             * TODO iter++
             */
            iterator operator++(int)
            {
                iterator temp = *this;
                *this += 1;
                return temp;
            }
            /**
             * TODO ++iter
             */
            iterator &operator++()
            {
                return *this += 1;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int)
            {
                iterator temp = *this;
                *this -= 1;
                return temp;
            }
            /**
             * TODO --iter
             */
            iterator &operator--()
            {
                return *this -= 1;
            }
            /**
             * TODO *it
             */
            T &operator*() const
            {
                return *ptr;
            }
            /**
             * friend function
             */
            friend bool isequal(const iterator &it1, const const_iterator &it2);
            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const
            {
                return ptr == rhs.ptr;
            }
            bool operator==(const const_iterator &rhs) const
            {
                return isequal(*this, rhs);
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
                return !isequal(*this, rhs);
            }
        };
        /**
         * TODO
         * has same function as iterator, just for a const object.
         */
        class const_iterator
        {
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::output_iterator_tag;

        private:
            /*TODO*/
            const value_type *ptr;     // 指向当前元素的指针
            const value_type *vec_ptr; // 指向数组的指针
            size_t index;              // 当前元素的索引
            size_t vec_size;           // 当前数组的大小
        public:
            /**
             *
             * Construct
             */
            explicit const_iterator(pointer p, pointer vec_p, size_t idx, size_t vec_s)
                    : ptr(p), vec_ptr(vec_p), index(idx), vec_size(vec_s)
            {
            }

            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            const_iterator operator+(const int &n) const
            {
                // TODO
                if (index + n < vec_size)
                {
                    return const_iterator(ptr + n, vec_ptr, index + n, vec_size);
                }
                else // 返回end()
                {
                    return const_iterator(vec_ptr + vec_size, vec_ptr, vec_size, vec_size);
                }
            }
            const_iterator operator-(const int &n) const
            {
                // TODO
                if (index - n >= 0)
                {
                    return const_iterator(ptr - n, vec_ptr, index - n, vec_size);
                }
                else // 返回begin()
                {
                    return const_iterator(vec_ptr, vec_ptr, 0, vec_size);
                }
            }
            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const const_iterator &rhs) const
            {
                // TODO
                if (rhs.vec_ptr != vec_ptr)
                {
                    throw invalid_iterator();
                }
                return ptr - rhs.ptr;
            }
            const_iterator &operator+=(const int &n)
            {
                // TODO
                if (index + n < vec_size)
                {
                    ptr += n;
                    index += n;
                }
                else // 赋值end()
                {
                    ptr = vec_ptr + vec_size;
                    index = vec_size;
                }
                return *this;
            }
            const_iterator &operator-=(const int &n)
            {
                // TODO
                if (index - n >= 0)
                {
                    ptr -= n;
                    index -= n;
                }
                else // 赋值begin()
                {
                    ptr = vec_ptr;
                    index = 0;
                }
                return *this;
            }
            /**
             * TODO iter++
             */
            const_iterator operator++(int)
            {
                const_iterator temp = *this;
                *this += 1;
                return temp;
            }
            /**
             * TODO ++iter
             */
            const_iterator &operator++()
            {
                return *this += 1;
            }
            /**
             * TODO iter--
             */
            const_iterator operator--(int)
            {
                const_iterator temp = *this;
                *this -= 1;
                return temp;
            }
            /**
             * TODO --iter
             */
            const_iterator &operator--()
            {
                return *this -= 1;
            }
            /**
             * TODO *it
             */
            const T &operator*() const
            {
                return *ptr;
            }
            /**
             * friend function
             */
            friend bool isequal(const iterator &it1, const const_iterator &it2);
            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const const_iterator &rhs) const
            {
                return ptr == rhs.ptr;
            }
            bool operator==(const iterator &rhs) const
            {
                return isequal(rhs, *this);
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
                return !isequal(rhs, *this);
            }
        };
        /**
         * data members
         */
    private:
        T *data;
        size_t m_size;
        size_t capacity;
        /**
         * TODO Constructs
         * At least two: default constructor, copy constructor
         */
    public:
        vector() : data(nullptr), m_size(0), capacity(0)
        {
        }
        vector(const vector &other)
        {
            capacity = other.capacity;
            m_size = other.size();
            data = static_cast<T *>(::operator new(capacity * sizeof(T))); // 分配新的内存空间
            for (size_t i = 0; i < m_size; ++i)
            {
                new (&data[i]) T(other.data[i]);
            }
        }
        /**
         * TODO Destructor
         */
        ~vector()
        {
            for (int i = 0; i < m_size; i++)
            {
                data[i].~T(); // 手动调用析构函数
            }
            m_size = 0;
            capacity = 0;
            ::operator delete(data);
        }
        /**
         * TODO Assignment operator
         */
        vector &operator=(const vector &other)
        {
            if (this == &other)
            {
                return *this;
            }

            if (capacity <= other.size())
            {
                // 重新分配更大的内存空间
                reserve(other.size());
            }
            for (int i = 0; i < m_size; i++)
            {
                data[i].~T();
            }
            m_size = other.size();

            for (size_t i = 0; i < m_size; ++i)
            {
                data[i] = other.data[i];
            }

            return *this;
        }
        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         */
        T &at(const size_t &pos)
        {
            if (pos < 0 || pos >= m_size)
            {
                throw index_out_of_bound();
            }
            return data[pos];
        }
        const T &at(const size_t &pos) const
        {
            if (pos < 0 || pos >= m_size)
            {
                throw index_out_of_bound();
            }
            return data[pos];
        }
        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary but I want you to do.
         */
        T &operator[](const size_t &pos)
        {
            if (pos < 0 || pos >= m_size)
            {
                throw index_out_of_bound();
            }
            return data[pos];
        }
        const T &operator[](const size_t &pos) const
        {
            if (pos < 0 || pos >= m_size)
            {
                throw index_out_of_bound();
            }
            return data[pos];
        }
        /**
         * access the first element.
         * throw container_is_empty if size == 0
         */
        const T &front() const
        {
            if (m_size == 0)
            {
                throw container_is_empty();
            }
            return data[0];
        }
        /**
         * access the last element.
         * throw container_is_empty if size == 0
         */
        const T &back() const
        {
            if (m_size == 0)
            {
                throw container_is_empty();
            }
            return data[m_size - 1];
        }
        /**
         * returns an iterator to the beginning.
         */
        iterator begin()
        {
            return iterator(&data[0], data, 0, m_size);
        }
        const_iterator cbegin() const
        {
            return const_iterator(&data[0], data, 0, m_size);
        }
        /**
         * returns an iterator to the end.
         */
        iterator end()
        {
            return iterator(&data[m_size], data, m_size, m_size);
        }
        const_iterator cend() const
        {
            return const_iterator(&data[m_size], data, m_size, m_size);
        }
        /**
         * checks whether the container is empty
         */
        bool empty() const
        {
            return m_size == 0;
        }
        /**
         * returns the number of elements
         */
        size_t size() const
        {
            return m_size;
        }
        /**
         * clears the contents
         */
        void clear()
        {
            m_size = 0;
        }
        /**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */
        iterator insert(iterator pos, const T &value)
        {
            int pose = pos - begin();
            if (m_size + 1 >= capacity)
            {
                reserve(m_size + 1);
            }
            for (int i = m_size; i > pose; i--)
            {
                data[i] = data[i - 1];
            }
            data[pose] = value;
            m_size++;
            return begin() + pose;
        }
        /**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will
         * increase 1.)
         */
        iterator insert(const size_t &ind, const T &value)
        {
            if (ind < 0 || ind > m_size)
            {
                throw index_out_of_bound();
            }
            if (m_size + 1 >= capacity)
            {
                reserve(m_size + 1);
            }
            for (int i = m_size; i > ind; i--)
            {
                data[i] = data[i - 1];
            }
            data[ind] = value;
            m_size++;
            return begin() + ind;
        }
        /**
         * removes the element at pos.
         * return an iterator pointing to the following element.
         * If the iterator pos refers the last element, the end() iterator is returned.
         */
        iterator erase(iterator pos)
        {
            int pose = pos - begin();
            for (int i = pose; i < m_size - 1; i++)
            {
                data[i] = data[i + 1];
            }
            data[m_size - 1].~T();
            m_size--;
            return begin() + pose;
        }
        /**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= size
         */
        iterator erase(const size_t &ind)
        {
            if (ind < 0 || ind >= m_size)
            {
                throw index_out_of_bound();
            }
            for (size_t i = ind; i < m_size - 1; i++)
            {
                data[i] = data[i + 1];
            }
            data[m_size - 1].~T();
            m_size--;
            return begin() + ind;
        }
        /**
         * adds an element to the end.
         */
        void push_back(const T &value)
        {
            if (m_size + 1 >= capacity)
            {
                reserve(m_size + 1);
            }
            new (&data[m_size]) T(value);
            m_size++;
        }
        /**
         * remove the last element from the end.
         * throw container_is_empty if size() == 0
         */
        void pop_back()
        {
            if (m_size == 0)
            {
                throw container_is_empty();
            }
            data[m_size - 1].~T();
            m_size--;
        }

        /**
         * Double the capacity and copy original elements
         */
        void reserve(size_t si)
        {
            while (capacity <= si + 1)
                capacity = (capacity == 0 ? 4 : capacity * 2);
            T *new_data = static_cast<T *>(::operator new(capacity * sizeof(T))); // 分配新的内存空间

            for (size_t i = 0; i < m_size; ++i)
            {
                new (&new_data[i]) T(data[i]);
                data[i].~T();
            }

            ::operator delete(data); // 释放原有的内存空间
            data = new_data;
        }
    };

/**
 * friend function
 */
    template <typename U>
    bool isequal(const typename vector<U>::iterator &it1, const typename vector<U>::const_iterator &it2)
    {
        return it1.ptr == it2.ptr;
    }

} // namespace sjtu

#endif
