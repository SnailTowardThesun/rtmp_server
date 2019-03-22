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
protected:
    typedef enum {
        rs_io_uninitialized = 0,
        rs_io_open,
        rs_io_close
    } RS_IO_STATUS;

private:
    RS_IO_STATUS _status;
public:
    IRsIO() { _status = rs_io_uninitialized; };

    virtual ~IRsIO() = default;

protected:
    void change_status(RS_IO_STATUS status) {
        _status = status;
    }

public:
    bool is_initialized() { return _status != rs_io_uninitialized; }

    bool is_open() { return _status != rs_io_open; }

    bool is_stop() { return _status != rs_io_close; }
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
    on_new_connection_cb _on_conn_cb;
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
    int initialize(uv_stream_t *stream);

// implement IRsReaderWrite
public:
    int write(std::string buf, int size) override;

    int read(std::string &buf, int size) override;

private:
    void close();
};

#endif
