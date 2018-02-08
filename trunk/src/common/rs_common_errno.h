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

/**
 * The Error Number for rtmp server.
 * 0 for success
 * Begin with 1000
 */

#ifndef RS_COMMON_ERRNO_H
#define RS_COMMON_ERRNO_H

#define ERROR_SUCCESS 0

// error number for signal
#define ERROR_SIGNAL_INITIALIZE 1000


// error number for protocol
#define ERROR_RTMP_PROTOCOL_VERSION_ERROR 2000
#define ERROR_RTMP_PROTOCOL_C0C1_ZERO_NOT_ZERO 2001
#define ERROR_RTMP_PROTOCOL_C0C1_LENGTH_ERROR 2002
#define ERROR_RTMP_PROTOCOL_CHUNK_MESSAGE_FMT_ERROR 2003
#define ERROR_RTMP_PROTOCOL_AMF0_DECODE_ERROR 2004


#define ERROR_CONTEXT_CONN_ID_EXISTS 3000

// error number for configure
#define ERROR_CONFIGURE_OPEN_FILE_FAILED 4000
#define ERROR_CONFIGURE_NO_SERVER_PARTS 4001
#define ERROR_CONFIGURE_SYNTAX_INVALID 4002
#endif
