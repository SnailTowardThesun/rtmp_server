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

#ifndef RS_KERNEL_IO_HEADER_H_
#define RS_KERNEL_IO_HEADER_H_

#include <uv.h>
#include "rs_common.h"
#include "rs_kernel_context.h"

class IRsIO {
public:
    IRsIO() = default;

    virtual ~IRsIO() = default;
};

class IRsReaderWriter : public IRsIO {
public:
    IRsReaderWriter() = default;

    virtual ~IRsReaderWriter() = default;

public:
    virtual int write(std::string buf, int size) = 0;

    virtual int read(std::string &buf, int size) = 0;

};

using on_new_connection_cb = std::function<void(IRsReaderWriter *, void *)>;

class RsTCPListener : public IRsIO {
private:
    uv_tcp_t _listen_sock{};
    void *_extra_param;
    on_new_connection_cb _cb;
public:
    RsTCPListener();

    ~RsTCPListener() override;

private:
    static void on_connection(uv_stream_t *s, int status);

public:
    int initialize(std::string ip, int port, on_new_connection_cb, void *param);

private:
    void close();
};

class RsTCPSocketIO : public IRsReaderWriter {
public:
    uv_tcp_t *_uv_tcp_socket;
    std::vector<char> _base;
    std::string buffer;
public:
    RsTCPSocketIO();

    ~RsTCPSocketIO() override;

public:
    int initialize(uv_tcp_t *stream);

// implement IRsReaderWrite
public:
    int write(std::string buf, int size) override;

    int read(std::string &buf, int size) override;

private:
    void close();
};

#endif
