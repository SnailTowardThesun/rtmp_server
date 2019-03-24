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

#ifndef RS_KERNEL_CONNECTION_HEADER_H_
#define RS_KERNEL_CONNECTION_HEADER_H_

class RsConnection {
protected:
    typedef enum {
        rs_connection_uninitialized = 0,
        rs_connection_running,
        rs_connection_stopped
    } RS_CONNECTION_STATUS;

private:
    RS_CONNECTION_STATUS _status;

public:
    RsConnection() : _status(rs_connection_uninitialized) {};

    virtual ~RsConnection() { _status = rs_connection_stopped; };

public:
    void change_status(RS_CONNECTION_STATUS status) {
        _status = status;
    };

    bool is_initialized() { return _status == rs_connection_uninitialized; }

    bool is_running() { return _status == rs_connection_running; }

    bool is_stopped() { return _status == rs_connection_stopped; }

public:

    virtual int initialize(IRsIO *io) = 0;

    virtual void update_status() = 0;
};

#endif