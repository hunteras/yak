#ifndef BSTREE_H
#define BSTREE_H

#include <iostream>

namespace yak {

    template <typename T>
    class BSNode
    {
    public:
        T key;
        BSNode<T> *left;
        BSNode<T> *right;
        BSNode<T> *parent;

        BSNode(T k, BSNode *l = nullptr, BSNode *r = nullptr, BSNode *p = nullptr)
            : key(k), left(l), right(r), parent(p) {}

        ~BSNode()
        {} ;

        BSNode<T> *root()
        {
            BSNode<T> *t = this;
            while (nullptr != this->parent)
                t = this->parent;
            return t;
        }

        void insert(BSNode<T> *z)
        {
            BSNode<T> *y = nullptr;
            BSNode<T> *x = this->root();

            while (nullptr != x)
            {
                y = x;
                if (z->key < x->key)
                    x = x->left;
                else x = x->right;
            }

            z->parent = y;
            if (z->key < y->key)
                y->left = z;
            else y->right = z;
        }

        friend std::ostream& operator<<(std::ostream& out, const BSNode *node) {
            out << "key: " << node->key;

            out << " left: ";
            if (nullptr == node->left) out << "NULL ";
            else out << node->left->key;

            out << " right: ";
            if (nullptr == node->right) out << "NULL ";
            else out << node->right->key;

            out << " parent: ";
            if (nullptr == node->parent) out << "NULL ";
            else out << node->parent->key;

            out << std::endl;

            return out;
        }

    };

    template <typename T>
    using FuncNode = void (*) (BSNode<T> *);

    template <typename T>
    class BSTree
    {
    public:
        BSNode<T> *root;

        BSTree(BSNode<T> *r = nullptr)
            : root(r) {}

        ~BSTree()
        {} ;

        void insert(BSNode<T> *node) { root->insert(node); }

        static
        void
        inorder_tree_walker(BSNode<T> *node, FuncNode<T> fn)
        {
            if (nullptr != node)
            {
                inorder_tree_walker(node->left, fn);
                fn(node);
                inorder_tree_walker(node->right, fn);
            }
        }

        static
        void
        preorder_tree_walker(BSNode<T> *node, FuncNode<T> fn)
        {
            if (nullptr != node)
            {
                fn(node);
                preorder_tree_walker(node->left, fn);
                preorder_tree_walker(node->right, fn);
            }
        }

        static
        void
        postorder_tree_walker(BSNode<T> *node, FuncNode<T> fn)
        {
            if (nullptr != node)
            {
                postorder_tree_walker(node->left, fn);
                postorder_tree_walker(node->right, fn);
                fn(node);
            }
        }

        void
        inorder_walker(FuncNode<T> fn) { inorder_tree_walker(root, fn); }

        void
        preorder_walker(FuncNode<T> fn) { preorder_tree_walker(root, fn); }

        void
        postorder_walker(FuncNode<T> fn) { postorder_tree_walker(root, fn); }

        static
        BSNode<T> *
        tree_search(BSNode<T> *node, T k)
        {
            if (nullptr == node || k == node->key)
                return node;
            if (k < node->key)
                return tree_search(node->left, k);
            else return tree_search(node->right, k);
        }

        BSNode<T> *
        search(T k) { return tree_search(root, k); }

        static
        BSNode<T> *
        iterative_tree_search(BSNode<T> *node, T k)
        {
            BSNode<T> *iter = node;
            while (nullptr != iter && k != iter->key)
            {
                if (k < iter->key)
                    iter = iter->left;
                else iter = iter->right;
            }
            return iter;
        }

        BSNode<T> *
        iterative_search(T k) { return iterative_tree_search(root, k); }

        static
        BSNode<T> *
        tree_minimum(BSNode<T> *node)
        {
            BSNode<T> *iter = node;
            while (nullptr != iter->left)
                iter = iter->left;
            return iter;
        }

        BSNode<T> *
        minimum() { return tree_minimum(root); }

        static
        BSNode<T> *
        tree_maximum(BSNode<T> *node)
        {
            BSNode<T> *iter = node;
            while (nullptr != iter->right)
                iter = iter->right;
            return iter;
        }

        BSNode<T> *
        maximum() { return tree_maximum(root); }

        static
        BSNode<T> *
        tree_successor(BSNode<T> *node)
        {
            BSNode<T> *x = node;
            if (nullptr != x->right)
                return tree_minimum(x->right);
            BSNode<T> *y = x->parent;
            while (nullptr != y && x == y->right)
            {
                x = y;
                y = y->parent;
            }
            return y;
        }

        static
        BSNode<T> *
        tree_predecessor(BSNode<T> *node)
        {
            BSNode<T> *x = node;
            if (nullptr != x->left)
                return tree_maximum(x->left);
            BSNode<T> *y = x->parent;
            while (nullptr != y && x == y->left)
            {
                x = y;
                y = y->parent;
            }
            return y;
        }

        void
        transplant(BSNode<T> *u, BSNode<T> *v)
        {
            if (nullptr == u->parent)
                root = v;
            else if (u == u->parent->left)
                u->parent->left = v;
            else u->parent->right = v;
            if (nullptr != v)
                v->parent = u->parent;
        }

        void
        del(BSNode<T> *z)
        {
            if (nullptr == z->left)
                this->transplant(z, z->right);
            else if (nullptr == z->right)
                this->transplant(z, z->left);
            else {
                BSNode<T> *y = tree_minimum(z->right);
                if (y->parent != z)
                {
                    transplant(y, y->right);
                    y->right = z->right;
                    y->right->parent = y;
                }
                transplant(z, y);
                y->left = z->left;
                y->left->parent = y;
            }
        }

        void
        clean(bool delete_node = true)
        {
            BSNode<T> *node = root;
            while (nullptr != node)
            {
                this->del(node);

                if (delete_node) {
                    delete node;
                }

                node = root;
            }
        }
    };

}
#endif
