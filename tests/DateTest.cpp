#include "../Date.h"
#include "gtest/gtest.h"

#include <iostream>
#include <cstring>

using namespace yak;
using namespace std;

TEST(Date, Usage)
{
    Date d;
    cout << d.toString() << endl;

    d = Date::Now();
    cout << d.toString() << endl;
}


