#include <memory>

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

#include <rs_module_server.h>
#include <rs_module_config.h>
#include <rs_common_utility.h>
#include <rs_module_config.h>
#include <rs_module_log.h>

RsRtmpServer::RsRtmpServer() {
    listen_sock = new RsTCPSocketIO();
    listen_sock->cb = on_connection;
    listen_sock->param = this;
}

RsRtmpServer::~RsRtmpServer() {
    dispose();
    conns.clear();
    rs_free_p(listen_sock);
}

void RsRtmpServer::on_connection(IRsReaderWriter *io, void *param) {
    rs_info(io, "get one connection for rtmp");
}

int RsRtmpServer::initialize(rs_config::RsConfigBaseServer *config) {
    int ret = ERROR_SUCCESS;

    rs_info(listen_sock, "ready to initialize a new rtmp server, name=%s, port=%d",
            config->get_server_name().c_str(), config->get_port());

    listen_sock->listen("0.0.0.0", config->get_port());

    return ret;
}

int RsRtmpServer::dispose() {
    int ret = ERROR_SUCCESS;

    listen_sock->close();

    return ret;
}

int RsServerManager::initialize(const rs_config::ConfigServerContainer &servers) {
    int ret = ERROR_SUCCESS;

    for (auto &i : servers) {
        auto config = i.second.get();

        std::shared_ptr<RsBaseServer> baseServer = nullptr;
        switch (config->get_type()) {
            case rs_config::RS_SERVER_TYPE_RTMP:
                baseServer = std::make_shared<RsRtmpServer>();
                break;
            default:
                rs_error(nullptr, "Sorry, we only support rtmp server now");
                return ERROR_CONFIGURE_TYPE_OF_SERVER_NOT_SUPPORT;
        }

        assert(baseServer != nullptr);

        if ((ret = baseServer->initialize(config)) != ERROR_SUCCESS) {
            rs_error(nullptr, "initialize server failed. name=%s, type=%d, ret=%d",
                     config->get_server_name().c_str(), config->get_type(), ret);
            return ret;
        }

        server_container[config->get_server_name()] = baseServer;
    }

    return ret;
}

int RsServerManager::run() {
    return uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

int RsServerManager::stop() {
    int ret = ERROR_SUCCESS;

    for (auto &i : server_container) {
        i.second->dispose();
    }

    server_container.clear();

    uv_stop(uv_default_loop());

    return ret;
}
