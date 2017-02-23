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

#include "rs_protocol_rtmp.h"
#include "rs_kernel_buffer.h"
#include "gtest/gtest.h"

using namespace std;

TEST(RTMP_CHUNK_MESSAGE, dump)
{
    RsRtmpChunkMessage msg;
    msg.fmt = 0;
    msg.cs_id = 3;
    msg.timestamp = 1024;
    msg.timestamp_delta = 128;
    msg.chunk_size = 1024;
    msg.message_length = 512;
    msg.message_type_id = 9;
    msg.message_stream_id = 2048;
    msg.chunk_data = rs_get_random(512);

    string buf;
    // type 0
    ASSERT_EQ(ERROR_SUCCESS, msg.dump(buf));
    ASSERT_EQ(buf.length(), 512 + 11 + 1);

    // type 1
    buf.clear();
    msg.fmt = 1;
    ASSERT_EQ(ERROR_SUCCESS, msg.dump(buf));
    ASSERT_EQ(buf.length(), 512 + 1 + 7);

    // type 2
    buf.clear();
    msg.fmt = 2;
    ASSERT_EQ(ERROR_SUCCESS, msg.dump(buf));
    ASSERT_EQ(buf.length(), 512 + 1 + 3);

    // type 3
    buf.clear();
    msg.fmt = 3;
    ASSERT_EQ(ERROR_SUCCESS, msg.dump(buf));
    ASSERT_EQ(buf.length(), 512 + 1);
}

TEST(RTMP_CHUNK_MESSAGE, initialize)
{
    RsRtmpChunkMessage msg;
    msg.fmt = 0;
    msg.cs_id = 3;
    msg.timestamp = 1024;
    msg.timestamp_delta = 128;
    msg.chunk_size = 1024;
    msg.message_length = 512;
    msg.message_type_id = 9;
    msg.message_stream_id = 2048;
    msg.chunk_data = rs_get_random(512);

    {
        string buf;
        RsBufferLittleEndian rs_buf;
        ASSERT_EQ(ERROR_SUCCESS, msg.dump(buf));
        rs_buf.write_bytes(buf);

        RsRtmpChunkMessage test_msg;
        ASSERT_EQ(ERROR_SUCCESS, test_msg.initialize(&rs_buf, 1024));
        ASSERT_EQ(msg.fmt, test_msg.fmt);
        ASSERT_EQ(msg.cs_id, test_msg.cs_id);
        ASSERT_EQ(msg.timestamp, test_msg.timestamp);
        ASSERT_EQ(msg.message_length, test_msg.message_length);
        ASSERT_EQ(msg.message_type_id, test_msg.message_type_id);
        ASSERT_EQ(msg.message_stream_id, test_msg.message_stream_id);
        ASSERT_STREQ(msg.chunk_data.c_str(), test_msg.chunk_data.c_str());
    }

    {
        string buf;
        RsBufferLittleEndian rs_buf;
        msg.fmt = 1;
        ASSERT_EQ(ERROR_SUCCESS, msg.dump(buf));
        rs_buf.write_bytes(buf);

        RsRtmpChunkMessage test_msg;
        ASSERT_EQ(ERROR_SUCCESS, test_msg.initialize(&rs_buf, 1024));
        ASSERT_EQ(msg.fmt, test_msg.fmt);
        ASSERT_EQ(msg.cs_id, test_msg.cs_id);
        ASSERT_EQ(msg.timestamp_delta, test_msg.timestamp_delta);
        ASSERT_EQ(msg.message_length, test_msg.message_length);
        ASSERT_EQ(msg.message_type_id, test_msg.message_type_id);
        ASSERT_STREQ(msg.chunk_data.c_str(), test_msg.chunk_data.c_str());
    }

    {
        string buf;
        RsBufferLittleEndian rs_buf;
        msg.fmt = 2;
        ASSERT_EQ(ERROR_SUCCESS, msg.dump(buf));
        rs_buf.write_bytes(buf);

        RsRtmpChunkMessage test_msg;
        ASSERT_EQ(ERROR_SUCCESS, test_msg.initialize(&rs_buf, 1024, 512));
        ASSERT_EQ(msg.fmt, test_msg.fmt);
        ASSERT_EQ(msg.cs_id, test_msg.cs_id);
        ASSERT_EQ(msg.timestamp_delta, test_msg.timestamp_delta);
        ASSERT_STREQ(msg.chunk_data.c_str(), test_msg.chunk_data.c_str());
    }

    {
        string buf;
        RsBufferLittleEndian rs_buf;
        msg.fmt = 3;
        ASSERT_EQ(ERROR_SUCCESS, msg.dump(buf));
        rs_buf.write_bytes(buf);

        RsRtmpChunkMessage test_msg;
        ASSERT_EQ(ERROR_SUCCESS, test_msg.initialize(&rs_buf, 1024, 512));
        ASSERT_EQ(msg.fmt, test_msg.fmt);
        ASSERT_EQ(msg.cs_id, test_msg.cs_id);
        ASSERT_STREQ(msg.chunk_data.c_str(), test_msg.chunk_data.c_str());
    }
}

TEST(RTMP_CHUNK_MESSAGE, create_msgs)
{
    {
        string buf = rs_get_random(1024);
        auto test = RsRtmpChunkMessage::create_chunk_messages(1024, buf, 1, 1, 512);
        ASSERT_EQ(2, test.size());
    }

    {
        string buf = rs_get_random(2048);
        auto test = RsRtmpChunkMessage::create_chunk_messages(1024, buf, 1, 1, 512);
        ASSERT_EQ(4, test.size());
        test.clear();
    }
}