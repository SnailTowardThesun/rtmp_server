#include <utility>

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

#include "rs_module_log.h"
#include "rs_kernel_io.h"
#include "rs_kernel_context.h"

#define MESSAGE_BUFFER_LENGTH 4096

RsTCPListener::RsTCPListener() {
    _extra_param = nullptr;

    RsConnContext::getInstance()->do_register(this);
}

RsTCPListener::~RsTCPListener() {
    close();

    RsConnContext::getInstance()->do_deregister(this);
}

void RsTCPListener::on_connection(uv_stream_t *s, int status) {
    assert(status == 0);

    int ret = ERROR_SUCCESS;

    auto pt_this = static_cast<RsTCPListener *>(s->data);

    rs_info(pt_this, "get one tcp connection");

    auto io = new RsTCPSocketIO();
    if ((ret = io->initialize(s)) != ERROR_SUCCESS) {
        rs_error(pt_this, "accept one connection for tcp failed. ret=%d", ret);
        return;
    }

    pt_this->_on_conn_cb(io, pt_this->_extra_param);
}

int RsTCPListener::initialize(std::string ip, int port, on_new_connection_cb cb, void *param) {
    int ret = ERROR_SUCCESS;

    _on_conn_cb = cb;
    _extra_param = param;

    if ((ret = uv_tcp_init(uv_default_loop(), &_listen_sock)) != ERROR_SUCCESS) {
        rs_error(this, "create socket using libuv failed. ret=%d", ret);
        return ret;
    }

    _listen_sock.data = this;

    struct sockaddr_in addr{};
    if ((ret = uv_ip4_addr(ip.c_str(), port, &addr)) != ERROR_SUCCESS) {
        rs_error(this, "initialize the ip4 address failed. ret=%d", ret);
        return ret;
    }

    if ((ret = uv_tcp_bind(&_listen_sock, (const struct sockaddr *) &addr, 0)) != ERROR_SUCCESS) {
        rs_error(this, "bind socket failed. ret=%d", ret);
        return ret;
    }

    if ((ret = uv_listen((uv_stream_t *) &_listen_sock, 128, on_connection)) != ERROR_SUCCESS) {
        rs_error(this, "socket listen failed. ret=%d", ret);
        return ret;
    }

    change_status(rs_io_open);
    rs_info(this, "initialize one tcp listener success!");

    return ret;
}

void RsTCPListener::close() {
    uv_shutdown_t uv_shutdown_req;
    uv_shutdown_req.data = this;

    int ret = ERROR_SUCCESS;

    change_status(rs_io_close);

    auto shutdown_cb = [](uv_shutdown_t *req, int status) {
        auto io = (RsTCPListener *) req->data;
        rs_info(io, "do shutdown!");
    };

    if ((ret = uv_shutdown(&uv_shutdown_req, (uv_stream_t *) &_listen_sock, shutdown_cb)) != 0) {
        rs_error(this, "shutdown tcp listener failed. ret=%d", ret);
        return;
    }

    auto close_cb = [](uv_handle_t *handle) {
        rs_info(nullptr, "uv close handle function");
    };

    uv_close((uv_handle_t *) &_listen_sock, close_cb);
}

RsTCPSocketIO::RsTCPSocketIO() {
    _base.reserve(MESSAGE_BUFFER_LENGTH);
    _uv_tcp_socket = new uv_tcp_t();
    _extra_data = nullptr;

    RsConnContext::getInstance()->do_register(this);
}

RsTCPSocketIO::~RsTCPSocketIO() {
    rs_free_p(_uv_tcp_socket);

    RsConnContext::getInstance()->do_deregister(this);
}

int RsTCPSocketIO::initialize(uv_stream_t *stream) {
    int ret = ERROR_SUCCESS;

    if ((ret = uv_tcp_init(uv_default_loop(), _uv_tcp_socket)) != ERROR_SUCCESS) {
        rs_error(nullptr, "initialize the client socket from loop failed. ret=%d", ret);
        return ret;
    }

    if ((ret = uv_accept(stream, (uv_stream_t *) _uv_tcp_socket)) != ERROR_SUCCESS) {
        rs_error(nullptr, "accept one connection failed. ret=%d", ret);
        return ret;
    }

    _uv_tcp_socket->data = this;

    change_status(rs_io_open);
    rs_info(this, "ready to read message");
    return ret;
}

int RsTCPSocketIO::start_read(read_cb cb, void *param) {
    int ret = ERROR_SUCCESS;

    assert(_extra_data == nullptr);

    _read_cb = cb;
    _extra_data = param;

    // start to read
    auto alloc_cb = [](uv_handle_t *handle,
                       size_t suggested_size,
                       uv_buf_t *buf) {
        auto io = (RsTCPSocketIO *) handle->data;
        *buf = uv_buf_init(io->_base.data(), MESSAGE_BUFFER_LENGTH);
    };

    auto read_cb = [](uv_stream_t *stream,
                      ssize_t num_read,
                      const uv_buf_t *buf) {
        auto io = (RsTCPSocketIO *) stream->data;

        if (num_read <= 0) {
            io->close();
            return;
        }

        io->_read_cb(buf->base, num_read, io->_extra_data);

    };

    if ((ret = uv_read_start((uv_stream_t *) _uv_tcp_socket, alloc_cb, read_cb)) !=
        ERROR_SUCCESS) {
        rs_error(this, "start to read failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int RsTCPSocketIO::write(std::string buf, int size) {
    int ret = ERROR_SUCCESS;

    assert(buf.size() >= size);

    auto write_cb = [](uv_write_t *req, int status) {
        if (status == UV_EINVAL) {
            rs_error(nullptr, "invalid");
        }

        rs_info(nullptr, "write finished, status=%d", status);
    };

    uv_write_t *write_req = new uv_write_t();
    uv_buf_t test_buf = {(char *) buf.c_str(), (size_t) size};

    if ((ret = uv_write(write_req, (uv_stream_t *) _uv_tcp_socket, &test_buf, 1,
                        write_cb)) !=
        ERROR_SUCCESS) {
        rs_error(this, "write failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

void RsTCPSocketIO::close() {
    change_status(rs_io_close);
    rs_info(this, "do close one tcp connection");
    uv_close((uv_handle_t *) _uv_tcp_socket, NULL);
}
