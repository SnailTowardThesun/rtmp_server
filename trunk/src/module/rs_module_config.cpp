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

#include "rs_module_config.h"
#include "rs_module_log.h"
#include <rapidjson/filereadstream.h>
#include <fstream>

namespace rs_config {

    int RsConfig::do_parse_log_related(const rapidjson::Value &obj) {
        int ret = ERROR_SUCCESS;

        RS_LOG_TANK_TYPE tankType = RS_LOG_TANK_TYPE_UNKNOWN;
        std::string logFilePath;

        if (!obj.IsObject()) {
            ret = ERROR_CONFIGURE_SYNTAX_INVALID;
            rs_error(nullptr, "configure: format of log is invalid. ret=%d", ret);
            return ret;
        }

        if (!obj.HasMember("tank")) {
            ret = ERROR_CONFIGURE_SYNTAX_INVALID;
            rs_error(nullptr, "configure: no tank item in log part. ret=%d", ret);
            return ret;
        }

        const rapidjson::Value &tankVal = obj["tank"];
        if (!tankVal.IsString()) {
            ret = ERROR_CONFIGURE_SYNTAX_INVALID;
            rs_error(nullptr, "configure: tank item is invalid. ret=%d", ret);
            return ret;
        }

        auto tankStr = std::string(tankVal.GetString());
        if (tankStr == std::string("file")) {
            tankType = RS_LOG_TANK_TYPE_FILE;
        } else if (tankStr == std::string("console")) {
            tankType = RS_LOG_TANK_TYPE_CONSOLE;
        } else {
            ret = ERROR_CONFIGURE_SYNTAX_INVALID;
            rs_error(nullptr, "configure: the tank type is invalid. ret=%d", ret);
            return ret;
        }

        if (tankType == RS_LOG_TANK_TYPE_CONSOLE) {
            log = LogConfig("", tankType);
            return ret;
        }

        if (!obj.HasMember("file")) {
            ret = ERROR_CONFIGURE_SYNTAX_INVALID;
            rs_error(nullptr, "configure: no file item in log part. ret=%d", ret);
            return ret;
        }

        const rapidjson::Value &fileVal = obj["file"];
        if (!fileVal.IsString()) {
            ret = ERROR_CONFIGURE_SYNTAX_INVALID;
            rs_error(nullptr, "configure: file item is invalid. ret=%d", ret);
            return ret;
        }

        log = LogConfig(fileVal.GetString(), tankType);

        return ret;
    }

    int RsConfig::do_parse_server_related(const rapidjson::Value &array) {
        int ret = ERROR_SUCCESS;

        return ret;
    }

    int RsConfig::do_parse_configure_file(const rapidjson::Document &doc) {
        int ret = ERROR_SUCCESS;

        // parse log related
        if (doc.HasMember("log")) {
            if ((ret = do_parse_log_related(doc["log"])) != ERROR_SUCCESS) {
                rs_error(nullptr, "do parse log part in configure file failed. ret=%d",
                         ret);
                return ret;
            }
        } else {
            // use default value
            log = LogConfig(DEFAULT_LOG_TANK_FILE_PATH, DEFAULT_LOG_TANK_TYPE);
        }

        // parse server related
        if (doc.HasMember("server")) {
            if ((ret = do_parse_server_related(doc["server"])) != ERROR_SUCCESS) {
                rs_error(nullptr, "parse server part in configure file failed. ret=%d",
                         ret);
                return ret;
            }
        } else {
            ret = ERROR_CONFIGURE_NO_SERVER_PARTS;
            rs_error(nullptr, "no server parts in configure. ret=%d", ret);
            return ret;
        }

        return ret;
    }

    int RsConfig::initialize(const std::string &path) {
        int ret = ERROR_SUCCESS;

        conf = path;

        rapidjson::Document doc;
        {
            auto file = std::fopen(conf.c_str(), "rb");
            if (file == nullptr) {
                rs_error(nullptr, "open configure file failed. errno=%d", errno);
                ret = ERROR_CONFIGURE_OPEN_FILE_FAILED;
                return ret;
            }
            char buffer[65536] = {0};
            rapidjson::FileReadStream fileReadStream(file, buffer, sizeof(buffer));
            doc.ParseStream(fileReadStream);
            std::fclose(file);
        }

        if ((ret = do_parse_configure_file(doc)) != ERROR_SUCCESS) {
            rs_error(nullptr, "parse configure file failed. ret=%d", ret);
            return ret;
        }

        return ret;
    }

    int RsConfig::get_rtmp_listen(std::string name) {
        // TODO:FIXME: implement this function
        return RTMP_DEFAULT_PORT;
    }
}