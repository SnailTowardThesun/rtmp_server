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

#include "gtest/gtest.h"
#include "rs_module_log.h"
#include "rs_kernel_context.h"

using namespace std;
using namespace rs_log;

class MockRsLog : public IRSLog {
public:
    int64_t _cid;
    string _level;
    string _msg;
public:
    MockRsLog() = default;

    ~MockRsLog() = default;

public:
    virtual void log(int64_t cid, string level, string message) {
        _cid = cid;
        _level = level;
        _msg = message;
    }
};

TEST (RsLog, IRSLog_info) {
    RsTCPSocketIO *io = new RsTCPSocketIO();
    RsConnContext::getInstance()->regist(io);
    MockRsLog *log = new MockRsLog();

    log->info(io, "this is for info test %d", 10);
    ASSERT_EQ(log->_cid, RsConnContext::getInstance()->get_id(io));
    ASSERT_EQ(log->_level, "info");
    ASSERT_EQ(log->_msg, "this is for info test 10");

    RsConnContext::getInstance()->unregist(io);
    rs_free_p(io);
    rs_free_p(log);
}

TEST (RsLog, IRSLog_verbose) {
    RsTCPSocketIO *io = new RsTCPSocketIO();
    RsConnContext::getInstance()->regist(io);
    MockRsLog *log = new MockRsLog();

    log->verbose(io, "this is for verbose test %d", 10);
    ASSERT_EQ(log->_cid, RsConnContext::getInstance()->get_id(io));
    ASSERT_EQ(log->_level, "verbose");
    ASSERT_EQ(log->_msg, "this is for verbose test 10");

    RsConnContext::getInstance()->unregist(io);
    rs_free_p(io);
    rs_free_p(log);
}

TEST (RsLog, IRSLog_trace) {
    RsTCPSocketIO *io = new RsTCPSocketIO();
    RsConnContext::getInstance()->regist(io);
    MockRsLog *log = new MockRsLog();

    log->trace(io, "this is for trace test %d", 10);
    ASSERT_EQ(log->_cid, RsConnContext::getInstance()->get_id(io));
    ASSERT_EQ(log->_level, "trace");
    ASSERT_EQ(log->_msg, "this is for trace test 10");

    RsConnContext::getInstance()->unregist(io);
    rs_free_p(io);
    rs_free_p(log);
}

TEST (RsLog, IRSLog_warn) {
    RsTCPSocketIO *io = new RsTCPSocketIO();
    RsConnContext::getInstance()->regist(io);
    MockRsLog *log = new MockRsLog();

    log->warn(io, "this is for warn test %d", 10);
    ASSERT_EQ(log->_cid, RsConnContext::getInstance()->get_id(io));
    ASSERT_EQ(log->_level, "warn");
    ASSERT_EQ(log->_msg, "this is for warn test 10");

    RsConnContext::getInstance()->unregist(io);
    rs_free_p(io);
    rs_free_p(log);
}

TEST (RsLog, IRSLog_error) {
    RsTCPSocketIO *io = new RsTCPSocketIO();
    RsConnContext::getInstance()->regist(io);
    MockRsLog *log = new MockRsLog();

    log->error(io, "this is for error test %d", 10);
    ASSERT_EQ(log->_cid, RsConnContext::getInstance()->get_id(io));
    ASSERT_EQ(log->_level, "error");
    ASSERT_EQ(log->_msg, "this is for error test 10");

    RsConnContext::getInstance()->unregist(io);
    rs_free_p(io);
    rs_free_p(log);
}
