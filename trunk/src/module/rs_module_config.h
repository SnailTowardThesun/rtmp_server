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

namespace rs_config {

    const std::string DEFAULT_LOG_TANK_FILE_PATH = "./objs/log.log";

    enum RS_LOG_TANK_TYPE {
        RS_LOG_TANK_TYPE_CONSOLE = 0,
        RS_LOG_TANK_TYPE_FILE,
        RS_LOG_TANK_TYPE_UNKNOWN
    };

    class LogConfig {
    private:
        std::string filePath;
        RS_LOG_TANK_TYPE type;
    public:
        LogConfig() {
            filePath = DEFAULT_LOG_TANK_FILE_PATH;
            type = RS_LOG_TANK_TYPE_UNKNOWN;
        };

        LogConfig(std::string &p, RS_LOG_TANK_TYPE t) {
            filePath = p;
            type = t;
        }

        ~LogConfig() = default;

    public:
        std::string &get_path() { return filePath; }

        RS_LOG_TANK_TYPE get_type() { return type; }
    };

    class ServerItem {
    public:
        uint32_t listenPort;
        enum {
            RS_SERVER_TYPE_RTMP = 0
        } serverType;
    };

    class RsConfig {
    private:
        std::string conf;

        std::map<std::string, std::shared_ptr<ServerItem>> servers;

    private:
        RsConfig() = default;

    public:

        RsConfig(RsConfig const &) = delete;

        RsConfig &operator=(RsConfig const &) = delete;

        virtual ~RsConfig() = default;

    public:
        int initialize(std::string path);

    public:
        int get_rtmp_listen(std::string server);

    public:
        static std::shared_ptr<RsConfig> getInstance() {
            static std::shared_ptr<RsConfig> ins(new RsConfig());
            return ins;
        };
    };
}
