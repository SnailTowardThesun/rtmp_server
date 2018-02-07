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

#include "rs_module_server.h"
#include "rs_module_config.h"
#include "rs_common_utility.h"

using namespace std;

RsRtmpServer::RsRtmpServer() : sock(nullptr) {
    sock = new RsTCPSocketIO();
    sock->cb = on_connection;
    sock->param = this;
}

RsRtmpServer::~RsRtmpServer() {
    dispose();
    conns.clear();
    rs_free_p(sock);
}

int RsRtmpServer::initialize() {
    int ret = ERROR_SUCCESS;

    // create the listen socket
    string local_ip = rs_get_local_ip();
    int rtmp_port = RsConfig::getInstance()->get_rtmp_listen();
    if ((ret = sock->listen(local_ip, rtmp_port)) != ERROR_SUCCESS) {
        cout << "initialize socket for rtmp server failed. ret=" << ret << endl;
        return ret;
    }

    return ret;
}

void RsRtmpServer::on_connection(IRsReaderWriter *io, void *param) {
    cout << "got on connection for rtmp" << endl;
}

int RsRtmpServer::dispose() {
    int ret = ERROR_SUCCESS;

    return ret;
}

RsServer::RsServer() {

}

RsServer::~RsServer() {

}

static RsServer *_instance = nullptr;

RsServer *RsServer::getInstance() {
    if (_instance == nullptr) {
        _instance = new RsServer();
    }
    return _instance;
}

int RsServer::run() {
    int ret = ERROR_SUCCESS;

    // initialize the all servers
    if ((ret = _rtmp_server.initialize()) != ERROR_SUCCESS) {
        cout << "initialize the rtmp server failed. ret=" << ret << endl;
        return ret;
    }

    return uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

int RsServer::exit() {
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
