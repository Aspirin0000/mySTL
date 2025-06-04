#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include "exceptions.hpp"
#include "utility.hpp"
#include <cstddef>
#include <functional>

namespace sjtu
{

    template <class Key, class T, class Compare = std::less<Key>> class map
    {
    public:
        typedef pair<const Key, T> value_type;
        // 引入红黑树类
    public:
        class const_iterator;
        class iterator;

    private:
        class RedBlackTree
        {
            friend class map;
            friend class const_iterator;
            friend class iterator;

        private:
            const bool RED = 1;
            const bool BLACK = 0;
            struct _value_type
            {
                value_type value;
                _value_type(Key k, T t) : value(k, t)
                {
                }
                _value_type(const _value_type &other) : value(other.value.first, other.value.second)
                {
                }
                bool operator==(const _value_type &other) const
                {
                    return (!Compare()(value.first, other.value.first)) &&
                           (!Compare()(other.value.first, value.first)); // 只根据键值判断是否相同
                }
                bool operator!=(const _value_type &other) const
                {
                    return !(value.first == other.value.first);
                }
                bool operator<(const _value_type &other) const
                {
                    return Compare()(value.first, other.value.first);
                }
                bool operator>(const _value_type &other) const
                {
                    return Compare()(other.value.first, value.first);
                }
            };
            struct Node
            {
                Node *left;
                Node *right;
                Node *parent;
                bool color;
                _value_type element;
                Node(const _value_type &val, const bool &col, Node *p = nullptr, Node *l = nullptr, Node *r = nullptr)
                        : element(val), color(col), parent(p), left(l), right(r)
                {
                }
            };
            Node *root;
            size_t siz;
            Node *copy(Node *cur, const Node *other, Node *parent, bool flag)
            {
                if (other != nullptr)
                {
                    cur = new Node(other->element, other->color, parent, nullptr, nullptr);
                    if (parent != nullptr)
                    {
                        if (flag == 0)
                            parent->left = cur;
                        else
                            parent->right = cur;
                    }
                    copy(cur->left, other->left, cur, 0);
                    copy(cur->right, other->right, cur, 1);
                }
                else
                    cur = nullptr;
                return cur;
            }
            RedBlackTree() : root(nullptr), siz(0)
            {
            }
            RedBlackTree(const RedBlackTree &other)
            {
                siz = other.siz;
                root = copy(root, other.root, nullptr, 0);
            }
            ~RedBlackTree()
            {
                clear(root);
                siz = 0;
                root = nullptr;
            }
            void clear(Node *cur)
            {
                if (cur == nullptr || siz == 0)
                    return;
                clear(cur->left);
                clear(cur->right);
                siz--;
                cur->left = nullptr;
                cur->right = nullptr;
                delete cur;
                cur = nullptr;
            }
            T &find(const Key &k)
            {
                Node *res = search(root, k);
                if (res == nullptr)
                {
                    throw index_out_of_bound();
                    // return T();
                }
                return res->element.value.second;
            }
            Node *search(Node *cur, const Key &k) const
            {
                if (cur == nullptr)
                    return nullptr;
                if ((!Compare()(cur->element.value.first, k)) && (!Compare()(k, cur->element.value.first)))
                    return cur;
                if (Compare()(cur->element.value.first, k))
                    return search(cur->right, k);
                return search(cur->left, k);
            }
            // 几个Node小函数
            bool is_left(const Node *cur)
            {
                if (cur->parent == nullptr)
                    return 0;
                return cur->parent->left == cur;
            }
            bool is_right(const Node *cur)
            {
                if (cur->parent == nullptr)
                    return 0;
                return cur->parent->right == cur;
            }
            Node *find_uncle(const Node *cur)
            {
                if (cur == nullptr || cur->parent == nullptr || cur->parent->parent == nullptr)
                    return nullptr;
                ;
                if (cur->parent == cur->parent->parent->left)
                    return cur->parent->parent->right;
                return cur->parent->parent->left;
            }
            void change_color(Node *cur)
            {
                if (cur->color == RED)
                    cur->color = BLACK;
                else
                    cur->color = RED;
            }
            Node *find_sibling(const Node *cur)
            {
                if (cur->parent == nullptr)
                    return nullptr;
                if (is_left(cur))
                    return cur->parent->right;
                return cur->parent->left;
            }
            // 右旋操作
            void right_rotate(Node *cur)
            {
                Node *replace = cur->left;  // 用左孩子代替原来的节点
                cur->left = replace->right; // 左孩子的右孩子变为新的左孩子
                if (replace->right != nullptr)
                {
                    replace->right->parent = cur;
                }
                replace->parent = cur->parent;
                if (replace->parent == nullptr) // 说明replace 成为根节点
                {
                    root = replace;
                }
                else
                {
                    if (is_left(cur))
                        cur->parent->left = replace;
                    else if (is_right(cur))
                        cur->parent->right = replace;
                }
                // 至此已经处理好replace的孩子和cur的关系  cur的父亲和replace的关系
                cur->parent = replace;
                replace->right = cur; // 倒反天罡（）
            }
            // 左旋操作
            void left_rotate(Node *cur)
            {
                Node *replace = cur->right; // 用右孩子代替原来的节点
                cur->right = replace->left; // 右孩子的左孩子变为新的右孩子
                if (replace->left != nullptr)
                {
                    replace->left->parent = cur;
                }
                replace->parent = cur->parent;
                if (replace->parent == nullptr) // 说明replace 成为根节点
                {
                    root = replace;
                }
                else
                {
                    if (is_left(cur))
                        cur->parent->left = replace;
                    else if (is_right(cur))
                        cur->parent->right = replace;
                }
                // 至此已经处理好replace的孩子和cur的关系  cur的父亲和replace的关系
                cur->parent = replace;
                replace->left = cur; // 倒反天罡（）
            }

            void insert_fix(Node *cur) // 维护插入后红黑树的性质
            {
                if (cur->parent == nullptr) // 当前节点没有父亲，为根节点
                {
                    cur->color = BLACK; // 为了满足 根叶黑
                    return;
                }
                if (cur->parent->color == BLACK) // 当前节点父亲节点为黑色，没问题
                {
                    return;
                }
                if (cur->parent->parent == nullptr) // 当前节点没有爷爷，且父亲节点也是红色
                {
                    cur->parent->color = BLACK; // 虽然这种情况不可能出现，但是以防万一（）
                }
                Node *uncle = find_uncle(cur);
                Node *grandfather = cur->parent->parent;
                Node *father = cur->parent;
                if (uncle == nullptr || uncle->color == BLACK) // 叔叔节点为黑色
                {
                    if (grandfather->left == father && father->left == cur) // LL型  右旋
                    {
                        right_rotate(grandfather); // 爷旋转
                        change_color(father);
                        change_color(grandfather); // 旋转点和中心点变色
                    }
                    else if (grandfather->right == father && father->right == cur) // RR型 左旋
                    {
                        left_rotate(grandfather); // 爷旋转
                        change_color(father);
                        change_color(grandfather); // 旋转点和中心点变色
                    }
                    else if (grandfather->left == father && father->right == cur) // LR型
                    {
                        left_rotate(father);       // 左旋爷爷的左孩子
                        right_rotate(grandfather); // 右旋爷爷
                        change_color(cur);
                        change_color(grandfather); // 旋转点和中心点变色
                    }
                    else // RL型
                    {
                        right_rotate(father);     // 左旋爷爷的左孩子
                        left_rotate(grandfather); // 右旋爷爷
                        change_color(cur);
                        change_color(grandfather); // 旋转点和中心点变色
                    }
                }
                else // 叔叔节点颜色为红色
                {
                    change_color(cur->parent);
                    change_color(uncle);
                    change_color(cur->parent->parent); // 叔 父 爷 变色
                    insert_fix(cur->parent->parent);   // 递归操作 爷
                }
            }
            sjtu::pair<Node *, bool> insert(const _value_type &ele)
            {

                if (root == nullptr) // 如果根节点为空，直接插入
                {
                    Node *nod = new Node(ele, RED);
                    siz++;
                    root = nod;
                    root->color = BLACK;
                    return {root, 1};
                }
                Node *temp = root;
                Node *father = nullptr;
                while (temp != nullptr)
                {
                    if (temp->element == ele)
                        return {temp, 0}; // 已经插入过相同键值的元素
                    father = temp;
                    if (temp->element < ele) // 说明应该插入在temp的右子树中
                    {
                        temp = temp->right;
                    }
                    else
                    {
                        temp = temp->left;
                    }
                }
                Node *nod = new Node(ele, RED);
                nod->parent = father; // 找到插入位置
                if (father->element < nod->element)
                    father->right = nod;
                else
                    father->left = nod;
                if (father->color == RED) // 出现 红红 相邻，则做调整
                    insert_fix(nod);
                siz++;
                return {nod, 1};
            }
            void swap_node(Node *x, Node *y) // 交换当前节点和其后继的颜色和位置
            {
                bool color = x->color;
                x->color = y->color;
                y->color = color;
                if (y->parent == x) // 后继节点是当前节点的右孩子
                {
                    Node *xp = x->parent;
                    Node *xl = x->left;
                    Node *yl = y->left;
                    Node *yr = y->right;
                    y->parent = xp;
                    if (xp != nullptr)
                    {
                        if (is_left(x))
                            xp->left = y;
                        else
                            xp->right = y;
                    }
                    y->left = xl;
                    if (xl != nullptr)
                    {
                        xl->parent = y;
                    }
                    y->right = x;
                    x->parent = y;
                    x->left = yl;
                    if (yl != nullptr)
                    {
                        yl->parent = x;
                    }
                    x->right = yr;
                    if (yr != nullptr)
                    {
                        yr->parent = x;
                    }
                }
                else
                {
                    Node *xp = x->parent;
                    Node *xl = x->left;
                    Node *xr = x->right;
                    Node *yp = y->parent;
                    Node *yl = y->left;
                    Node *yr = y->right;
                    bool flag = is_left(y);
                    y->parent = xp;
                    if (xp != nullptr)
                    {
                        if (is_left(x))
                            xp->left = y;
                        else
                            xp->right = y;
                    }
                    y->left = xl;
                    if (xl != nullptr)
                    {
                        xl->parent = y;
                    }
                    y->right = xr;
                    if (xr != nullptr)
                    {
                        xr->parent = y;
                    }
                    x->parent = yp;
                    if (yp != nullptr) // yp肯定不是空指针，但是以防万一
                    {
                        if (flag)
                            yp->left = x;
                        else
                            yp->right = x;
                    }
                    x->left = yl;
                    if (yl != nullptr)
                    {
                        yl->parent = x;
                    }
                    x->right = yr;
                    if (yr != nullptr)
                    {
                        yr->parent = x;
                    }
                }
                if (x == root)
                {
                    root = y;
                }
            }
            Node *find_min(Node *cur) // 找到以cur为根的子树中key最小的节点
            {
                if (cur == nullptr)
                    return cur;
                if (cur->left == nullptr)
                    return cur;
                return find_min(cur->left);
            }
            Node *find_max(Node *cur) // 找到以cur为根的子树中key最大的节点
            {
                if (cur == nullptr)
                    return cur;
                if (cur->right == nullptr)
                    return cur;
                return find_max(cur->right);
            }
            Node *find_least_bigger(Node *cur) // 找到树中比cur大最少的节点
            {
                if (cur == nullptr)
                    return cur;
                if (cur->right != nullptr)
                    return find_min(cur->right); // 直接找右子树中最小的
                // 右子树为空，尝试向上找
                if (cur->parent == nullptr)
                    return nullptr; // 根节点而且没有右孩子，直接返回
                if (is_left(cur))
                    return cur->parent; // 如果是左孩子则父节点即为所求
                while (is_right(cur))   // 否则找到第一个转折点
                {
                    cur = cur->parent;
                }
                return cur->parent;
            }
            Node *find_least_smaller(Node *cur) // 与上面对称
            {
                if (cur == nullptr)
                    return cur;
                if (cur->left != nullptr)
                    return find_max(cur->left); // 直接找左子树中最大的
                // 左子树为空，尝试向上找
                if (cur->parent == nullptr)
                    return nullptr; // 根节点而且没有左孩子，直接返回
                if (is_right(cur))
                    return cur->parent; // 如果是右孩子则父节点即为所求
                while (is_left(cur))    // 否则找到第一个转折点
                {
                    cur = cur->parent;
                }
                return cur->parent;
            }
            void erase_fix(Node *cur)
            {
                if (cur == nullptr)
                    return;
                if (cur->color == RED || cur->parent == nullptr) // 如果当前双黑节点为红色或者是根节点，直接变黑
                {
                    cur->color = BLACK;
                    return;
                }
                Node *sibling = find_sibling(cur);
                if (sibling == nullptr) // 不可能出现的情况，但是以防万一
                {
                    return;
                }
                if (sibling->color == BLACK) // 兄弟是黑色
                {

                    if ((sibling->left == nullptr && sibling->right == nullptr) ||
                        (sibling->left != nullptr && sibling->right != nullptr && sibling->left->color == BLACK &&
                         sibling->right->color == BLACK)) // 黑兄两个黑孩子
                    {
                        sibling->color = RED;                              // 兄弟变红
                        if (cur->left == nullptr && cur->right == nullptr) // 说明cur是要删除的节点，不是上移过来的节点
                        {
                            if (cur->parent != nullptr)
                            {
                                if (is_left(cur))
                                    cur->parent->left = nullptr;
                                else if (is_right(cur))
                                    cur->parent->right = nullptr;
                            }
                            delete cur;
                            cur = nullptr;
                        }
                        erase_fix(sibling->parent); // 双黑上移
                    }
                    else // 黑兄至少一个红孩子
                    {
                        if (cur->left == nullptr && cur->right == nullptr) // 说明cur是要删除的节点，不是上移过来的节点
                        {
                            if (cur->parent != nullptr)
                            {
                                if (is_left(cur))
                                    cur->parent->left = nullptr;
                                else
                                    cur->parent->right = nullptr;
                            }
                            delete cur;
                            cur = nullptr;
                        }
                        Node *father = sibling->parent;
                        if (is_left(sibling) && sibling->left != nullptr && sibling->left->color == RED) // LL型
                        {
                            Node *red_son = sibling->left;
                            red_son->color = sibling->color;
                            sibling->color = father->color;
                            father->color = BLACK;
                            right_rotate(father); // r变s,s变p,p变黑,右旋
                        }
                        else if (is_left(sibling) && sibling->right != nullptr && sibling->right->color == RED) // LR型
                        {
                            Node *red_son = sibling->right;
                            red_son->color = father->color;
                            father->color = BLACK;
                            left_rotate(sibling);
                            right_rotate(father); // r变p,p变黑,左旋左孩子,右旋
                        }
                        else if (is_right(sibling) && sibling->right != nullptr && sibling->right->color == RED) // RR型
                        {
                            Node *red_son = sibling->right;
                            red_son->color = sibling->color;
                            sibling->color = father->color;
                            father->color = BLACK;
                            left_rotate(father); // r变s,s变p,p变黑,左旋
                        }
                        else if (is_right(sibling) && sibling->left != nullptr && sibling->left->color == RED) // RL型
                        {
                            Node *red_son = sibling->left;
                            red_son->color = father->color;
                            father->color = BLACK;
                            right_rotate(sibling);
                            left_rotate(father); // r变p,p变黑,右旋右孩子,左旋
                        }
                    }
                }
                else // 兄弟是红色
                {
                    change_color(sibling);
                    change_color(cur->parent); // 兄父变色
                    if (is_left(cur))
                        left_rotate(cur->parent);
                    else
                        right_rotate(cur->parent); // 父亲旋转
                    erase_fix(cur);                // 继续调整
                }
            }
            void _delete(Node *cur)
            {
                if (cur->left == nullptr && cur->right == nullptr) // 没有孩子
                {
                    if (cur == root)
                    {
                        delete root;
                        root = nullptr;
                        return;
                    }
                    if (cur->color == RED) // 删除红色节点无需调整
                    {
                        if (cur->parent != nullptr)
                        {
                            if (is_left(cur))
                                cur->parent->left = nullptr;
                            else
                                cur->parent->right = nullptr;
                        }
                        delete cur;
                        cur = nullptr;
                        return;
                    }
                    else
                    {
                        erase_fix(cur); // 定义当前节点为 双黑节点（欠一个黑色）， 进行调整
                    }
                }
                else if (cur->left == nullptr) // 只有右孩子          //总是一黑一红这种情况
                {
                    Node *replace = cur->right;
                    replace->parent = cur->parent;
                    if (replace->parent == nullptr) // 说明replace 成为根节点
                    {
                        root = replace;
                    }
                    else
                    {
                        if (is_left(cur))
                            cur->parent->left = replace;
                        else if (is_right(cur))
                            cur->parent->right = replace;
                    }
                    replace->color = BLACK; // 改变颜色 使得满足 黑路同
                    delete cur;
                    cur = nullptr;
                }
                else if (cur->right == nullptr) // 只有左孩子
                {
                    Node *replace = cur->left;
                    replace->parent = cur->parent;
                    if (replace->parent == nullptr) // 说明replace 成为根节点
                    {
                        root = replace;
                    }
                    else
                    {
                        if (is_left(cur))
                            cur->parent->left = replace;
                        else if (is_right(cur))
                            cur->parent->right = replace;
                    }
                    replace->color = BLACK; // 改变颜色 使得满足 黑路同
                    delete cur;
                    cur = nullptr;
                }
                else // 两个孩子都有   这里寻找后继节点
                {
                    Node *post_node = find_min(cur->right); // 找到后继节点
                    // 这里注意不能直接替换数据这么简单， 必须要交换node位置, 否则会使之前获取的指向后继节点的迭代器失效
                    swap_node(cur, post_node);

                    // cur -> element = post_node -> element;
                    _delete(cur); // 转化成前两种情况
                }
            }
            void erase(const Key &key)
            {
                Node *cur = search(root, key);
                if (cur == nullptr)
                {
                    throw index_out_of_bound();
                    // return;
                }
                else
                    _delete(cur);
                if (siz > 0)
                    siz--;
            }
            /*
            void print(Node* cur)  //用于测试
            {
                if(cur == nullptr)return;
                if(cur -> parent != nullptr)std::cout << "我是普通节点" << cur -> element.value.first << "，颜色是";
                else std::cout << "我是根节点" << cur -> element.value.first << "，颜色是";
                if(cur -> color == RED)std::cout << "红色,";
                else std::cout << "黑色,";
                if(cur -> parent == nullptr)
                {
                    std::cout << "父亲是空节点";
                }
                else
                {
                    std::cout << "父亲是节点" << cur -> parent -> element.value.first;
                }
                std::cout << ',';
                if(cur -> left == nullptr)
                {
                    std::cout << "左孩子是空节点";
                }
                else
                {
                    std::cout << "左孩子是节点" << cur -> left -> element.value.first;
                }
                std::cout << ',';
                if(cur -> right == nullptr)
                {
                    std::cout << "右孩子是空节点";
                }
                else
                {
                    std::cout << "右孩子是节点" << cur -> right -> element.value.first;
                }
                std::cout<< '\n';
                print(cur -> left);
                if(cur -> left)std::cout << '\n';
                print(cur -> right);
                if(cur -> right)std::cout << '\n';
            }*/
        };

    private:
        RedBlackTree *rbt;

    public:
        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = map.begin(); --it;
         *       or it = map.end(); ++end();
         */
        using Node = typename RedBlackTree::Node;

        class iterator
        {
        private:
            friend class map;
            friend class const_iterator;
            Node *node;
            RedBlackTree *rbt_ptr;

        public:
            explicit iterator(Node *nod = nullptr, RedBlackTree *tree = nullptr) : node(nod), rbt_ptr(tree)
            {
            }
            iterator(const iterator &other)
            {
                node = other.node;
                rbt_ptr = other.rbt_ptr;
            }
            /**
             * TODO iter++
             */
            iterator operator++(int)
            {
                if (node == nullptr)
                {
                    throw invalid_iterator();
                }
                iterator origin(*this);
                node = rbt_ptr->find_least_bigger(node);
                return origin;
            }
            /**
             * TODO ++iter
             */
            iterator &operator++()
            {
                if (node == nullptr)
                {
                    throw invalid_iterator();
                }
                node = rbt_ptr->find_least_bigger(node);
                return *this;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int)
            {
                if (node == rbt_ptr->find_min(rbt_ptr->root))
                {
                    throw invalid_iterator();
                }
                iterator origin(*this);
                if (node == nullptr)
                {
                    node = rbt_ptr->find_max(rbt_ptr->root);
                    return origin;
                }
                node = rbt_ptr->find_least_smaller(node);
                return origin;
            }
            /**
             * TODO --iter
             */
            iterator &operator--()
            {
                if (node == rbt_ptr->find_min(rbt_ptr->root))
                {
                    throw invalid_iterator();
                }
                if (node == nullptr)
                {
                    node = rbt_ptr->find_max(rbt_ptr->root);
                    return *this;
                }
                node = rbt_ptr->find_least_smaller(node);
                return *this;
            }

            value_type &operator*() const
            {
                if (node == nullptr)
                {
                    throw invalid_iterator();
                }
                return node->element.value;
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const iterator &rhs) const
            {
                return node == rhs.node && rbt_ptr == rhs.rbt_ptr;
            }
            bool operator==(const const_iterator &rhs) const
            {
                return node == rhs.node && rbt_ptr == rhs.rbt_ptr;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const
            {
                return node != rhs.node || rbt_ptr != rhs.rbt_ptr;
            }
            bool operator!=(const const_iterator &rhs) const
            {
                return node != rhs.node || rbt_ptr != rhs.rbt_ptr;
            }

            value_type *operator->() const noexcept
            {
                return &(*(*this));
            }
        };
        class const_iterator
        {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
        private:
            friend class map;
            friend class const_iterator;
            const Node *node;
            RedBlackTree *rbt_ptr;

        public:
            explicit const_iterator(Node *nod = nullptr, RedBlackTree *tree = nullptr) : node(nod), rbt_ptr(tree)
            {
            }
            const_iterator(const const_iterator &other)
            {
                node = other.node;
                rbt_ptr = other.rbt_ptr;
            }
            const_iterator(const iterator &other)
            {
                node = other.node;
                rbt_ptr = other.rbt_ptr;
            }
            /**
             * TODO iter++
             */
            const_iterator operator++(int)
            {
                if (node == nullptr)
                {
                    throw invalid_iterator();
                }
                const_iterator origin(*this);
                if (node->right != nullptr)
                {
                    node = rbt_ptr->find_min(node->right); // 直接找右子树中最小的
                    return origin;
                }
                // 右子树为空，尝试向上找
                if (node->parent == nullptr)
                {
                    node = nullptr; // 根节点而且没有右孩子，直接返回
                    return origin;
                }
                if (rbt_ptr->is_left(node))
                    node = node->parent; // 如果是左孩子则父节点即为所求
                else
                {
                    while (rbt_ptr->is_right(node)) // 否则找到第一个转折点
                    {
                        node = node->parent;
                    }
                    node = node->parent;
                }
                return origin;
            }
            /**
             * TODO ++iter
             */
            const_iterator &operator++()
            {
                if (node == nullptr)
                {
                    throw invalid_iterator();
                }
                if (node->right != nullptr)
                {
                    node = rbt_ptr->find_min(node->right); // 直接找右子树中最小的
                    return *this;
                }
                // 右子树为空，尝试向上找
                if (node->parent == nullptr)
                {
                    node = nullptr; // 根节点而且没有右孩子，直接返回
                    return *this;
                }
                if (rbt_ptr->is_left(node))
                    node = node->parent; // 如果是左孩子则父节点即为所求
                else
                {
                    while (rbt_ptr->is_right(node)) // 否则找到第一个转折点
                    {
                        node = node->parent;
                    }
                    node = node->parent;
                }
                return *this;
            }
            /**
             * TODO iter--
             */
            const_iterator operator--(int)
            {
                if (node == rbt_ptr->find_min(rbt_ptr->root))
                {
                    throw invalid_iterator();
                }
                const_iterator origin(*this);
                if (node == nullptr)
                {
                    node = rbt_ptr->find_max(rbt_ptr->root);
                    return origin;
                }
                if (node->left != nullptr)
                {
                    node = rbt_ptr->find_max(node->left); // 直接找左子树中最大的
                    return origin;
                }
                // 左子树为空，尝试向上找
                if (node->parent == nullptr)
                {
                    node = nullptr; // 根节点而且没有右孩子，直接返回
                    return origin;
                }
                if (rbt_ptr->is_right(node))
                    node = node->parent; // 如果是右孩子则父节点即为所求
                else
                {
                    while (rbt_ptr->is_left(node)) // 否则找到第一个转折点
                    {
                        node = node->parent;
                    }
                    node = node->parent;
                }
                return origin;
            }
            /**
             * TODO --iter
             */
            const_iterator &operator--()
            {
                if (node == rbt_ptr->find_min(rbt_ptr->root))
                {
                    throw invalid_iterator();
                }
                if (node == nullptr)
                {
                    node = rbt_ptr->find_max(rbt_ptr->root);
                    return *this;
                }
                if (node->left != nullptr)
                {
                    node = rbt_ptr->find_max(node->left); // 直接找左子树中最大的
                    return *this;
                }
                // 左子树为空，尝试向上找
                if (node->parent == nullptr)
                {
                    node = nullptr; // 根节点而且没有右孩子，直接返回
                    return *this;
                }
                if (rbt_ptr->is_right(node))
                    node = node->parent; // 如果是右孩子则父节点即为所求
                else
                {
                    while (rbt_ptr->is_left(node)) // 否则找到第一个转折点
                    {
                        node = node->parent;
                    }
                    node = node->parent;
                }
                return *this;
            }

            const value_type &operator*() const
            {
                if (node == nullptr)
                {
                    throw invalid_iterator();
                }
                return node->element.value;
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const iterator &rhs) const
            {
                return node == rhs.node && rbt_ptr == rhs.rbt_ptr;
            }
            bool operator==(const const_iterator &rhs) const
            {
                return node == rhs.node && rbt_ptr == rhs.rbt_ptr;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const
            {
                return node != rhs.node || rbt_ptr != rhs.rbt_ptr;
            }
            bool operator!=(const const_iterator &rhs) const
            {
                return node != rhs.node || rbt_ptr != rhs.rbt_ptr;
            }

            const value_type *operator->() const noexcept
            {
                return &*(*this);
            }
        };
        /**
         * TODO two constructors
         */
        map()
        {
            rbt = new RedBlackTree();
        }
        map(const map &other)
        {
            rbt = new RedBlackTree(*other.rbt);
        }
        /**
         * TODO assignment operator
         */
        map &operator=(const map &other)
        {
            if (this == &other)
                return *this;
            rbt->clear(rbt->root);
            delete rbt;
            rbt = new RedBlackTree(*other.rbt);
            return *this;
        }
        /**
         * TODO Destructors
         */
        ~map()
        {
            rbt->clear(rbt->root);
            delete rbt;
            rbt = nullptr;
        }
        /**
         * TODO
         * access specified element with bounds checking
         * Returns a reference to the mapped value of the element with key equivalent to key.
         * If no such element exists, an exception of type `index_out_of_bound'
         */
        T &at(const Key &key)
        {
            return rbt->find(key);
        }
        const T &at(const Key &key) const
        {
            return rbt->find(key);
        }
        /**
         * TODO
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T &operator[](const Key &key)
        {
            return insert({key, T()}).first.node->element.value.second;
        }
        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T &operator[](const Key &key) const
        {
            return rbt->find(key);
        }
        /**
         * return a iterator to the beginning
         */
        iterator begin()
        {
            Node *begin = rbt->find_min(rbt->root);
            return iterator(begin, rbt);
        }
        const_iterator cbegin() const
        {
            Node *begin = rbt->find_min(rbt->root);
            return const_iterator(begin, rbt);
        }
        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end()
        {
            return iterator(nullptr, rbt);
        }
        const_iterator cend() const
        {
            return const_iterator(nullptr, rbt);
        }
        /**
         * checks whether the container is empty
         * return true if empty, otherwise false.
         */
        bool empty() const
        {
            return rbt->siz == 0;
        }
        /**
         * returns the number of elements.
         */
        size_t size() const
        {
            return rbt->siz;
        }
        /**
         * clears the contents
         */
        void clear()
        {
            rbt->clear(rbt->root);
            if (rbt->root != nullptr)
                rbt->root = nullptr;
        }
        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        pair<iterator, bool> insert(const value_type &value)
        {
            typename RedBlackTree::_value_type val(value.first, value.second);
            pair<Node *, bool> res = rbt->insert(val);
            iterator it(res.first, rbt);
            return {it, res.second};
        }
        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos)
        {
            if (pos.node == nullptr || pos.rbt_ptr != rbt)
            {
                throw invalid_iterator();
            }
            rbt->erase(pos.node->element.value.first);
        }
        /*
        void print()
        {
            std::cout << "打印开始\n";
            rbt -> print (rbt -> root);
            std::cout << "打印完成\n";
        }*/
        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const Key &key) const
        {
            Node *res = rbt->search(rbt->root, key);
            if (res != nullptr)
                return 1;
            return 0;
        }
        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const Key &key)
        {
            Node *res = rbt->search(rbt->root, key);
            return iterator(res, rbt);
        }
        const_iterator find(const Key &key) const
        {
            Node *res = rbt->search(rbt->root, key);
            return const_iterator(res, rbt);
        }
    };

} // namespace sjtu

#endif
