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

#include "rs_kernel_io.h"

using namespace std;

#define MESSAGE_BUFFER_LENGTH 4096

RsTCPSocketIO::RsTCPSocketIO() : sock(nullptr) {
    base = shared_ptr<char>(new char[MESSAGE_BUFFER_LENGTH]);
}

RsTCPSocketIO::~RsTCPSocketIO() {
    if (sock) {
        close();
    }
}

int RsTCPSocketIO::listen(string ip, int port) {
    int ret = ERROR_SUCCESS;

    assert(sock == nullptr);
    sock = new uv_tcp_t();
    sock->data = this;

    if ((ret = uv_tcp_init(uv_default_loop(), sock)) != ERROR_SUCCESS) {
        cout << "create socket using libuv failed. ret=" << ret << endl;
        return ret;
    }

    struct sockaddr_in addr;
    if ((ret = uv_ip4_addr(ip.c_str(), port, &addr)) != ERROR_SUCCESS) {
        cout << "initialize the ip4 address failed. ret=" << ret << endl;
        return ret;
    }

    if ((ret = uv_tcp_bind(sock, (const struct sockaddr *) &addr, 0)) != ERROR_SUCCESS) {
        cout << "bind socket failed. ret=" << ret << endl;
        return ret;
    }

    if ((ret = uv_listen((uv_stream_t *) sock, 128, on_connected)) != ERROR_SUCCESS) {
        cout << "socket listen failed. ret=" << ret << endl;
        return ret;
    }

    return ret;
}

int RsTCPSocketIO::connect(std::string ip, int port) {
    // TODO:FIXME: implement this function
    int ret = ERROR_SUCCESS;

    assert(sock == nullptr);
    sock = new uv_tcp_t();
    sock->data = this;

    if ((ret = uv_tcp_init(uv_default_loop(), sock)) != ERROR_SUCCESS) {
        cout << "create scocket using libuv failed. ret=" << ret << endl;
        return ret;
    }

    struct sockaddr_in addr;
    if ((ret = uv_ip4_addr(ip.c_str(), port, &addr)) != ERROR_SUCCESS) {
        cout << "initialize the ip4 address failed. ret=" << ret << endl;
        return ret;
    }

    auto req = make_shared<uv_connect_t>();
    req->data = this;


    auto conn_cb = [](uv_connect_t *req, int status) -> void {
        if (status == UV_ECONNREFUSED) {
            std::cout << "connected" << std::endl;
        }
        cout << "connect failed" << endl;
    };

    if ((ret = uv_tcp_connect(req.get(), sock, (const struct sockaddr *) &addr,
                              conn_cb)) != ERROR_SUCCESS) {
        cout << "connected failed. ret=" << ret << endl;
        return ret;
    }

    return ret;
}

int RsTCPSocketIO::initialize(uv_tcp_t *stream) {
    int ret = ERROR_SUCCESS;

    assert(sock == nullptr);
    sock = stream;
    sock->data = this;

    // start to read
    auto alloc_cb = [](uv_handle_t *handle,
                       size_t suggested_size,
                       uv_buf_t *buf) {
        RsTCPSocketIO *io = (RsTCPSocketIO *) handle->data;
        *buf = uv_buf_init(io->base.get(), MESSAGE_BUFFER_LENGTH);
    };

    auto read_cb = [](uv_stream_t *stream,
                      ssize_t nread,
                      const uv_buf_t *buf) {
        RsTCPSocketIO *io = (RsTCPSocketIO *) stream->data;
        if (nread <= 0) {
            io->close();
            return;
        }

        io->buffer.append(buf->base, (unsigned long) nread);
        cout << "get message: " << buf->base << "nread = " << nread << endl;
    };

    if ((ret = uv_read_start((uv_stream_t *) sock, alloc_cb, read_cb)) != ERROR_SUCCESS) {
        cout << "start to read failed. ret=" << ret << endl;
        return ret;
    }
    cout << "ready to read message" << endl;
    return ret;
}

void RsTCPSocketIO::on_connected(uv_stream_t *server, int status) {
    int ret = ERROR_SUCCESS;

    RsTCPSocketIO *tcp = static_cast<RsTCPSocketIO *>(server->data);
    assert(tcp != nullptr);

    auto incomming = new uv_tcp_t();
    if ((ret = uv_tcp_init(server->loop, incomming)) != ERROR_SUCCESS) {
        cout << "initialzie the client socket from loop failed. ret=" << ret << endl;
        return;
    }

    if ((ret = uv_accept(server, (uv_stream_t *) incomming)) != ERROR_SUCCESS) {
        cout << "accept one connection failed. ret=" << ret << endl;
        return;
    }

    auto client = new RsTCPSocketIO();
    if ((ret = client->initialize(incomming)) != ERROR_SUCCESS) {
        cout << "initialize tcp socket with uv stream t failed. ret=" << ret << endl;
        return;
    }
}

int RsTCPSocketIO::write(string buf, int size) {
    int ret = ERROR_SUCCESS;

    assert(buf.size() >= size);

    auto write_cb = [](uv_write_t *req, int status) {
        if (status == UV_EINVAL) {
            cout << "invalid" << endl;
        }
        cout << "write finished, status=" << status << endl;
    };

    uv_write_t write_req;
    uv_buf_t test_buf = {(char *) buf.c_str(), (size_t) size};

    if ((ret = uv_write(&write_req, (uv_stream_t *) sock, &test_buf, 1, write_cb)) !=
        ERROR_SUCCESS) {
        cout << "write failed. ret=" << ret << endl;
        return ret;
    }

    return ret;
}

int RsTCPSocketIO::read(string &buf, int size) {
    int ret = ERROR_SUCCESS;

    while (buffer.size() < size) {
        cout << "wait for new message from client" << endl;
    }

    buf = buffer.substr(0, (unsigned long) size);
    buffer.erase(0, (unsigned long) size);
    return ret;
}

void RsTCPSocketIO::close() {
    uv_close((uv_handle_t *) sock, [](uv_handle_t *handle) {
        cout << "socket has been closed" << endl;
    });

    sock = nullptr;
}
