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
#include "rs_module_config.h"
#include "rs_module_log.h"

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

int
RsRtmpServer::initialize(const std::shared_ptr<rs_config::RSConfigServerItem> &config) {
    int ret = ERROR_SUCCESS;

    return ret;
}

void RsRtmpServer::on_connection(IRsReaderWriter *io, void *param) {
    cout << "got on connection for rtmp" << endl;
}

int RsRtmpServer::dispose() {
    int ret = ERROR_SUCCESS;

    return ret;
}

int RsServerManager::initialize() {
    int ret = ERROR_SUCCESS;

    return ret;
}

// TODO:FIXME: implement other type of server
int RsServerManager::create_new_server(
        const std::shared_ptr<rs_config::RSConfigServerItem> &config) {
    int ret = ERROR_SUCCESS;

    if (config->get_server_type() != rs_config::RS_SERVER_TYPE_RTMP) {
        rs_error(nullptr, "Sorry, we only support rtmp server now");
        ::exit(-1);
    }

    std::shared_ptr<RsBaseServer> baseServer = nullptr;

    auto rtmpServer = new RsRtmpServer();
    baseServer.reset(rtmpServer);

    if ((ret = rtmpServer->initialize(config)) != ERROR_SUCCESS) {
        rs_error(nullptr, "initialize rtmp server=%s failed. ret=%d",
                 config->get_server_name().c_str(), ret);
        return ret;
    }

    container[config->get_server_name()] = baseServer;

    return ret;
}

int RsServerManager::run() {
    return uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

int RsServerManager::exit() {
    int ret = ERROR_SUCCESS;

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
