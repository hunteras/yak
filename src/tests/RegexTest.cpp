#include "../Regex.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace yak;


TEST(Regex, RegexNfa)
{
    // Nfa re("abcd");
    // //std::cout << re << std::endl;
    // re.walk();
}

TEST(Regex, RegexToPost)
{
    EXPECT_EQ(re_to_post("d(ab)|c"), "dab..c|");
    EXPECT_EQ(re_to_post("d((ab)|c)"), "dab.c|.");
}

