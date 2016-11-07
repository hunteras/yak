#include "../Regex.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace yak;


TEST(Regex, RegexNfa)
{
    std::string post = re_to_post("(a|b)c");
    std::cout << post << std::endl;
    Nfa *nfa = post_to_nfa(post);
    //std::cout << *(nfa->start) << std::endl;
    //nfa->walk();
    std::cout << nfa->match("ac") << std::endl;;
    //std::cout << nfa->match("ac") << std::endl;
}

TEST(Regex, RegexToPost)
{
    EXPECT_EQ(re_to_post("d(ab)|c"), "dab..c|");
    EXPECT_EQ(re_to_post("d((ab)|c)"), "dab.c|.");
}

