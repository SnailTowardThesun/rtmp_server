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

#include "rs_kernel_buffer.h"
#include "rs_protocol_rtmp.h"

using namespace std;

#define DEFAULT_BUFFER_LENGTH 4096

c0c1::c0c1()
{

}

c0c1::~c0c1()
{

}

int c0c1::initialize()
{
    int ret =ERROR_SUCCESS;

    version = 0x03;
    timestamp = (uint32_t)rs_get_system_time_ms();
    zero = 0;
    random_data = rs_get_random(1528);
    return ret;
}

int c0c1::initialize(std::string buf)
{
    int ret = ERROR_SUCCESS;

    if (buf.size() < 1537) {
        ret = ERROR_RTMP_PROTOCOL_C0C1_LENGTH_ERROR;
        return ret;
    }

    RsBuffer buffer;
    buffer.write_bytes(buf.c_str(), (int)buf.size());

    if (buffer.read_1_byte() != 0x03) {
        ret = ERROR_RTMP_PROTOCOL_VERSION_ERROR;
        return ret;
    }

    timestamp = buffer.read_4_byte();
    zero = buffer.read_4_byte();
    if (zero != 0) {
        ret = ERROR_RTMP_PROTOCOL_C0C1_ZERO_NOT_ZERO;
        return ret;
    }

    random_data = buffer.read_bytes(1528);

    return ret;
}

string c0c1::dumps()
{
    RsBuffer buf;

    buf.write_1_byte(version);
    buf.write_4_byte(timestamp);
    buf.write_4_byte(zero);
    buf.write_bytes(random_data);

    return string(buf.dumps());
}

c2::c2()
{

}

c2::~c2()
{

}

int c2::initialize()
{
    int ret = ERROR_SUCCESS;
    timestamp = (uint32_t)rs_get_system_time_ms();
    timestamp2 = 0;
    random_data = rs_get_random(1528);
    return ret;
}

int c2::initialize(uint32_t ts, string rd)
{
    int ret = ERROR_SUCCESS;

    timestamp = (uint32_t)rs_get_system_time_ms();
    timestamp2 = ts;
    random_data = rd;

    return ret;
}

string c2::dumps()
{
    RsBuffer buf;

    buf.write_4_byte(timestamp);
    buf.write_4_byte(timestamp2);
    buf.write_bytes(random_data);

    return string(buf.dumps());
}

RsRtmpChunkMessage::RsRtmpChunkMessage()
{
}

RsRtmpChunkMessage::~RsRtmpChunkMessage()
{
}

int RsRtmpChunkMessage::initialize(IRsReaderWriter reader, uint32_t cs, uint32_t payload_length)
{
    int ret = ERROR_SUCCESS;
    chunk_size = cs;

    // read basic header
    {
        string buf;
        if ((ret = reader.read(buf, 1) != ERROR_SUCCESS)) {
            cout << "read fmt failed. ret=" << ret << endl;
            return ret;
        }
        fmt = uint8_t(buf.c_str()[0] & 0xc0);

        int tmp = buf.c_str()[0] & 0x3f;
        if (tmp > 1) {
            cs_id = uint8_t(tmp);
        }

        if (tmp == 0) {
            buf.clear();
            if ((ret = reader.read(buf, 1)) != ERROR_SUCCESS) {
                cout << "read 1 byte failed. ret=" << ret << endl;
                return ret;
            }
            cs_id = 64 + uint8_t(buf.c_str()[0]);
        }

        if (tmp == 1) {
            buf.clear();
            if ((ret = reader.read(buf, 2)) != ERROR_SUCCESS) {
                cout << "read 2 bytes failed. ret=" << ret << endl;
                return ret;
            }

            cs_id = 64 + uint8_t(buf.c_str()[0]) + 256 * uint8_t(buf.c_str()[1]);
        }
    }

    // read message header
    bool has_extended_timestamp = false;

    auto convert3bytesIntoUint32 = [](string buf) {
        RsBuffer rs_buf;
        rs_buf.write_bytes(buf.c_str(), 3);
        return rs_buf.read_3_byte();
    };

    auto convert4bytesIntoUint32 = [](string buf) {
        RsBuffer rs_buf;
        rs_buf.write_bytes(buf.c_str(), 4);
        return rs_buf.read_4_byte();
    };

    {
        if (fmt != 0 || fmt != 3) {
            // TODO:FIXME: only support type0 and type3
            assert(false);
        }

        string buf;
        if (fmt == 0) {
            // read timestamp
            if ((ret = reader.read(buf, 3)) != ERROR_SUCCESS) {
                cout << "read timestamp failed. ret=" << ret << endl;
                return ret;
            }
            timestamp = convert3bytesIntoUint32(buf);
            buf.clear();

            if (timestamp > 16777215) {
                has_extended_timestamp = true;
            }

            // read message length
            if ((ret = reader.read(buf, 3)) != ERROR_SUCCESS) {
                cout << "read message length failed. ret=" << ret << endl;
                return ret;
            }

            message_length = convert3bytesIntoUint32(buf);
            buf.clear();

            // read message type
            if ((ret = reader.read(buf, 1)) != ERROR_SUCCESS) {
                cout << "read message type failed. ret=" << ret << endl;
                return ret;
            }

            message_type = buf.c_str()[0];
            buf.clear();

            // read message stream id
            if ((ret = reader.read(buf, 4)) != ERROR_SUCCESS) {
                cout << "read message stream id failed. ret=" << ret << endl;
                return ret;
            }

            message_stream_id = convert4bytesIntoUint32(buf);
            buf.clear();

            // read extened timestamp if there it is
            if (has_extended_timestamp) {
                if ((ret = reader.read(buf, 4)) != ERROR_SUCCESS) {
                    cout << "read extended timestamp failed. ret=" << ret << endl;
                    return ret;
                }

                extended_timestamp = convert4bytesIntoUint32(buf);
                buf.clear();
            }

            // read payload
            int payload_length = message_length > chunk_size ? chunk_size : message_length;
            if ((ret = reader.read(buf, payload_length)) != ERROR_SUCCESS) {
                cout << "read message payload failed. ret=" << ret << endl;
                return ret;
            }
            chunk_data = buf;
            buf.clear();
        }
        if (fmt == 3) {
            if ((ret = reader.read(chunk_data, payload_length)) != ERROR_SUCCESS) {
                cout << "read chunk data failed. ret=" << ret << endl;
                return ret;
            }
        }
    }

    return ret;
}

string RsRtmpChunkMessage::dumps()
{
    RsBuffer msg;

    return string(msg.dumps());
}

vector<RsRtmpChunkMessage*> RsRtmpChunkMessage::create_chunk_messages(uint8_t fmt, string msg, uint8_t msg_type, uint32_t msg_stream_id)
{
    vector<RsRtmpChunkMessage*> msgs;

    return msgs;
}
