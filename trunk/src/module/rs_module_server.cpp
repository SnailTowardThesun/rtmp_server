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

#include <string.h>
#include "rs_module_server.h"
using namespace std;

RSRtmpServer::RSRtmpServer()
{
    server = new uv_tcp_t();
    server->data = static_cast<void*>(this);
}

RSRtmpServer::~RSRtmpServer()
{
    rs_free_p(server);
    conns.clear();
}

int RSRtmpServer::initialize()
{
    int ret = ERROR_SUCCESS;

    uv_tcp_init(uv_default_loop(), server);

    uv_ip4_addr("0.0.0.0", RTMP_DEFAULT_PORT, &addr);

    uv_tcp_bind(server, (const struct sockaddr*)&addr, 0);
    if ((ret = uv_listen((uv_stream_t*) server, 1024, connection_cb)) != ERROR_SUCCESS) {
        cout << "socket listen failed. ret=" << ret << endl;
    }

    return ret;
}

void RSRtmpServer::connection_cb(uv_stream_t *server, int status)
{

    RSRtmpServer* ptr = static_cast<RSRtmpServer*>(server->data);
    if (ptr) {
        return ptr->on_connection(server, status);
    }
    cout << "get class pointer failed." << endl;
}

void RSRtmpServer::on_connection(uv_stream_t *server, int status)
{
    cout << "get on connection" << endl;
    int ret = ERROR_SUCCESS;

    RsRtmpConn* conn = new RsRtmpConn();
    if ((ret = conn->initialize(server)) != ERROR_SUCCESS) {
        cout << "initialize rtmp connection failed. ret=" << ret << endl;
        rs_free_p(conn);
        return;
    }

    conns.push_back(shared_ptr<RsRtmpConn>(conn));
}

int RSRtmpServer::dispose()
{
    int ret = ERROR_SUCCESS;

    uv_close((uv_handle_t*)server, [](uv_handle_t *handle) {
        cout << "close rtmp server" << endl;
    });

    return ret;
}

RSServer::RSServer()
{
}

RSServer::~RSServer()
{
    exit();
}

static RSServer* _instance = nullptr;
RSServer* RSServer::getInstance()
{
    if (_instance == nullptr) {
        _instance = new RSServer();
    }
    return _instance;
}

int RSServer::run()
{
    int ret = ERROR_SUCCESS;

    // initialize the all servers
    if ((ret = _rtmp_server.initialize()) != ERROR_SUCCESS) {
        cout << "initialize the rtmp server failed. ret=" << ret << endl;
        return ret;
    }

    return uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

int RSServer::exit()
{
    int ret = ERROR_SUCCESS;
    if ((ret = _rtmp_server.dispose()) != ERROR_SUCCESS) {
        cout << "rtmp server dispose failed. ret=" << ret << endl;
        return ret;
    }

    // stop all loop
    uv_stop(uv_default_loop());

    if ((ret = uv_loop_close(uv_default_loop())) != ERROR_SUCCESS) {
        if (ret == UV_EBUSY) {
            cout << "loop is busy" << endl;
        }
        cout << "close uv loop failed. ret=" << ret << endl;
        return ret;
    }

    return ret;
}
