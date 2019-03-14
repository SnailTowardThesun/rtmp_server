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
#include "uv.h"

class IRsReaderWriter {
public:
    IRsReaderWriter() = default;

    virtual ~IRsReaderWriter() = default;

public:
    virtual int write(std::string buf, int size) = 0;

    virtual int read(std::string &buf, int size) = 0;
};

using connection_cb = void (*)(IRsReaderWriter *io, void *param);

class RsTCPSocketIO : public IRsReaderWriter {
public:
    uv_tcp_t sock;
    std::shared_ptr<char> base;
    std::string buffer;
public:
    RsTCPSocketIO();

    ~RsTCPSocketIO() override;

public:
    connection_cb cb;
    void *param;
public:
    int listen(std::string ip, int port);

    int connect(std::string ip, int port);

    int initialize(uv_tcp_t *stream);

    static void on_connected(uv_stream_t *server, int status);
// implement IRsReaderWrite
public:
    int write(std::string buf, int size) override;

    int read(std::string &buf, int size) override;

    void close();
};
