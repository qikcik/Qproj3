#include "gtest/gtest.h"
#include "json/object.hpp"

using namespace json;

TEST(EntryTest, EntryTest)
{
    Object e;
    e = Object{ {
        {"a",Value{"b"}},
        {"b",Value{"32"}},
        {"c",Object{{
            {"a",Value{"b"}},
            {"d",Value{"32"}},
            {"v",Array{{
                Object{{{"i", Value{"0"}}}},
                Object{{{"i", Value{"1"}}}},
                }}},
            {"v2",Array{{
                Value{"test"}, Value{"test1"}, Value{"test3"}
                }}}
            }}
        },
    } };

    EXPECT_EQ(*e.get<Value>("a")->get<string>(), "b");
    EXPECT_EQ(*e.get<Value>("b")->get<string>(), "32");
    EXPECT_EQ(*e.get<Object>("c")->get<Value>("a")->get<string>(), "b");
    EXPECT_EQ(*e.get<Object>("c")->get<Value>("d")->get<string>(), "32");
    EXPECT_EQ(*e.get<Object>("c")->get<Array>("v")->get<Object>(0)->get<Value>("i")->get<string>(), "0");
    EXPECT_EQ(*e.get<Object>("c")->get<Array>("v")->get<Object>(1)->get<Value>("i")->get<string>(), "1");
    EXPECT_EQ(*e.get<Object>("c")->get<Array>("v2")->get<Value>(1)->get<string>(), "test1");

    //std::cout << e.stringify() << std::endl;
}