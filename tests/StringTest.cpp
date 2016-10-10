#include "../String.h"
#include "gtest/gtest.h"

#include <iostream>
#include <cstring>

using namespace yak;
using namespace std;

const char data[] = "hello world.";

TEST(String, DefaultConstructor)
{
    const String s;

    EXPECT_STREQ("", s.c_str());

    EXPECT_EQ(0u, s.length());
}

TEST(String, ConstructorFromCString)
{
    const String s1(data);
    EXPECT_EQ(0, strcmp(s1.c_str(), data));
    EXPECT_EQ(sizeof(data)/sizeof(*data) -1, s1.length());

    const String s2(data, 3);
    EXPECT_EQ(0, strcmp(s2.c_str(), "hel"));
    EXPECT_EQ(3, s2.length());
}

TEST(String, ConstructorFromCharPointer)
{
    const char *start = data;
    size_t size = sizeof(data)/sizeof(*data) -1;
    
    const String s1(start, start+size);
    EXPECT_EQ(0, strcmp(s1.c_str(), data));
    EXPECT_EQ(size, s1.length());

    const String s2(start, start + 3);
    EXPECT_EQ(0, strcmp(s2.c_str(), "hel"));
    EXPECT_EQ(3, s2.length());
}

TEST(String, ConstructorWithFillChar)
{
    const String s(10, 's');
    EXPECT_EQ(0, strcmp(s.c_str(), "ssssssssss"));
    EXPECT_EQ(10, s.length());
}

TEST(String, CopyConstructor)
{
    const String s1(data);
    const String s2 = s1;
    EXPECT_EQ(0, strcmp(s2.c_str(), data));
}

TEST(String, MoveConstructor)
{
    const char *rstr = "rvalue string";
    String s1(rstr);
    String s(std::move(s1));
    EXPECT_EQ(0, strcmp(s.c_str(), rstr));
    EXPECT_EQ(0, strcmp(s1.c_str(), ""));
}

TEST(String, Assign)
{
    String s;
    s.assign(data);
    EXPECT_EQ(0, strcmp(s.c_str(), data));
    s.assign(data, 3);
    EXPECT_EQ(0, strcmp(s.c_str(), "hel"));
    s.assign(NULL);
    EXPECT_EQ(0, strcmp(s.c_str(), ""));
}

const char *indexData = "Hello, World.";

TEST(String, LastIndexOf)
{
    String s(indexData);
    EXPECT_EQ(10, s.lastIndexOf('l'));
    EXPECT_EQ(-1, s.lastIndexOf('w'));
    EXPECT_EQ(7, s.lastIndexOf('w', std::string::npos, yak::String::CaseInsensitive));
}

TEST(String, IndexOf)
{
    String s(indexData);
    EXPECT_EQ(1, s.indexOf('e'));
    EXPECT_EQ(-1, s.indexOf('w'));
    EXPECT_EQ(7, s.indexOf('w', 0, yak::String::CaseInsensitive));
    EXPECT_EQ(-1, s.indexOf('w', 8, yak::String::CaseInsensitive));
}

TEST(String, LastIndexOfCString)
{
    String s(indexData);
    EXPECT_EQ(8, s.lastIndexOf("orl", 3));
    EXPECT_EQ(-1, s.lastIndexOf("wor", 3));
    EXPECT_EQ(7, s.lastIndexOf("wor", 3, yak::String::npos, yak::String::CaseInsensitive));
}

TEST(String, IndexOfCString)
{
    String s(indexData);
    EXPECT_EQ(8, s.indexOf("orl", 3));
    EXPECT_EQ(-1, s.indexOf("wor", 3));
    EXPECT_EQ(7, s.indexOf("wor", 3, 0, yak::String::CaseInsensitive));
    EXPECT_EQ(7, s.indexOf("wor", 3, 5, yak::String::CaseInsensitive));
}

TEST(String, Chomp)
{
    String s(indexData);
    s.chomp("World.");
    EXPECT_EQ(0, strcmp(s.c_str(), "Hello, "));

    String s1(indexData);
    s1.chomp("world.");
    EXPECT_EQ(0, strcmp(s1.c_str(), "Hello, W"));

    String s2(indexData);
    s2.chomp("world");
    EXPECT_EQ(0, strcmp(s2.c_str(), "Hello, W"));
}

TEST(String, Trimmed)
{
    String s(" Hello  ");
    String s1 = s.trimmed();

    EXPECT_EQ(0, strcmp(s.c_str(), " Hello  "));
    EXPECT_EQ(0, strcmp(s1.c_str(), "Hello"));
}

TEST(String, Format)
{
    String s = String::format("%s", indexData);

    EXPECT_EQ(0, strcmp(s.c_str(), indexData));
}

TEST(String, Operators)
{
    String sf(indexData);

    String s = sf;
    EXPECT_EQ(0, strcmp(s.c_str(), indexData));

    EXPECT_EQ(s[0], *indexData);

    String tmp("content ");
    tmp += 'c';
    EXPECT_EQ(0, strcmp(tmp.c_str(), "content c"));

    tmp += " cstr";
    EXPECT_EQ(0, strcmp(tmp.c_str(), "content c cstr"));

    tmp += String(" String");
    EXPECT_EQ(0, strcmp(tmp.c_str(), "content c cstr String"));

    EXPECT_TRUE(s == String(indexData));
    EXPECT_TRUE(s == indexData);

    EXPECT_FALSE(s != String(indexData));
    EXPECT_FALSE(s != indexData);

    String cmp = "Hello";
    EXPECT_TRUE(cmp < String("hello"));
    EXPECT_TRUE(cmp > String("HEllo"));

    EXPECT_TRUE("Hello" == String("Hello"));
    EXPECT_TRUE("Hello" != String("hello"));

    EXPECT_TRUE("content cstr" == (String("content ") + "cstr"));
    EXPECT_TRUE("content cstr" == ("content " + String("cstr")));
    EXPECT_TRUE("content c" == (String("content ") + 'c'));
    EXPECT_TRUE("c content" == ('c' + String(" content")));
    EXPECT_TRUE("c1 c2" == (String("c1") + String(" c2")));
}

