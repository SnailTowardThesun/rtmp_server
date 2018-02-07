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
#include <unistd.h>
#include <sys/time.h>
#include <cstdarg>
#include "rs_kernel_context.h"

namespace rs_log {
#define RS_LOG_MAX_LENGTH 4096

#define RS_LOG_LEVEL_INFO "info"
#define RS_LOG_LEVEL_VERBOSE "verbose"
#define RS_LOG_LEVEL_TRACE "trace"
#define RS_LOG_LEVEL_WARN "warn"
#define RS_LOG_LEVEL_ERROR "error"

    IRSLog::IRSLog() : msg(nullptr) {
        pid = getpid();
        msg = new char[4096];
    }

    IRSLog::~IRSLog() {
        rs_free_p(msg);
    }

    void IRSLog::info(IRsReaderWriter *io, const char *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        auto size = vsnprintf(msg, RS_LOG_MAX_LENGTH, fmt, ap);
        va_end(ap);

        auto cid = RsConnContext::getInstance()->get_id(io);

        log(cid, RS_LOG_LEVEL_INFO, std::string(msg, static_cast<unsigned long>(size)));
    }

    void IRSLog::verbose(IRsReaderWriter *io, const char *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        auto size = vsnprintf(msg, RS_LOG_MAX_LENGTH, fmt, ap);
        va_end(ap);

        auto cid = RsConnContext::getInstance()->get_id(io);

        log(cid, RS_LOG_LEVEL_VERBOSE, std::string(msg, static_cast<unsigned long>(size)));
    }

    void IRSLog::trace(IRsReaderWriter *io, const char *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        auto size = vsnprintf(msg, RS_LOG_MAX_LENGTH, fmt, ap);
        va_end(ap);

        auto cid = RsConnContext::getInstance()->get_id(io);

        log(cid, RS_LOG_LEVEL_TRACE, std::string(msg, static_cast<unsigned long>(size)));
    }

    void IRSLog::warn(IRsReaderWriter *io, const char *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        auto size = vsnprintf(msg, RS_LOG_MAX_LENGTH, fmt, ap);
        va_end(ap);

        auto cid = RsConnContext::getInstance()->get_id(io);

        log(cid, RS_LOG_LEVEL_WARN, std::string(msg, static_cast<unsigned long>(size)));
    }

    void IRSLog::error(IRsReaderWriter *io, const char *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        auto size = vsnprintf(msg, RS_LOG_MAX_LENGTH, fmt, ap);
        va_end(ap);

        auto cid = RsConnContext::getInstance()->get_id(io);

        log(cid, RS_LOG_LEVEL_ERROR, std::string(msg, static_cast<unsigned long>(size)));
    }

    void RSConsoleLog::log(int64_t cid, std::string level, std::string message) {
        std::stringstream ss;
        ss << "[" << pid << "]";
        ss << "[" << cid << "]";

        // time
        timeval tv = {0, 0};
        struct tm *time;
        if (gettimeofday(&tv, nullptr) == -1) {
            return;
        }

        if ((time = localtime(&tv.tv_sec)) == nullptr) {
            return;
        }

        // [2017-08-01 14:23:32.893]
        ss << "[" << 1900 + time->tm_year << "-" << 1 + time->tm_mon << "-"
           << time->tm_mday
           << " ";
        ss << time->tm_hour << ":" << time->tm_min << ":" << time->tm_sec << "."
           << (int) (tv.tv_usec / 1000.0) << "]";

        ss << "[" << level << "]";
        ss << ": " << message;

        std::cout << ss.str() << std::endl << std::flush;
    }
}
