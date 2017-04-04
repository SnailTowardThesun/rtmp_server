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

#include "rs_module_log.h"
#include <sstream>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include "rs_kernel_context.h"
using namespace std;

#define RS_LOG_MAX_LENGTH 4096

#define RS_LOG_LEVEL_INFO "info"
#define RS_LOG_LEVEL_VERBOSE "verbose"
#define RS_LOG_LEVEL_TRACE "trace"
#define RS_LOG_LEVEL_WARN "warn"
#define RS_LOG_LEVEL_ERROR "error"

IRSLog::IRSLog() : msg(nullptr)
{
    pid = getpid();
    msg = new char[4096];
}

IRSLog::~IRSLog()
{
    rs_free_p(msg);
}

void IRSLog::info(IRsReaderWriter* io, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    auto size = vsnprintf(msg, RS_LOG_MAX_LENGTH, fmt, ap);
    va_end(ap);

    auto cid = RsConnContext::getInstance()->get_id(io);

    log(cid, RS_LOG_LEVEL_INFO, string(msg, size));
}

void IRSLog::verbose(IRsReaderWriter* io, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    auto size = vsnprintf(msg, RS_LOG_MAX_LENGTH, fmt, ap);
    va_end(ap);

    auto cid = RsConnContext::getInstance()->get_id(io);

    log(cid, RS_LOG_LEVEL_VERBOSE, string(msg, size));
}

void IRSLog::trace(IRsReaderWriter* io, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    auto size = vsnprintf(msg, RS_LOG_MAX_LENGTH, fmt, ap);
    va_end(ap);

    auto cid = RsConnContext::getInstance()->get_id(io);

    log(cid, RS_LOG_LEVEL_TRACE, string(msg, size));
}

void IRSLog::warn(IRsReaderWriter* io, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    auto size = vsnprintf(msg, RS_LOG_MAX_LENGTH, fmt, ap);
    va_end(ap);

    auto cid = RsConnContext::getInstance()->get_id(io);

    log(cid, RS_LOG_LEVEL_WARN, string(msg, size));
}

void IRSLog::error(IRsReaderWriter* io, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    auto size = vsnprintf(msg, RS_LOG_MAX_LENGTH, fmt, ap);
    va_end(ap);

    auto cid = RsConnContext::getInstance()->get_id(io);

    log(cid, RS_LOG_LEVEL_ERROR, string(msg, size));
}

RSConsoleLog::RSConsoleLog()
{
}

RSConsoleLog::~RSConsoleLog()
{
}

void RSConsoleLog::log(int64_t cid, string level, string message)
{
    stringstream ss;
    ss << "[" << pid << "]";
    ss << "[" << cid << "]";
    ss << "[" << level << "]";
    ss << ": " << message << endl;

    cout << ss.str();
}
