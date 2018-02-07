/*
MIT License

Copyright (c) 2016 ME_Kun_Han

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "gtest/gtest.h"
#include "rs_common_utility.h"

TEST(CommonUtility, rs_get_random) {
    {
        int size = 0;
        std::string rd = rs_get_random(size);
        for (auto i : rd) {
            ASSERT_TRUE((uint8_t(i) > 32 && uint8_t(i) < 128 + 32));
        }
        ASSERT_TRUE(rd.empty());
    }
    {
        int size = 100;
        std::string rd = rs_get_random(size);
        for (auto i : rd) {
            ASSERT_TRUE((uint8_t(i) > 32 && uint8_t(i) < 128 + 32));
        }
    }
}

TEST(CommonUtility, rs_get_system_time_ms) {
    int64_t now = rs_get_system_time_ms();
    ASSERT_TRUE(now > 0);
}
