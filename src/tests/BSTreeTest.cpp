#include "../BSTree.h"
#include "gtest/gtest.h"

#include <iostream>
#include <cstring>
#include <random>

using namespace yak;

void print (BSNode<int> *node)
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
using TreeWalker = void (*)(BSNode<T> *, FuncNode<T>);

void print_tree (BSNode<int> *node, TreeWalker<int> walker, const char *comment)
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
    BSNode<int> t(1);
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
        tree = new BSTree<int>( new BSNode<int>(*(es.begin())) );

        for (std::vector<int>::iterator iter = es.begin() + 1; iter != es.end(); iter++)
        {
            BSNode<int> *node = new BSNode<int>(*iter);
            tree->insert(node);
        }

    }

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.  Otherwise,
    // you don't have to provide it.
    //
    virtual void TearDown()
    {
        tree->clean();
    }

    BSTree<int> *tree;
    std::vector<int> numbers;
};

TEST_F(BSTreeTest, BSTreeWalker)
{
    print_tree(tree->root, BSTree<int>::inorder_tree_walker,   "Inorder   print:");
    print_tree(tree->root, BSTree<int>::preorder_tree_walker,  "Preorder  print:");
    print_tree(tree->root, BSTree<int>::postorder_tree_walker, "Postorder print:");
}

TEST_F(BSTreeTest, BSTreeSearch)
{
    for (auto n: numbers)
    {
        BSNode<int> *node = tree->search(n);
        EXPECT_EQ(n, node->key);
    }

    BSNode<int> *not_found = tree->search(size);
    EXPECT_EQ(nullptr, not_found);    
}

TEST_F(BSTreeTest, BSTreeIterSearch)
{
    for (auto n: numbers)
    {
        BSNode<int> *node = tree->iterative_search(n);
        EXPECT_EQ(n, node->key);
    }

    BSNode<int> *not_found = tree->iterative_search(size);
    EXPECT_EQ(nullptr, not_found);    
}

TEST_F(BSTreeTest, BSTreeMinMax)
{
    BSNode<int> *min = tree->minimum();
    EXPECT_EQ(0, min->key);

    BSNode<int> *max = tree->maximum();
    EXPECT_EQ(size-1, max->key);    
}

TEST_F(BSTreeTest, BSTreeSuccessor)
{
    for (auto n: numbers)
    {
        BSNode<int> *node = tree->search(n);
        BSNode<int> *suc = BSTree<int>::tree_successor(node);
        if (nullptr != suc)
            EXPECT_EQ(n+1, suc->key);
        else EXPECT_EQ(size-1, node->key);
    }
}

TEST_F(BSTreeTest, BSTreePredecessor)
{
    for (auto n: numbers)
    {
        BSNode<int> *node = tree->search(n);
        BSNode<int> *pred = BSTree<int>::tree_predecessor(node);
        if (nullptr != pred)
            EXPECT_EQ(n-1, pred->key);
        else EXPECT_EQ(0, node->key);
    }
}

TEST_F(BSTreeTest, BSTreeDelete)
{
    for (auto n: numbers)
    {
        BSNode<int> *node = tree->search(n);
        tree->del(node);

        BSNode<int> *not_found = tree->search(n);
        EXPECT_TRUE(not_found == nullptr);
    }
}
