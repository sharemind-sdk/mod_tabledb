/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#include "TdbTypesUtil.h"

#include <cassert>
#include <cstring>
#include <new>


namespace sharemind {

SharemindTdbIndex * TdbIndex_new(const uint64_t idx) {
    SharemindTdbIndex * ptr = new SharemindTdbIndex;

    ptr->idx = idx;

    return ptr;
}

void TdbIndex_delete(const SharemindTdbIndex * ptr) {
    assert(ptr);

    delete ptr;
}

SharemindTdbString * TdbString_new(const std::string & str) {
    SharemindTdbString * ptr = new SharemindTdbString;

    ptr->str = new char[str.size() + 1];
    strncpy(ptr->str, str.c_str(), str.size() + 1);

    return ptr;
}

SharemindTdbString * TdbString_new(const char * str) {
    SharemindTdbString * ptr = new SharemindTdbString;

    const size_t len = strlen(str);
    ptr->str = new char[len + 1];
    strncpy(ptr->str, str, len + 1);

    return ptr;
}

void TdbString_delete(const SharemindTdbString * ptr) {
    assert(ptr);

    delete[] ptr->str;
    delete ptr;
}

SharemindTdbType * TdbType_new(const std::string & domain, const std::string & name, const uint64_t size) {
    SharemindTdbType * ptr = new SharemindTdbType;

    ptr->domain = new char[domain.size() + 1];
    strncpy(ptr->domain, domain.c_str(), domain.size() + 1);
    ptr->name = new char[name.size() + 1];
    strncpy(ptr->name, name.c_str(), name.size() + 1);
    ptr->size = size;

    return ptr;
}

SharemindTdbType * TdbType_new(const char * domain, const char * name, const uint64_t size) {
    SharemindTdbType * ptr = new SharemindTdbType;

    const size_t dlen = strlen(domain);
    ptr->domain = new char[dlen + 1];
    strncpy(ptr->domain, domain, dlen + 1);

    const size_t nlen = strlen(name);
    ptr->name = new char[nlen + 1];
    strncpy(ptr->name, name, nlen + 1);

    ptr->size = size;

    return ptr;
}

void TdbType_delete(const SharemindTdbType * ptr) {
    assert(ptr);

    delete[] ptr->name;
    delete[] ptr->domain;
    delete ptr;
}

SharemindTdbValue * TdbValue_new(const std::string & typeDomain, const std::string & typeName, const uint64_t typeSize, const void * buffer, const uint64_t size) {
    SharemindTdbValue * ptr = new SharemindTdbValue;

    ptr->type = TdbType_new(typeDomain, typeName, typeSize);
    ptr->buffer = ::operator new(size);
    memcpy(ptr->buffer, buffer, size);
    ptr->size = size;

    return ptr;
}

SharemindTdbValue * TdbValue_new(const char * typeDomain, const char * typeName, const uint64_t typeSize, const void * buffer, const uint64_t size) {
    SharemindTdbValue * ptr = new SharemindTdbValue;

    ptr->type = TdbType_new(typeDomain, typeName, typeSize);
    ptr->buffer = ::operator new(size);
    memcpy(ptr->buffer, buffer, size);
    ptr->size = size;

    return ptr;
}

void TdbValue_delete(const SharemindTdbValue * ptr) {
    assert(ptr);

    ::operator delete(ptr->buffer);
    TdbType_delete(ptr->type);
    delete ptr;
}

} /* namespace sharemind { */
