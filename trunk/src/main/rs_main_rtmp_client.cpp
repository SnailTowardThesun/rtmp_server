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

#include "rs_common.h"
#include "rs_kernel_io.h"

/**
 * Output the server info
 */
void _server_info()
{
    // TODO:FIXME: implement log system
    std::cout << "version: " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_REVISION << std::endl;
    std::cout << "Copyright (c) 2016 ME_Kun_Han hanvskun@hotmail.com" << std::endl;
    std::cout << "author : " << AUTHOR << std::endl;
}

uv_loop_t loop;
uv_tcp_t tcp_client;
uv_connect_t connection_request;
uv_write_t write_request;
uv_buf_t buf = { "HELLO", 5 };

static void write_cb(uv_write_t *req, int status) {
    if (status == UV_ECANCELED) {
        std::cout<< "write success" << std::endl;
    }
    uv_close((uv_handle_t*) req->handle, NULL);
}


static void connect_cb(uv_connect_t *req, int status) {
    if (status == UV_ECONNREFUSED) {
        std::cout << "connected" << std::endl;
    }
}

void send()
{
    struct sockaddr_in sa;
    if (0 != uv_ip4_addr("127.0.0.1", 1935, &sa)) {
        std::cout << "create ip failed" << std::endl;
        return;
    }
    if (0 != uv_tcp_init(uv_default_loop(), &tcp_client)) {
        std::cout << "initialzie the tcp failed." << std::endl;
    }

    if (0 != uv_tcp_connect(&connection_request,
                               &tcp_client,
                               (const struct sockaddr *)
                                       &sa,
                               connect_cb)) {
        std::cout << "connect failed" << std::endl;
    }

    if (0 != uv_write(&write_request,
                         (uv_stream_t *)&tcp_client,
                         &buf, 1,
                         write_cb)) {
        std::cout << "write failed." << std::endl;
    }



    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

int main(int argc, char* argv[])
{
    RsTCPSocketIO io;
    io.connect("127.0.0.1", 1935);
    io.write("test", 4);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}