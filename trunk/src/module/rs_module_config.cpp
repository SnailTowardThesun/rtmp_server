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

#include <rs_module_config.h>
#include <rs_module_log.h>
#include <rapidjson/filereadstream.h>
#include <fstream>

namespace rs_config {

    int RsConfigLogItem::initialize(const rapidjson::Value &obj) {

        int ret = ERROR_SUCCESS;

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
            type = RS_LOG_TANK_TYPE_FILE;
        } else if (tankStr == std::string("console")) {
            type = RS_LOG_TANK_TYPE_CONSOLE;
        } else {
            ret = ERROR_CONFIGURE_SYNTAX_INVALID;
            rs_error(nullptr, "configure: the tank type is invalid. ret=%d", ret);
            return ret;
        }

        if (type == RS_LOG_TANK_TYPE_CONSOLE) {
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

        filePath = fileVal.GetString();

        return ret;
    }

    RsConfigBaseServer *
    RsConfigBaseServer::create_server_config(const rapidjson::Value &obj, int &ret) {
        if (!obj.IsObject()) {
            ret = ERROR_CONFIGURE_SYNTAX_INVALID;
            rs_error(nullptr, "configure: server item should be object. ret=%d", ret);
            return nullptr;
        }

        // get server type
        const rapidjson::Value &typeVal = obj["type"];
        if (!typeVal.IsString()) {
            ret = ERROR_CONFIGURE_SYNTAX_INVALID;
            rs_error(nullptr,
                     "configure: type of type in server item should be string. ret=%d",
                     ret);
            return nullptr;
        }

        std::string typeStr = typeVal.GetString();
        RsConfigBaseServer *server = nullptr;
        if (typeStr == "rtmp") {
            server = new RsConfigRTMPServer();
            server->type = RS_SERVER_TYPE_RTMP;
        }

        // server name
        if (!obj.HasMember("name")) {
            ret = ERROR_CONFIGURE_SYNTAX_INVALID;
            rs_error(nullptr, "configure: no name in server item. ret=%d", ret);
            return nullptr;
        }
        const rapidjson::Value &nameVal = obj["name"];
        if (!nameVal.IsString()) {
            ret = ERROR_CONFIGURE_SYNTAX_INVALID;
            rs_error(nullptr,
                     "configure: type of name in server item should be string. ret=%d",
                     ret);
            return nullptr;
        }
        server->name = nameVal.GetString();

        // port
        if (!obj.HasMember("listen")) {
            ret = ERROR_CONFIGURE_SYNTAX_INVALID;
            rs_error(nullptr, "configure: no listen in server item. ret=%d", ret);
            return nullptr;
        }

        const rapidjson::Value &listenVal = obj["listen"];
        if (!listenVal.IsInt()) {
            ret = ERROR_CONFIGURE_SYNTAX_INVALID;
            rs_error(nullptr,
                     "configure: type of listen in server item should be integer. ret=%d",
                     ret);
            return nullptr;
        }

        server->listenPort = static_cast<uint32_t>(listenVal.GetInt());

        if ((ret = server->initialize(obj)) != ERROR_SUCCESS) {
            rs_error(nullptr, "initialize server=%s, type=%s, port=%d failed. ret=%d",
                     server->name.c_str(), typeStr.c_str(), server->listenPort, ret);
        }

        return server;
    }

    int RsConfigRTMPServer::initialize(const rapidjson::Value &obj) {
        int ret = ERROR_SUCCESS;

        return ret;
    }

    int RsConfig::do_parse_configure_file(const rapidjson::Document &doc) {
        int ret = ERROR_SUCCESS;

        // parse log related
        if (doc.HasMember("log")) {
            if ((ret = log.initialize(doc["log"])) != ERROR_SUCCESS) {
                rs_error(nullptr, "initialize log part in configure file failed. ret=%d",
                         ret);
                return ret;
            }
        }

        // parse server related
        if (!doc.HasMember("server")) {
            ret = ERROR_CONFIGURE_NO_SERVER_PARTS;
            rs_error(nullptr, "no server parts in configure. ret=%d", ret);
            return ret;
        }

        const rapidjson::Value &arrayVal = doc["server"];
        if (!arrayVal.IsArray()) {
            ret = ERROR_CONFIGURE_SYNTAX_INVALID;
            rs_error(nullptr, "configure: the server should be array. ret=%d", ret);
            return ret;
        }

        auto array = arrayVal.GetArray();
        for (auto i = 0; i < array.Size(); ++i) {
            auto newServer = std::shared_ptr<RsConfigBaseServer>(
                    RsConfigBaseServer::create_server_config(array[i], ret));
            if (ret != ERROR_SUCCESS) {
                rs_error(nullptr, "parse one server item failed. ret=%d", ret);
                return ret;
            }
            servers.insert(
                    std::pair<std::string, std::shared_ptr<RsConfigBaseServer>>(
                            newServer->get_server_name(),
                            newServer));
        }
        return ret;
    }

    int RsConfig::initialize(const std::string &path) {
        int ret = ERROR_SUCCESS;

        conf = path;

        rapidjson::Document doc;
        {
            std::FILE *file = std::fopen(path.c_str(), "rb");
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

    RS_LOG_TANK_TYPE RsConfig::get_log_tank() {
        return log.get_type();
    }

    const std::string &RsConfig::get_log_file_path() {
        return log.get_path();
    }
}