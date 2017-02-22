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
#define CHUNK_MESSAGE_TIMESTAMP_MAX 16777215

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

    RsBufferLittleEndian buffer;
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
    RsBufferLittleEndian buf;

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
    RsBufferLittleEndian buf;

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

int RsRtmpChunkMessage::type_0_decode(IRsReaderWriter* reader)
{
    int ret = ERROR_SUCCESS;
    bool has_extended_timestamp = false;
    string buf;

    assert(fmt == 0);
    // read timestamp
    if ((ret = reader->read(buf, 3)) != ERROR_SUCCESS) {
        cout << "read timestamp failed. ret=" << ret << endl;
        return ret;
    }
    timestamp = RsBufferLittleEndian::convert_3bytes_into_uint32(buf);
    buf.clear();

    if (timestamp > CHUNK_MESSAGE_TIMESTAMP_MAX) {
        has_extended_timestamp = true;
    }

    // read message length
    if ((ret = reader->read(buf, 3)) != ERROR_SUCCESS) {
        cout << "read message length failed. ret=" << ret << endl;
        return ret;
    }

    message_length = RsBufferLittleEndian::convert_3bytes_into_uint32(buf);
    buf.clear();

    // read message type
    if ((ret = reader->read(buf, 1)) != ERROR_SUCCESS) {
        cout << "read message type failed. ret=" << ret << endl;
        return ret;
    }

    message_type_id = (uint8_t) buf.c_str()[0];
    buf.clear();

    // read message stream id
    if ((ret = reader->read(buf, 4)) != ERROR_SUCCESS) {
        cout << "read message stream id failed. ret=" << ret << endl;
        return ret;
    }

    message_stream_id = RsBufferLittleEndian::convert_4bytes_into_uint32(buf);
    buf.clear();

    // read extened timestamp if there it is
    if (has_extended_timestamp) {
        if ((ret = reader->read(buf, 4)) != ERROR_SUCCESS) {
            cout << "read extended timestamp failed. ret=" << ret << endl;
            return ret;
        }

        extended_timestamp = RsBufferLittleEndian::convert_4bytes_into_uint32(buf);
        buf.clear();
    }

    // read payload
    int length = message_length > chunk_size ? chunk_size : message_length;
    if ((ret = reader->read(chunk_data, length)) != ERROR_SUCCESS) {
        cout << "read message payload failed. ret=" << ret << endl;
        return ret;
    }

    return ERROR_SUCCESS;
}

int RsRtmpChunkMessage::type_1_decode(IRsReaderWriter *reader)
{
    int ret = ERROR_SUCCESS;
    string buf;

    // timestamp delta
    if ((ret = reader->read(buf, 3)) != ERROR_SUCCESS) {
        cout << "read timestamp delta failed. ret=" << ret << endl;
        return ret;
    }
    timestamp_delta = RsBufferLittleEndian::convert_3bytes_into_uint32(buf);
    buf.clear();

    // message length
    if ((ret = reader->read(buf, 3)) != ERROR_SUCCESS) {
        cout << "read message length failed. ret=" << ret << endl;
        return ret;
    }
    message_length = RsBufferLittleEndian::convert_3bytes_into_uint32(buf);
    buf.clear();

    // message type id
    if ((ret = reader->read(buf, 1)) != ERROR_SUCCESS) {
        cout << "read message type id failed. ret=" << ret << endl;
        return ret;
    }
    message_type_id = (uint8_t) buf.c_str()[0];
    buf.clear();

    // chunk data
    int length = message_length > chunk_size ? chunk_size : message_length;
    if ((ret = reader->read(chunk_data, length)) != ERROR_SUCCESS) {
        cout << "read chunk data failed. ret=" << ret << endl;
        return ret;
    }

    return ret;
}

