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

#include "rs_module_buffer.h"
using namespace std;

RsBuffer::RsBuffer()
{

}

RsBuffer::~RsBuffer()
{
}

int RsBuffer::write_1_byte(uint8_t val)
{
    int ret = ERROR_SUCCESS;

    char *p = (char*)&val;

    buffer += p[0];

    return ret;
}

int RsBuffer::write_2_byte(uint16_t val)
{
    int ret = ERROR_SUCCESS;

    char *pVal = (char*)&val;

    buffer += pVal[1];
    buffer += pVal[0];

    return ret;
}

int RsBuffer::write_3_byte(uint32_t val)
{
    int ret = ERROR_SUCCESS;

    char *pVal = (char*)&val;

    buffer += pVal[2];
    buffer += pVal[1];
    buffer += pVal[0];

    return ret;
}

int RsBuffer::write_4_byte(uint32_t val)
{
    int ret = ERROR_SUCCESS;

    char *pVal = (char*)&val;

    buffer += pVal[3];
    buffer += pVal[2];
    buffer += pVal[1];
    buffer += pVal[0];

    return ret;
}

int RsBuffer::write_8_byte(uint64_t val)
{
    int ret = ERROR_SUCCESS;

    char *pVal = (char*)&val;

    buffer += pVal[7];
    buffer += pVal[6];
    buffer += pVal[5];
    buffer += pVal[4];
    buffer += pVal[3];
    buffer += pVal[2];
    buffer += pVal[1];
    buffer += pVal[0];

    return ret;
}

int RsBuffer::write_bytes(char *buf, int size)
{
    int ret = ERROR_SUCCESS;

    buffer += string((const char*)buf, (unsigned long) size);

    return ret;
}

uint8_t RsBuffer::read_1_byte()
{
    uint8_t val = 0;

    val = static_cast<uint8_t>(buffer[0]);

    buffer.erase(buffer.begin());

    return val;
}

uint16_t RsBuffer::read_2_byte()
{
    uint16_t val = 0;

    char *pVal = (char*)&val;

    for (int i = 1; i >= 0; --i) {
        pVal[i] = buffer.at(0);
        buffer.erase(buffer.begin());
    }

    return val;
}

uint32_t RsBuffer::read_3_byte()
{
    uint32_t val = 0;

    char *pVal = (char*)&val;

    for (int i = 2; i >= 0; --i) {
        pVal[i] = buffer.at(0);
        buffer.erase(buffer.begin());
    }

    return val;
}

uint32_t RsBuffer::read_4_byte()
{
    uint32_t val = 0;

    char *pVal = (char*)&val;

    for (int i = 3; i >= 0; --i) {
        pVal[i] = buffer.at(0);
        buffer.erase(buffer.begin());
    }

    return  val;
}

uint64_t RsBuffer::read_8_byte()
{
    uint64_t val = 0;

    char *pVal = (char*)&val;

    for (int i = 7; i >= 0; --i) {
        pVal[i] = buffer.at(0);
        buffer.erase(buffer.begin());
    }

    return val;
}

int RsBuffer::read_bytes(char *buf, int size)
{
    int ret = ERROR_SUCCESS;

    assert(buf != nullptr);
    assert(buffer.size() >= size);

    for (int i = 0; i < size; ++i) {
        buf[i] = buffer.at(0);
        buffer.erase(buffer.begin());
    }

    return ret;
}

const char* RsBuffer::dumps()
{
    if (!buffer.empty()) {
        return buffer.c_str();
    }

    cout << "the buffer is empty" << endl;
    return nullptr;
}