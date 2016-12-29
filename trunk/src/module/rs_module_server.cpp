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

#include <zconf.h>
#include "rs_common.h"
#include "rs_module_server.h"

RSServer::RSServer() : is_exit(false)
{
}

RSServer::~RSServer()
{
}

static RSServer* _instance = nullptr;
RSServer* RSServer::getInstance()
{
    if (_instance == nullptr) {
        _instance = new RSServer();
    }
    return _instance;
}

int RSServer::run()
{
    int ret = ERROR_SUCCESS;

    if ((ret = start()) != ERROR_SUCCESS) {
        std::cout << "start server thread failed. ret=" << ret << std::endl;
        return ret;
    }

    while (!is_exit) {
        usleep(10 * 1000);
    }

    if ((ret = stop()) != ERROR_SUCCESS) {
        std::cout << "stop server thread failed. ret=" << ret << std::endl;
    }

    return ret;
}

int RSServer::do_cycle()
{
    std::cout << "this is server run" << std::endl;
    return ERROR_SUCCESS;
}

void RSServer::exit()
{
    is_exit = true;
}