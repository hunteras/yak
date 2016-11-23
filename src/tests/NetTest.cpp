#include "../Net/Utils.h"
#include "../Net/Sock.h"
#include "../Net/Error.h"
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

TEST(Net, NetErrorTest)
{
    Error::err_ret("ret error");
    //Error::err_sys("sys error");
    //Error::err_dump("dump error");
    Error::err_msg("msg error");
    //Error::err_quit("quit error");
}
