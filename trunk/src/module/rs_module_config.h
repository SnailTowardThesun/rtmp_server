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

#include <rapidjson/document.h>
#include "rs_common.h"

namespace rs_config {

    enum RS_LOG_TANK_TYPE {
        RS_LOG_TANK_TYPE_CONSOLE = 0,
        RS_LOG_TANK_TYPE_FILE
    };

    static const char *DEFAULT_LOG_TANK_FILE_PATH = "./objs/log.log";
    static const RS_LOG_TANK_TYPE DEFAULT_LOG_TANK_TYPE = RS_LOG_TANK_TYPE_CONSOLE;

    class RSConfigLogItem {
    private:
        std::string filePath;
        RS_LOG_TANK_TYPE type;
    public:
        RSConfigLogItem() {
            filePath = DEFAULT_LOG_TANK_FILE_PATH;
            type = DEFAULT_LOG_TANK_TYPE;
        };

        ~RSConfigLogItem() = default;

    public:
        int initialize(const rapidjson::Value &logVal);

        std::string &get_path() { return filePath; }

        RS_LOG_TANK_TYPE get_type() { return type; }
    };

    enum RS_SERVER_TYPE {
        RS_SERVER_TYPE_RTMP = 0
    };

    static const RS_SERVER_TYPE DEFAULT_SERVER_TYPE = RS_SERVER_TYPE_RTMP;
    static const uint32_t DEFAULT_SERVER_PORT = 1935;
    static const char *DEFAULT_SERVER_NAME = "default-rtmp-server";

    class RSConfigServerItem {
    private:
        uint32_t listenPort;
        RS_SERVER_TYPE serverType;
        std::string name;
    public:
        RSConfigServerItem() {
            listenPort = DEFAULT_SERVER_PORT;
            serverType = DEFAULT_SERVER_TYPE;
            name = DEFAULT_SERVER_NAME;
        };

        virtual ~RSConfigServerItem() = default;

    public:
        int initialize(const rapidjson::Value &obj);

        std::string &get_server_name() { return name; }

        int get_server_port() { return listenPort; }

        RS_SERVER_TYPE get_server_type() { return serverType; }
    };

    class RsConfig {
    private:
        std::string conf;

        RSConfigLogItem log;

        std::map<std::string, std::shared_ptr<RSConfigServerItem>> servers;

    private:
        RsConfig() = default;

    public:

        RsConfig(RsConfig const &) = delete;

        RsConfig &operator=(RsConfig const &) = delete;

        virtual ~RsConfig() = default;

    private:

        int do_parse_configure_file(const rapidjson::Document &doc);

    public:
        int initialize(const std::string &path);

    public:
        int get_rtmp_listen(std::string server);

        RS_LOG_TANK_TYPE get_log_tank();

    public:
        static std::shared_ptr<RsConfig> get_instance() {
            static std::shared_ptr<RsConfig> ins(new RsConfig());
            return ins;
        };
    };
}
