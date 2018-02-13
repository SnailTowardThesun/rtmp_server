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

#include "rs_kernel_io.h"
#include "rs_module_rtmp_conn.h"
#include "rs_module_config.h"

class RsBaseServer {
public:
    RsBaseServer() = default;

    virtual ~RsBaseServer() = default;

public:
    virtual int
    initialize(const std::shared_ptr<rs_config::RSConfigServerItem> &config) = 0;

    virtual int dispose() = 0;
};

class RsRtmpServer : public RsBaseServer {
private:
    RsTCPSocketIO *sock;

public:
    RsRtmpServer();

    ~RsRtmpServer() override;

public:
    int initialize(const std::shared_ptr<rs_config::RSConfigServerItem> &config) override;

    static void on_connection(IRsReaderWriter *io, void *param);

private:
    std::vector<std::shared_ptr<RsRtmpConn>> conns;
public:
    int dispose() override;
};

using ServerContainer = std::map<std::string, std::shared_ptr<RsBaseServer>>;

class RsServerManager {
private:
    ServerContainer container;
public:
    RsServerManager() = default;

    ~RsServerManager() = default;

public:
    int initialize();

    int create_new_server(const std::shared_ptr<rs_config::RSConfigServerItem> &config);

    int run();

    int exit();
};
