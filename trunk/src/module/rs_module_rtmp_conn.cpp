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

#include "rs_module_rtmp_conn.h"
#include "rs_protocol_rtmp.h"

using namespace std;

RsRtmpConn::RsRtmpConn() : io(nullptr) {

}

RsRtmpConn::~RsRtmpConn() {
    rs_free_p(io);
}

int RsRtmpConn::simple_handshake_with_server() {
    assert(io != nullptr);

    int ret = ERROR_SUCCESS;

    // create c0c1
    c0c1 c;
    if ((ret = c.initialize()) != ERROR_SUCCESS) {
        cout << "create c0c1 failed. ret=" << ret << endl;
        return ret;
    }

    // send c0c1
    string str = c.dump();
    if ((ret = io->write(str, (int) str.length())) != ERROR_SUCCESS) {
        cout << "send c0c1 failed. ret=" << ret << endl;
        return ret;
    }

    // read s0s1s2
    string s0s1s2;
    if ((ret = io->read(s0s1s2, 3073)) != ERROR_SUCCESS) {
        cout << "read s0s1s2 package failed. ret=" << ret << endl;
        return ret;
    }

    s0s1 s0s11;
    if ((ret = s0s11.initialize(s0s1s2)) != ERROR_SUCCESS) {
        cout << "convert s0s1s2 into s0s1 failed. ret=" << ret << endl;
        return ret;
    }

    c2 c211;
    if ((ret = c211.initialize(s0s11.timestamp, s0s1s2.substr(1538))) != ERROR_SUCCESS) {
        return ret;
    }

    str = c211.dump();
    if ((ret = io->write(str, (int) str.length())) != ERROR_SUCCESS) {
        cout << "send c2 failed. ret=" << ret << endl;
        return ret;
    }

    return ret;
}

int RsRtmpConn::simple_handshake_with_client() {
    assert(io != nullptr);

    int ret = ERROR_SUCCESS;

    // read c0c1
    string str;
    if ((ret = io->read(str, 1538)) != ERROR_SUCCESS) {
        cout << "get c0c1 failed. ret=" << ret << endl;
        return ret;
    }

    c0c1 c;
    if ((ret = c.initialize(str)) != ERROR_SUCCESS) {
        cout << "convert buffer into c0c1 failed. ret=" << ret << endl;
        return ret;
    }

    // send s0s1s2
    s0s1 s0s11;
    if ((ret = s0s11.initialize()) != ERROR_SUCCESS) {
        cout << "create s0s1 failed. ret=" << ret << endl;
        return ret;
    }

    s2 s21;
    if ((ret = s21.initialize(c.timestamp, c.random_data)) != ERROR_SUCCESS) {
        cout << "create c2 failed. ret=" << ret << endl;
        return ret;
    }

    str = s0s11.dump() + s21.dump();
    if ((ret = io->write(str, (int) str.length())) != ERROR_SUCCESS) {
        cout << "send s0s1s2 failed. ret=" << ret << endl;
        return ret;
    }

    // get s2
    if ((ret = io->read(str, 1536)) != ERROR_SUCCESS) {
        cout << "get s2 failed. ret=" << ret << endl;
        return ret;
    }

    return ret;
}

int RsRtmpConn::complex_handshake_with_server() {
    // TODO:FIXME:implement this method
    return ERROR_SUCCESS;
}

int RsRtmpConn::complex_handshake_witout_client() {
    // TODO:FIXME:implement this method
    return ERROR_SUCCESS;
}

int RsRtmpConn::connect() {
    // TODO:FIXME:implement this method
    return ERROR_SUCCESS;
}

int RsRtmpConn::play_stream() {
    // TODO:FIXME:implement this method
    return ERROR_SUCCESS;
}

int RsRtmpConn::publish_stream() {
    // TODO:FIXME:implement this method
    return ERROR_SUCCESS;
}
