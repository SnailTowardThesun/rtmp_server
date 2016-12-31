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
#include <netinet/in.h>

class RSuvSocket
{
    typedef struct {
        const char *bind_host;
        unsigned short bind_port;
        unsigned int idle_timeout;
    } server_config;

    typedef struct {
        unsigned int idle_timeout;  /* Connection idle timeout in ms. */
        uv_tcp_t tcp_handle;
        uv_loop_t *loop;
    } server_ctx;

    typedef struct {
        uv_getaddrinfo_t getaddrinfo_req;
        server_config config;
        server_ctx *servers;
        uv_loop_t *loop;
    }server_state;

private:
    int _port;
    struct addrinfo addr;
    server_state state;
private:
    static void do_bind(uv_getaddrinfo_t *req, int status, struct addrinfo *addrs);
    static void on_connection(uv_stream_t *server, int status);
    static void conn_alloc(uv_handle_t *handle, size_t size, uv_buf_t *buf);
    static void conn_read_done(uv_stream_t *handle, ssize_t  nread, const uv_buf_t *buf);
    static void close_done(uv_handle_t *handle);
public:
    RSuvSocket();
    virtual ~RSuvSocket();
public:
    virtual int initialize(int port);
};