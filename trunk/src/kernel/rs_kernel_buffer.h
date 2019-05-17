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

#ifndef RS_KERNEL_BUFFER_HEADER_H_
#define RS_KERNEL_BUFFER_HEADER_H_


#include "rs_common.h"
#include "rs_kernel_io.h"

// little endian
class RsBufferLittleEndian : public IRsReaderWriter {
private:
    std::string buffer;
public:
    RsBufferLittleEndian() = default;

    ~RsBufferLittleEndian() override = default;

public:
    int write_1_byte(uint8_t val);

    int write_2_byte(unsigned long val);

    int write_3_byte(uint32_t val);

    int write_4_byte(uint32_t val);

    int write_8_byte(uint64_t val);

    int write_bytes(std::string buf);

    int write_bytes(const char *buf, int size);

    int write(std::string buf, int size) override { return write_bytes(buf.c_str(), size); }

    uint8_t read_1_byte();

    uint16_t read_2_byte();

    uint32_t read_3_byte();

    uint32_t read_4_byte();

    uint64_t read_8_byte();

    std::string read_bytes(int size);

    int read(std::string &buf, int size) override {
        buf = read_bytes(size);
        return ERROR_SUCCESS;
    }

    int start_read(read_cb, void *param) override {
        return ERROR_SUCCESS;
    }

    std::string dump();

    size_t length() { return buffer.size(); }

    void clear() { buffer.clear(); };

public:
    static uint16_t convert_2bytes_into_uint16(std::string buf);

    static uint32_t convert_3bytes_into_uint32(std::string buf);

    static uint32_t convert_4bytes_into_uint32(std::string buf);

    static uint64_t convert_8bytes_into_uint64(std::string buf);
};

#endif
