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

#ifndef RS_MODULE_SERVER_H_
#define RS_MODULE_SERVER_H_

#include "rs_kernel_io.h"
#include "rs_module_rtmp_conn.h"
#include "rs_module_config.h"


class RsBaseServer {
public:
    RsBaseServer() = default;

    virtual ~RsBaseServer() = default;

public:
    virtual int initialize(rs_config::RsConfigBaseServer *config) = 0;

    virtual int dispose() = 0;
};

class RsRtmpServer : public RsBaseServer {
private:
    RsTCPListener *listen_sock;

    std::vector<std::shared_ptr<RsRtmpConn>> conns;
public:
    RsRtmpServer();

    ~RsRtmpServer() override;

private:
    static void on_new_connection(IRsReaderWriter *io, void *param);

public:
    int initialize(rs_config::RsConfigBaseServer *config) override;

    int dispose() override;
};

class RsServerManager {
private:
    using ServerContainer = std::map<std::string, std::shared_ptr<RsBaseServer>>;
    ServerContainer server_container;
public:
    RsServerManager() = default;

    ~RsServerManager() { stop(); };

public:
    int initialize(const rs_config::ConfigServerContainer &servers);

    int run();

    void stop();
};

#endif