int RsRtmpChunkMessage::type_2_decode(IRsReaderWriter* reader, uint32_t size)
{
    int ret = ERROR_SUCCESS;
    string buf;

    // timestamp delta
    if ((ret = reader->read(buf, 3)) != ERROR_SUCCESS) {
        cout << "read timestamp delta failed. ret=" << ret << endl;
        return ret;
    }
    timestamp_delta = RsBufferLittleEndian::convert_3bytes_into_uint32(buf);
    buf.clear();

    // chunk data
    if ((ret = reader->read(chunk_data, size)) != ERROR_SUCCESS) {
        cout << "read chunk data failed. ret=" << ret << endl;
        return ret;
    }

    return ret;
}

int RsRtmpChunkMessage::type_3_decode(IRsReaderWriter* reader, uint32_t size)
{
    int ret = ERROR_SUCCESS;

    if ((ret = reader->read(chunk_data, size)) != ERROR_SUCCESS) {
        cout << "read chunk data from reader failed. ret=" << ret << endl;
    }

    return ret;
}

string RsRtmpChunkMessage::basic_header_dump()
{
    RsBufferLittleEndian buf;
    if (cs_id < 64) {
        uint8_t cid = (uint8_t) cs_id;
        buf.write_1_byte((uint8_t) (((fmt << 6) & 0xc0) | (cid & 0x3f)));
        return buf.dumps();
    } else if (cs_id < 320) {
        uint8_t cid = (uint8_t) (cs_id - 64);
        buf.write_1_byte((uint8_t) ((fmt << 6) & 0xc0));
        buf.write_1_byte(cid);
        return buf.dumps();
    } else if (cs_id < 65599) {
        buf.write_1_byte((uint8_t) ((fmt << 6) & 0xc0 + 1));

        uint16_t cid = uint16_t(cs_id - 64);
        buf.write_1_byte((uint8_t)cid);
        buf.write_1_byte((uint8_t)(cid >> 8));
        return buf.dumps();
    }

    cout << "cs id is beyound the limits" << endl;
    return "";
}

int RsRtmpChunkMessage::type_0_dump(string& buf)
{
    int ret = ERROR_SUCCESS;

    RsBufferLittleEndian rs_buf;

    // basic header
    rs_buf.write_bytes(basic_header_dump());
    // timestamp
    rs_buf.write_3_byte(timestamp);
    // message_length
    rs_buf.write_3_byte(message_length);
    // message type id
    rs_buf.write_1_byte(message_type_id);
    // message stream id
    rs_buf.write_4_byte(message_stream_id);
    // extended timestamp
    if (timestamp >= CHUNK_MESSAGE_TIMESTAMP_MAX && extended_timestamp != 0) {
        rs_buf.write_4_byte(extended_timestamp);
    }
    // chunk data
    rs_buf.write_bytes(chunk_data);

    buf = rs_buf.dumps();
    return ret;
}

int RsRtmpChunkMessage::type_1_dump(string& buf)
{
    int ret = ERROR_SUCCESS;

    RsBufferLittleEndian rs_buf;

    //basic header
    rs_buf.write_bytes(basic_header_dump());
    // timestamp delta
    rs_buf.write_3_byte(timestamp_delta);
    // message length
    rs_buf.write_3_byte(message_length);
    // message type id
    rs_buf.write_1_byte(message_type_id);
    // chunk data
    rs_buf.write_bytes(chunk_data);

    buf = rs_buf.dumps();
    return ret;
}

int RsRtmpChunkMessage::type_2_dump(string& buf)
{
    int ret =  ERROR_SUCCESS;

    RsBufferLittleEndian rs_buf;

    // basic header
    rs_buf.write_bytes(basic_header_dump());
    // timestamp delta
    rs_buf.write_3_byte(timestamp_delta);
    // chunk data
    rs_buf.write_bytes(chunk_data);

    buf = rs_buf.dumps();
    return ret;
}

int RsRtmpChunkMessage::type_3_dump(string& buf)
{
    int ret = ERROR_SUCCESS;
    buf.append(basic_header_dump());
    buf.append(chunk_data);
    return ret;
}

