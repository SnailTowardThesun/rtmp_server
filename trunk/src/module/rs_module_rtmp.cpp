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
using namespace std;

#define DEFAULT_BUFFER_LENGTH 4096

RsRtmpConn::RsRtmpConn() : _incomming(new uv_tcp_t())
        , _buffer_ptr(nullptr)
{
    _incomming->data = static_cast<void*>(this);
}

RsRtmpConn::~RsRtmpConn()
{
    rs_freep(_incomming);
    rs_freep(_buffer_ptr);
}

int RsRtmpConn::initialzie(uv_stream_t *server)
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
    if (_buffer_ptr != nullptr) {
        _buffer_ptr = new char[DEFAULT_BUFFER_LENGTH];
    }
    buf->base = _buffer_ptr;
    buf->len = DEFAULT_BUFFER_LENGTH;
}

void RsRtmpConn::do_conn_read_done(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    int ret = ERROR_SUCCESS;
    if ((ret = uv_read_stop(stream)) != ERROR_SUCCESS) {
        cout << "tcp socket stop read failed. ret=" << ret << endl;
        return;
    }

    uv_close((uv_handle_t*)stream, [](uv_handle_t* handle) {
        rs_freep(handle);
    });
}

