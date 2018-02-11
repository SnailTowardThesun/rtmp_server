/*
MIT License

Copyright (c) 2017 ME_Kun_Han

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

#include <rs_module_log.h>
#include <getopt.h>
#include <rs_module_config.h>
#include "rs_module_server.h"

// TODO:FIXME: need to implement signal system

/**
 * useage
 */
void usage() {
    printf("useage:\n");
    printf("\t-c\tpath of configure file. should no be empty\n");
    printf("example:\n");
    printf("\t./objs/rs_server -c conf/default.json\n");
    ::exit(-1);
}

/**
 * Output the server info
 */
void _server_info() {
    printf("rs-server version: %d.%d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR,
           VERSION_REVISION, VERSION_BUILD);
    printf("Copyright (c) 2016 ME_Kun_Han hanvskun@hotmail.com\n");
}

int main(int argc, char *argv[]) {
    int ret = ERROR_SUCCESS;
    // print server info
    _server_info();

    // parse params
    std::string configure_file;
    {
        char ch = 0;
        while ((ch = static_cast<char>(getopt(argc, argv, "c:"))) != -1) {
            switch (ch) {
                case 'c':
                    configure_file = optarg;
                    rs_info(nullptr, "the path of configure file is %s",
                            configure_file.c_str());
                    break;
                default:
                    break;
            }
        }
    }

    if (configure_file.empty()) {
        usage();
    }

    // initialize configure
    auto config = rs_config::RsConfig::get_instance();
    if ((ret = config->initialize(configure_file)) !=
        ERROR_SUCCESS) {
        rs_error(nullptr, "initialize configure failed. ret=%d", ret);
        return ret;
    }

    // initialize log
    switch (config->get_log_tank()) {
        case rs_config::RS_LOG_TANK_TYPE_CONSOLE:
            rs_log::RSLogManager::get_instance()->change_log_interface(new rs_log::RSConsoleLog());
            break;
        default:
            ::exit(0);
    }

    rs_info(nullptr, "ready to run!");
    return RsServer::get_instance()->run();
}
