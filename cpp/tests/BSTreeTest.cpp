#include "../BSTree.h"
#include "gtest/gtest.h"

#include <iostream>
#include <cstring>
#include <random>

using namespace yak;

void print (BSTree<int> *node)
{
    std::cout << node;
}

std::vector<int> generate_random_vector(int size)
{
    std::vector<int> numbers(size);
    
    int n = {0};
    std::generate(numbers.begin(), numbers.end(), [&n]{ return n++; });

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, size - 1);

    for (int n = 0; n < size; ++n)
    {
        std::iter_swap(numbers.begin() + dis(gen), numbers.begin() + dis(gen));
    }
    
    return numbers;
}

template <typename T>
using TreeWalker = void (*)(BSTree<T> *, FuncNode<T>);

void print_tree (BSTree<int> *node, TreeWalker<int> walker, const char *comment)
{
    std::cout << comment << std::endl;
    walker(node, print);
    std::cout << std::endl;
}

template <typename T>
void print_vector (std::vector<T> &v, const char *comment)
{
    std::cout << comment << " vector with size " << v.size() << " :" << std::endl;
    for (auto item : v) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

TEST(BSTree, BSTreeInit)
{
    BSTree<int> t(1);
    EXPECT_EQ(1, t.key);
}




class BSTreeTest : public testing::Test {
protected: 
    static const int size = 10;
    
    virtual void SetUp()
    {
        numbers.resize(size);
    
        int n = {0};
        std::generate(numbers.begin(), numbers.end(), [&n]{ return n++; });

        std::vector<int> es = generate_random_vector(size);

        print_vector<int>(es, "random number");
        tree = new BSTree<int>(*(es.begin()));

        for (std::vector<int>::iterator iter = es.begin() + 1; iter != es.end(); iter++)
        {
            BSTree<int> *node = new BSTree<int>(*iter);
            tree->insert(node);
        }

    }

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.  Otherwise,
    // you don't have to provide it.
    //
    // virtual void TearDown() {
    // }

    BSTree<int> *tree;
    std::vector<int> numbers;
};

TEST_F(BSTreeTest, BSTreeWalker)
{
    print_tree(tree, inorder_tree_walker,   "Inorder   print:");
    print_tree(tree, preorder_tree_walker,  "Preorder  print:");
    print_tree(tree, postorder_tree_walker, "Postorder print:");
}

TEST_F(BSTreeTest, BSTreeSearch)
{
    for (auto n: numbers)
    {
        BSTree<int> *node = tree_search(tree, n);
        EXPECT_EQ(n, node->key);
    }

    BSTree<int> *not_found = tree_search(tree, size);
    EXPECT_EQ(nullptr, not_found);    
}

TEST_F(BSTreeTest, BSTreeIterSearch)
{
    for (auto n: numbers)
    {
        BSTree<int> *node = iterative_tree_search(tree, n);
        EXPECT_EQ(n, node->key);
    }

    BSTree<int> *not_found = iterative_tree_search(tree, size);
    EXPECT_EQ(nullptr, not_found);    
}

TEST_F(BSTreeTest, BSTreeMinMax)
{
    BSTree<int> *min = tree_minimum(tree);
    EXPECT_EQ(0, min->key);

    BSTree<int> *max = tree_maximum(tree);
    EXPECT_EQ(size-1, max->key);    
}

TEST_F(BSTreeTest, BSTreeSuccessor)
{
    for (auto n: numbers)
    {
        BSTree<int> *node = tree_search(tree, n);
        BSTree<int> *suc = tree_successor(node);
        if (nullptr != suc)
            EXPECT_EQ(n+1, suc->key);
        else EXPECT_EQ(size-1, node->key);
    }
}

TEST_F(BSTreeTest, BSTreePredecessor)
{
    for (auto n: numbers)
    {
        BSTree<int> *node = tree_search(tree, n);
        BSTree<int> *pred = tree_predecessor(node);
        if (nullptr != pred)
            EXPECT_EQ(n-1, pred->key);
        else EXPECT_EQ(0, node->key);
    }
}
