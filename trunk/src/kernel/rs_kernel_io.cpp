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

RsTCPSocketIO::RsTCPSocketIO() : sock(nullptr)
{
    signal = make_shared<uv_sem_t>();
    base = shared_ptr<char>(new char[MESSAGE_BUFFER_LENGTH]);
}

RsTCPSocketIO::~RsTCPSocketIO()
{
    if (sock) {
        close();
    }
}

int RsTCPSocketIO::listen(string ip, int port) {
    int ret = ERROR_SUCCESS;

    sock = shared_ptr<uv_tcp_t>(new uv_tcp_t());
    sock->data = this;

    if ((ret = uv_tcp_init(uv_default_loop(), sock.get())) != ERROR_SUCCESS) {
        cout << "create socket using libuv failed. ret=" << ret << endl;
        return ret;
    }

    struct sockaddr_in addr;
    if ((ret = uv_ip4_addr(ip.c_str(), port, &addr)) != ERROR_SUCCESS) {
        cout << "initialize the ip4 address failed. ret=" << ret << endl;
        return ret;
    }

    if ((ret = uv_tcp_bind(sock.get(), (const struct sockaddr*)&addr, 0)) != ERROR_SUCCESS) {
        cout << "bind socket failed. ret=" << ret << endl;
        return ret;
    }

    if ((ret = uv_listen((uv_stream_t*)sock.get(), 128, on_connected)) != ERROR_SUCCESS) {
        cout << "socket listen failed. ret=" << ret << endl;
        return ret;
    }

    return ret;
}

int RsTCPSocketIO::read(std::string &buf, int size)
{
    // TODO:FIXME: implement this function
    int ret = ERROR_SUCCESS;

    return ret;
}

int RsTCPSocketIO::initialize(uv_tcp_t *stream)
{
    int ret = ERROR_SUCCESS;

    sock = shared_ptr<uv_tcp_t>(stream);

    // start to read
    auto alloc_cb = [](uv_handle_t* handle,
                       size_t suggested_size,
                       uv_buf_t* buf) {
        RsTCPSocketIO *io = (RsTCPSocketIO*) handle->data;
        *buf = uv_buf_init(io->base.get(), MESSAGE_BUFFER_LENGTH);
    };

    auto read_cb = [](uv_stream_t* stream,
                      ssize_t nread,
                      const uv_buf_t* buf) {
        RsTCPSocketIO *io = (RsTCPSocketIO*) stream->data;
        io->buffer.append(buf->base, (unsigned long) nread);

        // send signal
        uv_sem_post(io->signal.get());
    };

    if ((ret = uv_read_start((uv_stream_t*)sock.get(), alloc_cb, read_cb)) != ERROR_SUCCESS) {
        cout << "start to read failed. ret=" << ret << endl;
        return ret;
    }

    return ret;
}

void RsTCPSocketIO::on_connected(uv_stream_t *stream, int status)
{
    int ret = ERROR_SUCCESS;

    RsTCPSocketIO* tcp = static_cast<RsTCPSocketIO*>(stream->data);
    assert(tcp != nullptr);

    cout << "get a connection" << endl;

    auto socket = make_shared<uv_tcp_t>();
    if ((ret = uv_tcp_init(stream->loop, socket.get())) != ERROR_SUCCESS) {
        cout << "initialzie the client socket from loop failed. ret=" << ret << endl;
        return ;
    }

    if ((ret = uv_accept(stream, (uv_stream_t*)socket.get())) != ERROR_SUCCESS) {
        cout << "accept one connection failed. ret=" << ret << endl;
        return ;
    }

    auto client = make_shared<RsTCPSocketIO>();
    if ((ret = client->initialize(socket.get())) != ERROR_SUCCESS) {
        cout << "initialize tcp socket with uv stream t failed. ret=" << ret << endl;
        return ;
    }
    tcp->cb(client.get());
}

int RsTCPSocketIO::write(string buf, int size)
{
    // TODO:FIXME: implement this function
    int ret = ERROR_SUCCESS;

    uv_write_t write_req;
    uv_buf_t uv_buf = { (char*)buf.c_str(), (size_t)size };

    auto write_cb = [](uv_write_t *req, int status) {
        cout << "write finished, status=" << status << endl;
    };

    if ((ret = uv_write(&write_req, (uv_stream_t*)sock.get(), &uv_buf, (unsigned int) size, write_cb)) != ERROR_SUCCESS) {
        cout << "write failed. ret=" << ret << endl;
        return ret;
    }

    return ret;
}

int RsTCPSocketIO::read(string& buf, int size)
{
    int ret = ERROR_SUCCESS;

    while (buffer.size() < size) {
        cout << "wait for new message from client" << endl;
        uv_sem_wait(signal.get());
    }

    buf = buffer.substr(0, (unsigned long) size);
    return ret;
}

void RsTCPSocketIO::close()
{
    uv_close((uv_handle_t*)sock.get(), [](uv_handle_t* handle){
        cout << "socket has been closed" << endl;
    });
}
