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

#include <rs_kernel_context.h>

RsConnContext::RsConnContext() {
    DEFAULT_ID = 100;
    current_ID = 200;
}

RsConnContext::~RsConnContext() {
}

int32_t RsConnContext::do_register(IRsIO *io) {
    int32_t ret = ERROR_SUCCESS;

    if (connection_ids.find(io) != connection_ids.end()) {
        ret = ERROR_CONTEXT_CONN_ID_EXISTS;
        return ret;
    }

    connection_ids[io] = current_ID++;

    return ret;
}


void RsConnContext::do_deregister(IRsIO *io) {
    connection_ids.erase(io);
}

uint64_t RsConnContext::get_id(IRsIO *io) {
    auto id = connection_ids.find(io);
    if (id == connection_ids.end()) {
        return DEFAULT_ID;
    }

    return id->second;
}

static RsConnContext *_rs_context = nullptr;

RsConnContext *RsConnContext::getInstance() {
    if (_rs_context == nullptr) {
        _rs_context = new RsConnContext();
    }

    return _rs_context;
}
