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

#ifndef RS_MODULE_RTMP_CONN_H_
#define RS_MODULE_RTMP_CONN_H_

#include <uv.h>
#include <rs_kernel_buffer.h>
#include "rs_common.h"
#include "rs_kernel_io.h"
#include "rs_kernel_connection.h"
#include "rs_protocol_rtmp.h"

/**
 * the basic rtmp connection class
 * @remark, can not be allocated
 */
class RsRtmpConn : public RsConnection {
protected:
    enum {
        rs_rtmp_connection_uninitialized = 0,
        rs_rtmp_connection_c0c1_received,
        rs_rtmp_connection_c2_received,
        rs_rtmp_connection_established,
    } _rtmp_status;

    RsRtmpChunkMsgAsync *_cache_message;
public:
    RsRtmpConn() : _rtmp_status(rs_rtmp_connection_uninitialized), _cache_message(nullptr) {};

    ~RsRtmpConn() override = default;

public:
    int initialize(IRsIO *io) override;

    void update_status() override;
};

/**
 * the connection from client whether publishing or playing stream
 */
class RsServerRtmpConn : public RsRtmpConn {
private:
    std::shared_ptr<RsTCPSocketIO> _tcp_io;

    std::shared_ptr<RsBufferLittleEndian> _rs_buffer;
public:
    RsServerRtmpConn();

    ~RsServerRtmpConn() override;

private:
    // for rtmp protocol
    int simple_handshake_with_server();

    int simple_handshake_with_client();

    int complex_handshake_with_server();

    int complex_handshake_witout_client();

private:
    static void on_message(char *, ssize_t, void *);

public:
    int initialize(IRsIO *io) override;

    void update_status() override;

};

/**
 * the connection to publishing or playing rtmp stream from other server
 * TODO:FIXME: implement this function
 *
 * class RsClientRtmpConn : public RsRtmpConn {
 * public:
 *     RsClientRtmpConn() = default;
 *
 *     ~RsClientRtmpConn() override = default;
 *
 * public:
 *     int initialize(IRsIO *io) override {
 *         assert(false);
 *         return ERROR_SUCCESS;
 *     };
 *
 *     void update_status() override { assert(false); };
 * public:
 *     int connect() {
 *         assert(false);
 *         return ERROR_SUCCESS;
 *     };
 *
 *     int play_stream() {
 *         assert(false);
 *         return ERROR_SUCCESS;
 *     };
 *
 *     int publish_stream() {
 *         assert(false);
 *         return ERROR_SUCCESS;
 *     };
 *
 * };
*/
#endif