int RsRtmpChunkMessage::initialize(IRsReaderWriter *reader, uint32_t cs, uint32_t payload_length)
{
    int ret = ERROR_SUCCESS;
    chunk_size = cs;

    // read basic header
    string buf;
    if ((ret = reader->read(buf, 1) != ERROR_SUCCESS)) {
        cout << "read fmt failed. ret=" << ret << endl;
        return ret;
    }
    fmt = uint8_t((buf.c_str()[0] & 0xc0) >> 6);

    int tmp = buf.c_str()[0] & 0x3f;
    if (tmp > 1) {
        cs_id = uint8_t(tmp);
    }

    if (tmp == 0) {
        buf.clear();
        if ((ret = reader->read(buf, 1)) != ERROR_SUCCESS) {
            cout << "read 1 byte failed. ret=" << ret << endl;
            return ret;
        }
        cs_id = 64 + uint8_t(buf.c_str()[0]);
    }

    if (tmp == 1) {
        buf.clear();
        if ((ret = reader->read(buf, 2)) != ERROR_SUCCESS) {
            cout << "read 2 bytes failed. ret=" << ret << endl;
            return ret;
        }

        cs_id = (uint32_t) (64 + uint8_t(buf.c_str()[0]) + 256 * uint8_t(buf.c_str()[1]));
    }

    // read message header and chunk data
    switch (fmt) {
        case 0:
            return type_0_decode(reader);
        case 1:
            return type_1_decode(reader);
        case 2:
            return type_2_decode(reader, cs < payload_length ? cs : payload_length);
        case 3:
            return type_3_decode(reader, cs < payload_length ? cs : payload_length);
        default:
            cout << "the fmt should be 0, 1, 2, 3, now is " << fmt << endl;
            ret = ERROR_RTMP_PROTOCOL_CHUNK_MESSAGE_FMT_ERROR;
            return ret;
    }
}

int RsRtmpChunkMessage::dumps(string& buf)
{
    buf.clear();

    switch (fmt) {
        case 0:
            return type_0_dump(buf);
        case 1:
            return type_1_dump(buf);
        case 2:
            return type_2_dump(buf);
        case 3:
            return type_3_dump(buf);
        default:
            cout << "the fmt is beyound the limist" << endl;
    }

    return ERROR_SUCCESS;
}

RTMP_CHUNK_MESSAGES RsRtmpChunkMessage::create_chunk_messages(uint32_t ts, string msg, uint8_t msg_type, uint32_t msg_stream_id, uint32_t cs)
{
    RTMP_CHUNK_MESSAGES msgs;

    // create type 0 message
    RsRtmpChunkMessage* type0 = new RsRtmpChunkMessage();
    type0->fmt = 0;
    type0->chunk_size = cs;
    type0->timestamp = ts;
    if (ts > CHUNK_MESSAGE_TIMESTAMP_MAX) {
        type0->timestamp = CHUNK_MESSAGE_TIMESTAMP_MAX;
        type0->extended_timestamp = ts;
    }

    type0->message_length = (uint32_t) msg.length();
    type0->message_type_id = msg_type;
    type0->message_stream_id = msg_stream_id;

    auto data_length = msg.length() > cs ? cs : msg.length();
    type0->chunk_data = msg.substr(0, data_length);
    msg.erase(0, data_length);

    msgs.push_back(shared_ptr<RsRtmpChunkMessage> (type0));

    // create type 3 messages
    while (!msg.empty()) {
        RsRtmpChunkMessage* type3 = new RsRtmpChunkMessage();
        type3->fmt = 3;
        auto length = msg.length() > cs ? cs : msg.length();
        type3->chunk_data = msg.substr(0, length);
        msg.erase(0, length);

        msgs.push_back(shared_ptr<RsRtmpChunkMessage>(type3));
    }

    return msgs;
}
