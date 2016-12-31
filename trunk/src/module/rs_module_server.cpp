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
#include "rs_module_server.h"
#include "rs_module_io.h"
using namespace std;

RSRtmpServer::RSRtmpServer() : _socket(new RSuvSocket())
{

}

RSRtmpServer::~RSRtmpServer()
{
}

int RSRtmpServer::initialize()
{
    int ret = ERROR_SUCCESS;

    if ((ret = _socket->initialize(RTMP_DEFAULTPORT)) != ERROR_SUCCESS) {
        cout << "initialize listen socket failed. ret=" << ret << endl;
        return ret;
    }

    return ret;
}


RSServer::RSServer()
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

    // initialize the all servers
    if ((ret = _rtmp_server.initialize()) != ERROR_SUCCESS) {
        cout << "initialize the rtmp server failed. ret=" << ret << endl;
        return ret;
    }

    return uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

void RSServer::exit()
{
    // stop all loop
    uv_stop(uv_default_loop());
    uv_loop_close(uv_default_loop());
}
