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
#include "rs_kernel_buffer.h"

TEST(RsBuffer, read_write)
{
    RsBufferLittleEndian buffer;

    {
        uint8_t val = 0;
        ASSERT_EQ(buffer.write_1_byte(val), ERROR_SUCCESS);
        ASSERT_EQ(buffer.read_1_byte(), val);

        val = 128;
        ASSERT_EQ(buffer.write_1_byte(val), ERROR_SUCCESS);
        ASSERT_EQ(buffer.read_1_byte(), val);

        val = 255;
        ASSERT_EQ(buffer.write_1_byte(val), ERROR_SUCCESS);
        ASSERT_EQ(buffer.read_1_byte(), val);

    }

    {
        uint16_t val = 0;
        ASSERT_EQ(buffer.write_2_byte(val), ERROR_SUCCESS);
        ASSERT_EQ(buffer.read_2_byte(), val);

        val = 32800;
        ASSERT_EQ(buffer.write_2_byte(val), ERROR_SUCCESS);
        ASSERT_EQ(buffer.read_2_byte(), val);

        val = 65535;
        ASSERT_EQ(buffer.write_2_byte(val), ERROR_SUCCESS);
        ASSERT_EQ(buffer.read_2_byte(), val);

    }

    {
        uint32_t val = 0;
        ASSERT_EQ(buffer.write_3_byte(val), ERROR_SUCCESS);
        ASSERT_EQ(buffer.read_3_byte(), val & 0x00FFFFFF);

        ASSERT_EQ(buffer.write_4_byte(val), ERROR_SUCCESS);
        ASSERT_EQ(buffer.read_4_byte(), val);

        val = 2200000000;
        ASSERT_EQ(buffer.write_3_byte(val), ERROR_SUCCESS);
        ASSERT_EQ(buffer.read_3_byte(), val & 0x00FFFFFF);

        ASSERT_EQ(buffer.write_4_byte(val), ERROR_SUCCESS);
        ASSERT_EQ(buffer.read_4_byte(), val);

        val = 4294967295;
        ASSERT_EQ(buffer.write_3_byte(val), ERROR_SUCCESS);
        ASSERT_EQ(buffer.read_3_byte(), val & 0x00FFFFFF);

        ASSERT_EQ(buffer.write_4_byte(val), ERROR_SUCCESS);
        ASSERT_EQ(buffer.read_4_byte(), val);
    }

    {
        uint64_t val = 0;
        ASSERT_EQ(buffer.write_8_byte(val), ERROR_SUCCESS);
        ASSERT_EQ(buffer.read_8_byte(), val);

        val = 950000000000000000;
        ASSERT_EQ(buffer.write_8_byte(val), ERROR_SUCCESS);
        ASSERT_EQ(buffer.read_8_byte(), val);

        val = 1844674407370955161;
        ASSERT_EQ(buffer.write_8_byte(val), ERROR_SUCCESS);
        ASSERT_EQ(buffer.read_8_byte(), val);
    }

    {
        std::string strTest = "hello rtmp server utest";
        ASSERT_EQ(buffer.write_bytes((char*)strTest.c_str(), strTest.size()), ERROR_SUCCESS);
        std::string strRead = buffer.read_bytes(strTest.size());
        ASSERT_TRUE(strTest == strRead);
    }

}
