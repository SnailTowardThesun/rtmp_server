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

#include <math.h>
#include <sys/time.h>
#include <rs_common_utility.h>

std::string rs_get_random(int size) {
    std::string rd;

    for (int i = 0; i < size; ++i) {
        rd.push_back(rand() % 128 + 32);
    }

    return rd;
}

int64_t rs_get_system_time_ms() {
    int64_t now_us = 0;

    timeval now;
    if (gettimeofday(&now, NULL) < 0) {
        return -1;
    }

    now_us = ((int64_t) now.tv_sec) * 1000 * 1000 + (int64_t) now.tv_usec;
    return now_us / 1000;
}

std::string rs_get_local_ip() {
    // TODO:FIXME: implement this function
    return "127.0.0.1";
}
