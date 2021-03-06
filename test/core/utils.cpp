/******************************************************************************
 *                                                                            *
 * Copyright 2018 Jan Henrik Weinstock                                        *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 *     http://www.apache.org/licenses/LICENSE-2.0                             *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 *                                                                            *
 ******************************************************************************/

#include <gtest/gtest.h>
#include "vcml.h"

TEST(utils, mkstr) {
    EXPECT_EQ(vcml::mkstr("%d %s", 42, "fortytwo"), "42 fortytwo");
    EXPECT_EQ(vcml::mkstr("%.9f", 1.987654321), "1.987654321");
}

TEST(utils, dirname) {
    EXPECT_EQ(vcml::dirname("/a/b/c.txt"), "/a/b");
    EXPECT_EQ(vcml::dirname("a/b/c.txt"), "a/b");
    EXPECT_EQ(vcml::dirname("/a/b/c/"), "/a/b/c");
    EXPECT_EQ(vcml::dirname("nothing"), ".");
}

namespace N {

    template<typename T>
    struct A {
        struct  B {
            void func() {
                std::vector<std::string> bt = ::vcml::backtrace(1, 1);
                EXPECT_EQ(bt.size(), 1);
                //EXPECT_TRUE(str_begins_with(bt[0], "N::A<int>::B::func()"));
            }

            void func(T t) {
                std::vector<std::string> bt = ::vcml::backtrace(1, 1);
                EXPECT_EQ(bt.size(), 1);
                //EXPECT_TRUE(str_begins_with(bt[0], "N::A<char const*>::B::func(char const*)"));
            }

            void func2() {
                std::vector<std::string> bt = ::vcml::backtrace(1, 1);
                EXPECT_EQ(bt.size(), 1);
                //EXPECT_TRUE(str_begins_with(bt[0], "N::A<N::A<std::map<int, double, std::less<int>, std::allocator<std::pair<int const, double> > > > >::B::func2()"));
            }
        };
    };

    struct U {
        template<int N>
        void unroll(double d) {
            unroll<N-1>(d);
        }
    };

    template<>
    void U::unroll<0>(double d) {
        //const char* ref[5] = {
        //    "void N::U::unroll<0>(double)",
        //    "void N::U::unroll<1>(double)",
        //    "void N::U::unroll<2>(double)",
        //    "void N::U::unroll<3>(double)",
        //    "void N::U::unroll<4>(double)"
        //};
        std::vector<std::string> bt = ::vcml::backtrace(5, 1);
        EXPECT_EQ(bt.size(), 5);
        //for (int i = 0; i < 5; i++)
        //    EXPECT_TRUE(str_begins_with(bt[i], ref[i]));
    }
}

TEST(utils, backtrace) {
    N::A<int>::B().func();
    N::A<const char*>::B().func("42");
    N::A< N::A<std::map<int, double> > >::B().func2();
    N::U().unroll<5>(42.0);
}

TEST(utils, memswap) {
    vcml::u8 x8 = 0x11;
    vcml::memswap(&x8, sizeof(x8));
    EXPECT_EQ(x8, 0x11);

    vcml::u16 x16 = 0x1122;
    vcml::memswap(&x16, sizeof(x16));
    EXPECT_EQ(x16, 0x2211);

    vcml::u32 x32 = 0x11223344;
    vcml::memswap(&x32, sizeof(x32));
    EXPECT_EQ(x32, 0x44332211);

    vcml::u64 x64 = 0x1122334455667788ull;
    vcml::memswap(&x64, sizeof(x64));
    EXPECT_EQ(x64, 0x8877665544332211ull);
}

TEST(utils, split) {
    std::string s = "abc def\nghi\tjkl    :.;";
    std::vector<std::string> v = vcml::split(s, [] (unsigned char c) {
        return isspace(c);
    });

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.at(0), "abc");
    EXPECT_EQ(v.at(1), "def");
    EXPECT_EQ(v.at(2), "ghi");
    EXPECT_EQ(v.at(3), "jkl");
    EXPECT_EQ(v.at(4), ":.;");
}

TEST(utils, from_string) {
    vcml::u64 a = vcml::from_string<vcml::u64>("0xF");
    EXPECT_EQ(a, 0xf);
    vcml::u64 b = vcml::from_string<vcml::u64>("0x0000000b");
    EXPECT_EQ(b, 0xb);
    vcml::i32 c = vcml::from_string<vcml::i32>("10");
    EXPECT_EQ(c, 10);
    vcml::i32 d = vcml::from_string<vcml::i32>("-10");
    EXPECT_EQ(d, -10);
    vcml::u64 e = vcml::from_string<vcml::u64>("010");
    EXPECT_EQ(e, 8);
}

TEST(utils, replace) {
    std::string s ="replace this";
    EXPECT_EQ(vcml::replace(s, "this", "done"), 1);
    EXPECT_EQ(s, "replace done");

    std::string s2 = "$dir/file.txt";
    EXPECT_EQ(vcml::replace(s2, "$dir", "/home/user"), 1);
    EXPECT_EQ(s2, "/home/user/file.txt");
}
