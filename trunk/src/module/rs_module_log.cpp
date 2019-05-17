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
#include <cstdarg>
#include <time.h>
#include <sys/time.h>
#include "rs_kernel_context.h"

const char *RS_LOG_LEVEL_INFO = "info";
const char *RS_LOG_LEVEL_VERBOSE = "verbose";
const char *RS_LOG_LEVEL_TRACE = "trace";
const char *RS_LOG_LEVEL_WARN = "warn";
const char *RS_LOG_LEVEL_ERROR = "error";

const int32_t RS_LOG_MAX_LENGTH = 4096;

namespace rs_log {

    RsLogManager::RsLogManager() : msg(nullptr), log_interface(nullptr) {
        msg = std::shared_ptr<char>(new char[RS_LOG_MAX_LENGTH]);
    }

    void RsLogManager::info(IRsIO *io, const char *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        auto size = vsnprintf(msg.get(), RS_LOG_MAX_LENGTH, fmt, ap);
        va_end(ap);

        auto cid = RsConnContext::getInstance()->get_id(io);

        log(cid, RS_LOG_LEVEL_INFO, std::string(msg.get(), static_cast<unsigned long>(size)));
    }

    void RsLogManager::verbose(IRsIO *io, const char *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        auto size = vsnprintf(msg.get(), RS_LOG_MAX_LENGTH, fmt, ap);
        va_end(ap);

        auto cid = RsConnContext::getInstance()->get_id(io);

        log(cid, RS_LOG_LEVEL_VERBOSE,
            std::string(msg.get(), static_cast<unsigned long>(size)));
    }

    void RsLogManager::trace(IRsIO *io, const char *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        auto size = vsnprintf(msg.get(), RS_LOG_MAX_LENGTH, fmt, ap);
        va_end(ap);

        auto cid = RsConnContext::getInstance()->get_id(io);

        log(cid, RS_LOG_LEVEL_TRACE, std::string(msg.get(), static_cast<unsigned long>(size)));
    }

    void RsLogManager::warn(IRsIO *io, const char *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        auto size = vsnprintf(msg.get(), RS_LOG_MAX_LENGTH, fmt, ap);
        va_end(ap);

        auto cid = RsConnContext::getInstance()->get_id(io);

        log(cid, RS_LOG_LEVEL_WARN, std::string(msg.get(), static_cast<unsigned long>(size)));
    }

    void RsLogManager::error(IRsIO *io, const char *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        auto size = vsnprintf(msg.get(), RS_LOG_MAX_LENGTH, fmt, ap);
        va_end(ap);

        auto cid = RsConnContext::getInstance()->get_id(io);

        log(cid, RS_LOG_LEVEL_ERROR, std::string(msg.get(), static_cast<unsigned long>(size)));
    }

    void RsConsoleLog::log(int64_t cid, std::string level, std::string message) {
        std::stringstream ss;

        // time
        timeval tv = {0, 0};
        if (gettimeofday(&tv, nullptr) == -1) {
            return;
        }

        auto ptr_time = gmtime(&tv.tv_sec);

        // [2017-08-01 14:23:32.893]
        ss << "[" << 1900 + ptr_time->tm_year << "-" << 1 + ptr_time->tm_mon << "-"
           << ptr_time->tm_mday
           << " ";
        ss << ptr_time->tm_hour << ":" << ptr_time->tm_min << ":" << ptr_time->tm_sec << "."
           << (int) (tv.tv_usec / 1000.0) << "]";

        ss << "[" << get_pid() << "]";
        ss << "[" << cid << "]";


        ss << "[" << level << "]";
        ss << ": " << message;

        printf("%s\n", ss.str().c_str());
    }
}
