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

#include "rs_protocol_amf0.h"
#include "rs_kernel_buffer.h"
using namespace std;

RsAmf0Package::RsAmf0Package()
{

}

RsAmf0Package::~RsAmf0Package()
{

}

bool RsAmf0Package::is_number()
{
    return marker == AMF0_MARKER::AMF0_NUMBER;
}

bool RsAmf0Package::is_boolean()
{
    return marker == AMF0_MARKER::AMF0_BOOLEAN;
}

bool RsAmf0Package::is_string()
{
    return marker == AMF0_MARKER::AMF0_STRING;
}

bool RsAmf0Package::is_objec()
{
    return marker == AMF0_MARKER::AMF0_OBJECT;
}

bool RsAmf0Package::is_movieclip()
{
    return marker == AMF0_MARKER::AMF0_MOVIECLIP;
}

bool RsAmf0Package::is_null()
{
    return marker == AMF0_MARKER::AMF0_NULL;
}

bool RsAmf0Package::is_undefined()
{
    return marker == AMF0_MARKER::AMF0_UNDEFINED;
}

bool RsAmf0Package::is_reference()
{
    return marker == AMF0_MARKER::AMF0_REFERENCE;
}

bool RsAmf0Package::is_ecma_array()
{
    return marker == AMF0_MARKER::AMF0_ECMA_ARRAY;
}

bool RsAmf0Package::is_object_end()
{
    return marker == AMF0_MARKER::AMF0_OBJECT_END;
}

bool RsAmf0Package::is_strict_array()
{
    return marker == AMF0_MARKER::AMF0_STRICT_ARRAY;
}

bool RsAmf0Package::is_date()
{
    return marker == AMF0_MARKER::AMF0_DATE;
}

bool RsAmf0Package::is_long_string()
{
    return marker == AMF0_MARKER::AMF0_LONG_STRING;
}

bool RsAmf0Package::is_unsupported()
{
    return marker == AMF0_MARKER::AMF0_UNSUPPORTED;
}

bool RsAmf0Package::is_recordset()
{
    return marker == AMF0_MARKER::AMF0_RECORDSET;
}

bool RsAmf0Package::is_xml_document()
{
    return marker == AMF0_MARKER::AMF0_XML_DOCUMENT;
}

bool RsAmf0Package::is_typed_object()
{
    return marker == AMF0_MARKER::AMF0_TYPED_OBJECT;
}

string RsAmf0Package::dump()
{
    return encode();
}

RsAmf0Number::RsAmf0Number(double nu)
{
    marker = AMF0_MARKER::AMF0_NUMBER;
    value = nu;
}

RsAmf0Number::~RsAmf0Number()
{

}

string RsAmf0Number::encode()
{
    RsBufferLittleEndian buf;
    buf.write_1_byte(marker);
    buf.write_8_byte(static_cast<uint64_t>(value));
    return buf.dump();
}

int RsAmf0Number::initialize(IRsReaderWriter *reader)
{
    int ret = ERROR_SUCCESS;

    string buf;
    // read mark
    if ((ret = reader->read(buf, 1)) != ERROR_SUCCESS) {
        cout << "read mark for amf0 number failed. ret=" << ret << endl;
        return ret;
    }
    buf.clear();

    if (buf[0] != AMF0_MARKER::AMF0_NUMBER) {
        ret = ERROR_RTMP_PROTOCOL_AMF0_DECODE_ERROR;
        return ret;
    }
    marker = (uint8_t)buf[0];

    if ((ret = reader->read(buf, 8)) != ERROR_SUCCESS) {
        cout << "read number for amf0 number failed. ret=" << ret << endl;
        return ret;
    }
    value = static_cast<double>(RsBufferLittleEndian::convert_8bytes_into_uint64(buf));
    return ret;
}

RsAmf0Boolean::RsAmf0Boolean(bool val)
{
    marker = AMF0_MARKER::AMF0_BOOLEAN;
    value = val;
}

RsAmf0Boolean::~RsAmf0Boolean()
{

}

string RsAmf0Boolean::encode()
{
    RsBufferLittleEndian buf;
    buf.write_1_byte(marker);
    buf.write_1_byte(uint8_t(value));
    return buf.dump();
}

int RsAmf0Boolean::initialzie(IRsReaderWriter *reader)
{
    int ret = ERROR_SUCCESS;
    string buf;
    // read marker
    if ((ret = reader->read(buf, 1)) != ERROR_SUCCESS) {
        cout << "read marker for amf0 string failed. ret=" << ret << endl;
        return ret;
    }

    if (buf[0] != AMF0_MARKER::AMF0_BOOLEAN) {
        ret = ERROR_RTMP_PROTOCOL_AMF0_DECODE_ERROR;
        return ret;
    }
    marker = (uint8_t)buf[0];
    buf.clear();

    // read value
    if ((ret = reader->read(buf, 1)) != ERROR_SUCCESS) {
        cout << "read value for amf0 boolean failed. ret=" << ret << endl;
        return ret;
    }

    value = (uint8_t)buf[0];
    return ret;
}

RsAmf0String::RsAmf0String(string val)
{
    marker = AMF0_MARKER::AMF0_STRING;
    value = val;
}

RsAmf0String::~RsAmf0String()
{

}

string RsAmf0String::encode()
{
    RsBufferLittleEndian buf;
    buf.write_1_byte(marker);
    buf.write_2_byte((uint16_t) value.size());
    buf.write_bytes(value);
    return buf.dump();
}

int RsAmf0String::initialize(IRsReaderWriter *reader)
{
    int ret = ERROR_SUCCESS;
    string buf;

    // read mark
    if ((ret = reader->read(buf, 1)) != ERROR_SUCCESS) {
        cout << "read marker for amf0 string failed. ret=" << ret << endl;
        return ret;
    }
    if (buf[0] != AMF0_MARKER::AMF0_STRING) {
        ret = ERROR_RTMP_PROTOCOL_AMF0_DECODE_ERROR;
        return ret;
    }
    marker = (uint8_t)buf[0];
    buf.clear();

    // read size
    if ((ret = reader->read(buf, 2)) != ERROR_SUCCESS) {
        cout << "read size of string for amf0 string failed. ret=" << ret << endl;
        return ret;
    }
    size = RsBufferLittleEndian::convert_2bytes_into_uint16(buf);
    buf.clear();

    // read string
    if ((ret = reader->read(buf, size)) != ERROR_SUCCESS) {
        cout << "read string for amf0 string failed. ret=" << ret << endl;
        return ret;
    }

    value = buf;
    return ret;
}

RsAmf0ObjectProperty::RsAmf0ObjectProperty()
{

}

RsAmf0ObjectProperty::~RsAmf0ObjectProperty()
{

}

void RsAmf0ObjectProperty::set(std::string key, RsAmf0Package *value)
{
    auto predicate = [&](Property elem) {
        return elem.first == key;
    };

    properties.erase(remove_if(properties.begin(), properties.end(), predicate), properties.end());
    properties.push_back(make_pair(key, shared_ptr<RsAmf0Package>(value)));
}

RsAmf0Package* RsAmf0ObjectProperty::get(std::string key)
{
    for (auto i : properties) {
        if (i.first == key) {
            return i.second.get();
        }
    }

    return nullptr;
}

string RsAmf0ObjectProperty::dump()
{
    RsBufferLittleEndian buf;
    for (auto i : properties) {
        buf.write_2_byte((uint16_t) i.first.length());
        buf.write_bytes(i.first);
        buf.write_bytes(i.second->dump());
    }

    return buf.dump();
}
