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

#include "rs_common.h"
#include "rs_module_server.h"
#include <signal.h>

/**
 * add signal system
 */
 // TODO:FIXME: need to us libuv signal
 static void signal_handle(int sig)
{
    if (sig == SIGINT || sig == SIGQUIT || sig == SIGTERM) {
        RSServer::getInstance()->exit();
        std::cout << "terminate the program successfully" << std::endl;
    }
}

/**
 * Output the server info
 */
void _server_info()
{
    // TODO:FIXME: implemet log system
    std::cout << "version: " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_REVISION << std::endl;
    std::cout << "Copyright (c) 2016 ME_Kun_Han hanvskun@hotmail.com" << std::endl;
    std::cout << "author : " << AUTHOR << std::endl;
}

int main(int argc, char* argv[])
{
    _server_info();

    // initialize the signal system
    if (signal(SIGINT, signal_handle) == SIG_ERR) {
        exit(ERROR_SIGNAL_INITIALIZE);
    }
    if (signal(SIGTERM, signal_handle) == SIG_ERR) {
        exit(ERROR_SIGNAL_INITIALIZE);
    }
    if (signal(SIGQUIT, signal_handle) == SIG_ERR) {
        exit(ERROR_SIGNAL_INITIALIZE);
    }

    return RSServer::getInstance()->run();
}
