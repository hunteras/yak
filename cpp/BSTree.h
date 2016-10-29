#ifndef BSTREE_H
#define BSTREE_H

#include <iostream>

namespace yak {
    
    template <typename T>
    class BSTree
    {
    public:
        T key;
        BSTree<T> *left;
        BSTree<T> *right;
        BSTree<T> *parent;

        BSTree(T k, BSTree *l = nullptr, BSTree *r = nullptr, BSTree *p = nullptr)
            : key(k), left(l), right(r), parent(p) {}
        
        ~BSTree()
        {} ;

        BSTree<T> *root()
        {
            BSTree<T> *t = this;
            while (nullptr != this->parent)
                t = this->parent;
            return t;
        }
        
        void insert(BSTree<T> *z)
        {
            BSTree<T> *y = nullptr;
            BSTree<T> *x = this->root();

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

        friend std::ostream& operator<<(std::ostream& out, const BSTree *node) {
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
    using FuncNode = void (*) (BSTree<T> *);

    template <typename T>
    static
    void
    inorder_tree_walker(BSTree<T> *node, FuncNode<T> fn)
    {
        if (nullptr != node)
        {
            inorder_tree_walker(node->left, fn);
            fn(node);
            inorder_tree_walker(node->right, fn);
        }
    }

    template <typename T>
    static
    void
    preorder_tree_walker(BSTree<T> *node, FuncNode<T> fn)
    {
        if (nullptr != node)
        {
            fn(node);
            preorder_tree_walker(node->left, fn);
            preorder_tree_walker(node->right, fn);
        }
    }

    template <typename T>
    static
    void
    postorder_tree_walker(BSTree<T> *node, FuncNode<T> fn)
    {
        if (nullptr != node)
        {
            postorder_tree_walker(node->left, fn);
            postorder_tree_walker(node->right, fn);
            fn(node);
        }
    }

    template <typename T>
    static
    BSTree<T> *
    tree_search(BSTree<T> *node, T k)
    {
        if (nullptr == node || k == node->key)
            return node;
        if (k < node->key)
            return tree_search(node->left, k);
        else return tree_search(node->right, k);
    }

    template <typename T>
    static
    BSTree<T> *
    iterative_tree_search(BSTree<T> *node, T k)
    {
        BSTree<T> *iter = node;
        while (nullptr != iter && k != iter->key)
        {
            if (k < iter->key)
                iter = iter->left;
            else iter = iter->right;
        }
        return iter;
    }
    
    template <typename T>
    static
    BSTree<T> *
    tree_minimum(BSTree<T> *node)
    {
        BSTree<T> *iter = node;
        while (nullptr != iter->left)
            iter = iter->left;
        return iter;
    }

    template <typename T>
    static
    BSTree<T> *
    tree_maximum(BSTree<T> *node)
    {
        BSTree<T> *iter = node;
        while (nullptr != iter->right)
            iter = iter->right;
        return iter;
    }

    template <typename T>
    static
    BSTree<T> *
    tree_successor(BSTree<T> *node)
    {
        BSTree<T> *x = node;
        if (nullptr != x->right)
            return tree_minimum(x->right);
        BSTree<T> *y = x->parent;
        while (nullptr != y && x == y->right)
        {
            x = y;
            y = y->parent;
        }
        return y;
    }

    template <typename T>
    static
    BSTree<T> *
    tree_predecessor(BSTree<T> *node)
    {
        BSTree<T> *x = node;
        if (nullptr != x->left)
            return tree_maximum(x->left);
        BSTree<T> *y = x->parent;
        while (nullptr != y && x == y->left)
        {
            x = y;
            y = y->parent;
        }
        return y;
    }

    // template <typename T>
    // static
    // void
    // transplant(BSTree<T> *u, BSTree<T> *v)
    // {
        
    // }
    

}
#endif
