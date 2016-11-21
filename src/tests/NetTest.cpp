#include "../Net/Utils.h"
#include "../Net/Sock.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace yak;

TEST(Net, NetUtilsTest)
{
    std::cout << byte_order() << std::endl;
}

TEST(Net, NetSockTest)
{
    //std::cout << byte_order() << std::endl;
}
