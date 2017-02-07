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

#include "rs_module_rtmp.h"
#include <string.h>
using namespace std;

#define DEFAULT_BUFFER_LENGTH 4096

RsRtmpConn::c0c1::c0c1()
{
}

RsRtmpConn::c0c1::~c0c1()
{

}

int RsRtmpConn::c0c1::initialize(char *buffer, int size)
{
    int ret = ERROR_SUCCESS;

    if (buffer[0] != 0x03) {
        cout << "c0c1 should be started with 0x03" << endl;
        ret = ERROR_RTMP_PROTOCOL_C0_ERROR;
        return ret;
    }

    if (size != 1537) {
        cout << "the length of c0c1 failed" << endl;
        ret = ERROR_RTMP_PROTOCOL_C0C1_LENGTH_ERROR;
        return ret;
    }

    return ret;
}

RsRtmpConn::s0s1s2::s0s1s2()
{

}

RsRtmpConn::s0s1s2::~s0s1s2()
{

}

int RsRtmpConn::s0s1s2::initialize(char *buffer, int size)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

int RsRtmpConn::s0s1s2::get_msg(uv_buf_t *buf)
{
    int ret = ERROR_SUCCESS;

    assert(buf != nullptr);

    buf->base = buff;
    buf->len = 3073;

    return ret;
}

RsRtmpConn::RsRtmpConn() : _incomming(new uv_tcp_t())
        , _write_buf(new uv_buf_t()), _write_req(new uv_write_t()), _ptr_read_buffer(nullptr)
{
    _incomming->data = static_cast<void*>(this);
    rtmp_conn_state = UN_CONNECTED;
    _ptr_write_buffer = new char[DEFAULT_BUFFER_LENGTH];
    _ptr_read_buffer = new char[DEFAULT_BUFFER_LENGTH];
}

RsRtmpConn::~RsRtmpConn()
{
    dispose();
    rs_free_p(_write_buf->base);
    rs_free_p(_write_buf);
    rs_free_p(_write_req);
    rs_free_p(_incomming);
    rs_free_p(_ptr_read_buffer);
    rs_free_p(_ptr_write_buffer);
}

int RsRtmpConn::initialize(uv_stream_t *server)
{
    int ret = ERROR_SUCCESS;

    if ((ret = uv_tcp_init(server->loop, _incomming)) != ERROR_SUCCESS) {
        cout << "init tcp failed. ret=" << ret << endl;
        return ret;
    }

    if ((ret = uv_accept(server, (uv_stream_t*)_incomming)) != ERROR_SUCCESS) {
        cout << "accept tcp socket failed. ret=" << ret;
        return ret;
    }

    if ((ret = uv_read_start((uv_stream_t*)_incomming, conn_alloc, conn_read_done)) != ERROR_SUCCESS) {
        cout << "read message failed. ret=" << ret;
        return ret;
    }

    return ret;
}

void RsRtmpConn::conn_alloc(uv_handle_t *handle, size_t size, uv_buf_t *buf)
{
    cout << "step into alloc function" << endl;

    RsRtmpConn* con = static_cast<RsRtmpConn*>(handle->data);
    if (con) {
        return con->do_conn_alloc(handle, size, buf);
    }
    cout << "get class object failed." << endl;
}

void RsRtmpConn::conn_read_done(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    cout << "step into read done function" << endl;

    RsRtmpConn* con = static_cast<RsRtmpConn*>(stream->data);
    if (con) {
        return con->do_conn_read_done(stream, nread, buf);
    }
    cout << "get class object failed." << endl;
}

void RsRtmpConn::do_conn_alloc(uv_handle_t *handle, size_t size, uv_buf_t *buf)
{
    buf->base = _ptr_read_buffer;
    buf->len = DEFAULT_BUFFER_LENGTH;
}

void RsRtmpConn::do_conn_read_done(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    cout << buf << endl;

    int ret = ERROR_SUCCESS;
    if ((ret = handle(buf->base, nread)) != ERROR_SUCCESS) {
        cout << "handle rtmp message failed. ret=" << ret << endl;
        return;
    }

    if (_write_buf->len > 0) {
        ret = uv_write(_write_req, (uv_stream_t*)_incomming, _write_buf, 1, [](uv_write_t* req, int status){
            cout << "write one message to client" << endl;
        });
        if (ret != ERROR_SUCCESS) {
            cout << "rtmp write message failed. ret=" << ret << endl;
        }
    }
}

int RsRtmpConn::handshake(char *read, int read_nb)
{
    int ret = ERROR_SUCCESS;

    if (rtmp_conn_state == UN_CONNECTED) {
        c0c1 pkt;
        if ((ret = pkt.initialize(read, read_nb)) != ERROR_SUCCESS) {
            cout << "decode c0c1 failed. ret=" << ret << endl;
            return ret;
        }
        if ((ret = handshake_s_packet.initialize(read, read_nb)) != ERROR_SUCCESS) {
            cout << "create s0s1s2 packet failed. ret=" << ret << endl;
            return ret;
        }

        if ((ret = handshake_s_packet.get_msg(_write_buf)) != ERROR_SUCCESS) {
            cout << "get s0s1s2 message failed. ret=" << ret << endl;
            return ret;
        }

        if ((ret = uv_write(_write_req, (uv_stream_t*)_incomming, _write_buf, 1, [](uv_write_t *req, int status) {
            cout << "write message done" << endl;
        })) != ERROR_SUCCESS) {
            cout << "write s0s1s2 failed. ret = " << ret << endl;
            return ret;
        }
        rtmp_conn_state = DO_HANDSHAKE;
    }

    rtmp_conn_state = DO_CONNECT_STREAM;

    return ret;
}

int RsRtmpConn::handle(char *read, int read_nb)
{
    int ret = ERROR_SUCCESS;

    if (read == nullptr || read_nb < 1) {
        cout << "the message is empty" << endl;
        return ret;
    }

    switch (rtmp_conn_state) {
    case UN_CONNECTED:
    case DO_HANDSHAKE:
        if ((ret = handshake(read, read_nb)) != ERROR_SUCCESS) {
            cout << "handshake failed. ret=" << endl;
        }
        break;
    case DO_CONNECT_STREAM:
        break;
    case CONNECTED:
        break;
    default:
        cout << "the state of rtmp connection is error" << endl;
        break;
    }

    return ret;
}

int RsRtmpConn::dispose()
{
    int ret = ERROR_SUCCESS;

    if (_incomming == nullptr) {
        return ret;
    }

    if ((ret = uv_read_stop((uv_stream_t*)_incomming))) {
        cout << "tcp socket stop read failed. ret=" << ret << endl;
        return ret;
    }

    uv_close((uv_handle_t*)_incomming, [](uv_handle_t* handle) {
        cout << "close one connection" << endl;
        rs_free_p(handle);
    });

    return ret;
}

