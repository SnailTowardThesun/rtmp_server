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

#pragma once
#include "rs_common.h"
#include "rs_kernel_io.h"
#include "uv.h"

class c0c1
{
public:
    uint8_t version;
    uint32_t timestamp;
    uint32_t zero;
    std::string random_data;
public:
    c0c1();
    virtual ~c0c1();
public:
    int initialize();
    int initialize(std::string buf);
    std::string dumps();
};

class c2
{
public:
    uint32_t timestamp;
    uint32_t timestamp2;
    std::string random_data;
public:
    c2();
    virtual ~c2();
public:
    int initialize();
    int initialize(uint32_t ts, std::string rd);
    std::string dumps();

};

class s0s1 : public c0c1
{
public:
    s0s1(){};
    virtual ~s0s1(){};
};

class s2 : public c2
{
public:
    s2(){};
    virtual ~s2(){};
};

class RsRtmpChunkMessage
{
public:
    // chunk size
    uint32_t chunk_size;
    // basic header
    uint8_t fmt;
    uint32_t cs_id;
    // message header
    // type 0
    uint32_t timestamp;
    uint32_t message_length;
    uint8_t message_type;
    uint32_t message_stream_id;
    // type 1, 2
    uint32_t timestamp_delta;
    // extened timestamp
    uint32_t extended_timestamp;
    // chunk data
    std::string chunk_data;
public:
    RsRtmpChunkMessage();
    virtual ~RsRtmpChunkMessage();
public:
    int initialize(IRsReaderWriter reader, uint32_t cs, uint32_t payload_length);
    std::string dumps();
public:
    static std::vector<RsRtmpChunkMessage*> create_chunk_messages(uint8_t fmt, std::string msg, uint8_t msg_type, uint32_t msg_stream_id);
};
