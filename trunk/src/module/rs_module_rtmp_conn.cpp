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
#include "rs_module_rtmp_conn.h"
#include "rs_protocol_rtmp.h"

RsRtmpConn::RsRtmpConn() : _rtmp_status(RS_RTMP_CONN_STATUS::uninitialized),
               _cached_message(nullptr) {
    _cached_message = std::shared_ptr<RsRtmpChunkMsgAsync>(new RsRtmpChunkMsgAsync());
}

RsRtmpConn::~RsRtmpConn() {

}

int RsRtmpConn::initialize(IRsIO *io) {
    int ret = ERROR_SUCCESS;

    assert(io != nullptr);

    return ret;
}

void RsRtmpConn::update_status() {
}

RsServerRtmpConn::RsServerRtmpConn() {
    _rs_buffer = std::shared_ptr<RsBufferLittleEndian>(new RsBufferLittleEndian());
}

RsServerRtmpConn::~RsServerRtmpConn() {
    rs_info(_tcp_io.get(), "rtmp deconstruction");

    change_connection_status(rs_connection_stopped);
}

int RsServerRtmpConn::initialize(IRsIO *io) {
    auto ret = ERROR_SUCCESS;

    auto ptr = dynamic_cast<RsTCPSocketIO *>(io);
    assert(ptr != nullptr);

    _tcp_io.reset(ptr);


    if ((ret = _tcp_io->start_read(on_message, this)) != ERROR_SUCCESS) {
        rs_error(_tcp_io.get(), "start reading failed. ret=%d", ret);
        return ret;
    }

    change_connection_status(rs_connection_running);

    return ret;
}

void RsServerRtmpConn::update_status() {
    if (_tcp_io->is_stop()) {
        change_connection_status(rs_connection_stopped);
    }
}

void RsServerRtmpConn::on_message(char *buf, ssize_t size, void *param) {
    auto pt = (RsServerRtmpConn *) param;

    auto io = pt->_tcp_io;
    auto buffer = pt->_rs_buffer;
    auto cache_msg = pt->_cached_message;

    rs_info(io.get(), "get message from tcp io, size=%d", size);

    auto ret = ERROR_SUCCESS;

    buffer->write_bytes(buf, static_cast<int>(size));
    // do handshake
    if (pt->_rtmp_status == RS_RTMP_CONN_STATUS::uninitialized) {
        // get c0c1
        RtmpHandshakeC0C1 c0c1_obj;
        if ((ret = c0c1_obj.initialize(buffer.get())) != ERROR_SUCCESS) {
            rs_error(io.get(), "decode c0c1 failed. ret=%d", ret);
            return ;
        }

        // create s0s1s2
        RtmpHandshakeS0S1 s0s11;
        if ((ret = s0s11.initialize()) != ERROR_SUCCESS) {
            rs_error(io.get(), "create s0s1 failed. ret=%d", ret);
            return ;
        }

        RtmpHandshakeS2 s21;
        if ((ret = s21.initialize(c0c1_obj.timestamp, c0c1_obj.random_data)) != ERROR_SUCCESS) {
            rs_error(io.get(), "create s2 failed. ret=%d", ret);
            return ;
        }

        std::string str = s0s11.dump() + s21.dump();
        if ((ret = io->write(str, static_cast<int>(str.size()))) != ERROR_SUCCESS) {
            rs_error(io.get(), "send s0s1s2 failed. ret=%d", ret);
            return ;
        }

        // change status
        pt->_rtmp_status = RS_RTMP_CONN_STATUS::c0c1_received;
        return ;
    }

    // get c2
    if (pt->_rtmp_status == RS_RTMP_CONN_STATUS::c0c1_received) {
        if (buffer->length() < 1536) {
            return;
        }

        std::string str_c2;
        buffer->read(str_c2, 1536);

        // change status
        pt->_rtmp_status = RS_RTMP_CONN_STATUS::c2_received;


        rs_info(io.get(), "get c2 successfully");
        return ;
    }

    rs_info(io.get(), "msg: %s", buf);

    // reset cache message
    std::vector<uint8_t > msg(buf, buf + size);

    // receive connect cmd
    if (pt->_rtmp_status == RS_RTMP_CONN_STATUS::c2_received) {
        cache_msg->on_rtmp_msg(msg);
    }

    pt->_rtmp_status = RS_RTMP_CONN_STATUS::established;

    if (cache_msg->is_completed()) {
        cache_msg->clear();
    }

    // TODO:FIXME: achieve other handler for rtmp messages
}
