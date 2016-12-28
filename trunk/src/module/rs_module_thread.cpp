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

#include <cstdlib>
#include "rs_module_thread.h"
#include "rs_common.h"
using namespace std;

RSLocker::RSLocker() : _locker(nullptr)
{
    _locker = new uv_mutex_t();
    if (uv_mutex_init(_locker) == 0) {
        uv_mutex_lock(_locker);
    } else {
        uv_mutex_destroy(_locker);
        rs_freep(_locker);
    }
}

RSLocker::~RSLocker()
{
    if (_locker != nullptr) {
        uv_mutex_unlock(_locker);
    }
    uv_mutex_destroy(_locker);
    rs_freep(_locker);
}

BaseThread::BaseThread() : _thread(nullptr)
{

}

BaseThread::~BaseThread()
{
    stop();
    rs_freep(_thread);
}

void BaseThread::cycle(void *param)
{
    BaseThread *th = static_cast<BaseThread*>(param);
    if (th) {
        th->do_cycle();
    }
}

int BaseThread::start()
{
    int ret = ERROR_SUCCESS;

    if (_thread != nullptr && (ret = stop()) != ERROR_SUCCESS) {
        // TODO:FIXME implement log module
        cout<< "fail to stop thread" << endl;
        return ret;
    }

    _thread = new uv_thread_t();
    if ((ret = uv_thread_create(_thread, cycle, this) != ERROR_SUCCESS)) {
        // TODO:FIXME implement log module
        cout << "create uv thread failed. ret=" << ret << endl;
        return ret;
    }

    return ret;
}

int BaseThread::stop()
{
    int ret = ERROR_SUCCESS;

    if (_thread == nullptr) {
        return ret;
    }

    if ((ret = uv_thread_join(_thread)) != ERROR_SUCCESS) {
        // TODO:FIXME implement log module
        cout << "join uv thread failed. ret=" << ret << endl;
        return ret;
    }
    rs_freep(_thread);

    return ret;
}

int BaseThread::do_cycle()
{
    cout << "this is base thread do cycle function" << endl;

    return ERROR_SUCCESS;
}