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

#pragma once

#include "rs_kernel_io.h"
#include "rs_common.h"

namespace AMF0_MARKER
{
    const uint8_t AMF0_NUMBER = 0x00;
    const uint8_t AMF0_BOOLEAN = 0x01;
    const uint8_t AMF0_STRING = 0x02;
    const uint8_t AMF0_OBJECT = 0x03;
    const uint8_t AMF0_MOVIECLIP = 0x04;
    const uint8_t AMF0_NULL = 0x05;
    const uint8_t AMF0_UNDEFINED = 0x06;
    const uint8_t AMF0_REFERENCE = 0x07;
    const uint8_t AMF0_ECMA_ARRAY = 0x08;
    const uint8_t AMF0_OBJECT_END = 0x09;
    const uint8_t AMF0_STRICT_ARRAY = 0x0A;
    const uint8_t AMF0_DATE = 0x0B;
    const uint8_t AMF0_LONG_STRING = 0x0C;
    const uint8_t AMF0_UNSUPPORTED = 0x0D;
    const uint8_t AMF0_RECORDSET = 0x0E;
    const uint8_t AMF0_XML_DOCUMENT = 0x0F;
    const uint8_t AMF0_TYPED_OBJECT = 0x10;
};

class RsAmf0Package
{
public:
    uint8_t marker;
public:
    RsAmf0Package();
    virtual ~RsAmf0Package();
public:
    bool is_number();
    bool is_boolean();
    bool is_string();
    bool is_objec();
    bool is_movieclip();
    bool is_null();
    bool is_undefined();
    bool is_reference();
    bool is_ecma_array();
    bool is_object_end();
    bool is_strict_array();
    bool is_date();
    bool is_long_string();
    bool is_unsupported();
    bool is_recordset();
    bool is_xml_document();
    bool is_typed_object();
protected:
    virtual std::string encode() = 0;
public:
    virtual int initialize(IRsReaderWriter *reader) = 0;
public:
    static RsAmf0Package* create_package(IRsReaderWriter *reader);
    std::string dump();
};

class RsAmf0Number : public RsAmf0Package
{
public:
    double value;
public:
    RsAmf0Number();
    RsAmf0Number(double nu);
    virtual ~RsAmf0Number();
protected:
    std::string encode();
public:
    int initialize(IRsReaderWriter *reader);
};

class RsAmf0Boolean : public RsAmf0Package
{
public:
    bool value;
public:
    RsAmf0Boolean();
    RsAmf0Boolean(bool val);
    virtual ~RsAmf0Boolean();
protected:
    std::string encode();
public:
    int initialize(IRsReaderWriter *reader);
};

class RsAmf0String : public RsAmf0Package
{
public:
    uint16_t size;
    std::string value;
public:
    RsAmf0String();
    RsAmf0String(std::string val);
    virtual ~RsAmf0String();
protected:
    std::string encode();
public:
    int initialize(IRsReaderWriter *reader);
};

class RsAmf0ObjectProperty
{
private:
    using Property = std::pair<std::string, std::shared_ptr<RsAmf0Package>>;
    std::vector<Property> properties;
public:
    RsAmf0ObjectProperty();
    virtual ~RsAmf0ObjectProperty();
public:
    void set(std::string key, RsAmf0Package* value);
    RsAmf0Package* get(std::string key);
    // begin with 0
    RsAmf0Package* get(int index);
public:
    int initialize(IRsReaderWriter *reader);
    uint32_t count();
    std::string dump();
};

class RsAmf0Object : public RsAmf0Package
{
private:
    RsAmf0ObjectProperty property;
public:
    RsAmf0Object();
    virtual ~RsAmf0Object();
public:
    void set(std::string key, RsAmf0Package *value);
    RsAmf0Package* get(std::string key);
    RsAmf0Package* get(int index);
protected:
    std::string encode();
public:
    int initialize(IRsReaderWriter *reader);
};

class RsAmf0Null : public RsAmf0Package
{
public:
    RsAmf0Null();
    virtual ~RsAmf0Null();
protected:
    std::string encode();
public:
    int initialize(IRsReaderWriter *reader);
};

class RsAmf0Undefined : public RsAmf0Package
{
public:
    RsAmf0Undefined();
    virtual ~RsAmf0Undefined();
protected:
    std::string encode();
public:
    int initialize(IRsReaderWriter *reader);
};

class RsAmf0Reference : public RsAmf0Package
{
public:
    uint16_t reference;
public:
    RsAmf0Reference();
    RsAmf0Reference(uint16_t ref);
    virtual ~RsAmf0Reference();
protected:
    std::string encode();
public:
    int initialize(IRsReaderWriter *reader);
};

class RsAmf0ECMAArray : public RsAmf0Package
{
public:
    uint32_t count;
    RsAmf0ObjectProperty properties;
public:
    RsAmf0ECMAArray();
    virtual ~RsAmf0ECMAArray();
public:
    void set(std::string key, RsAmf0Package* val);
    RsAmf0Package* get(std::string key);
    RsAmf0Package* get(int index);
protected:
    std::string encode();
public:
    int initialize(IRsReaderWriter *reader);
};

class RsAmf0StrictArray : public RsAmf0Package
{
public:
    uint32_t count;
    std::vector<std::shared_ptr<RsAmf0Package>> array;
public:
    RsAmf0StrictArray();
    virtual ~RsAmf0StrictArray();
public:
    void set(RsAmf0Package* val);
    RsAmf0Package* get(int index);
protected:
    std::string encode();
public:
    int initialize(IRsReaderWriter *reader);
};
