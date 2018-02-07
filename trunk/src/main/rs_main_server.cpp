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

#include "rs_common.h"
#include "rs_module_server.h"

// TODO:FIXME: need to implement signal system

/**
 * Output the server info
 */
void _server_info() {
    // TODO:FIXME: implement log system
    std::cout << "version: " << VERSION_MAJOR << "." << VERSION_MINOR << "."
              << VERSION_REVISION << "." << VERSION_BUILD << std::endl;
    std::cout << "Copyright (c) 2016 ME_Kun_Han hanvskun@hotmail.com" << std::endl;
}

int main(int argc, char *argv[]) {
    _server_info();

    return RsServer::getInstance()->run();
}
