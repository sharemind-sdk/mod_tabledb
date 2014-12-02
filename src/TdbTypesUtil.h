/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#ifndef SHAREMIND_MOD_TABLEDB_TDBTYPESUTIL_H
#define SHAREMIND_MOD_TABLEDB_TDBTYPESUTIL_H

#include <cassert>
#include <cstring>
#include <new>
#include <string>

#include "tdbtypes.h"


inline static SharemindTdbIndex * SharemindTdbIndex_new(const uint64_t idx) {
    SharemindTdbIndex * ptr = new SharemindTdbIndex;

    ptr->idx = idx;

    return ptr;
}

inline static void SharemindTdbIndex_delete(const SharemindTdbIndex * ptr) {
    assert(ptr);

    delete ptr;
}

inline static SharemindTdbString * SharemindTdbString_new(const std::string & str) {
    SharemindTdbString * ptr = new SharemindTdbString;

    ptr->str = new char[str.size() + 1];
    strncpy(ptr->str, str.c_str(), str.size() + 1);

    return ptr;
}

inline static SharemindTdbString * SharemindTdbString_new(const char * str) {
    assert(str);

    SharemindTdbString * ptr = new SharemindTdbString;

    const size_t len = strlen(str);
    ptr->str = new char[len + 1];
    strncpy(ptr->str, str, len + 1);

    return ptr;
}

inline static void SharemindTdbString_delete(const SharemindTdbString * ptr) {
    assert(ptr);

    delete[] ptr->str;
    delete ptr;
}

inline static SharemindTdbType * SharemindTdbType_new(const std::string & domain, const std::string & name, const uint64_t size) {
    SharemindTdbType * ptr = new SharemindTdbType;

    ptr->domain = new char[domain.size() + 1];
    strncpy(ptr->domain, domain.c_str(), domain.size() + 1);
    ptr->name = new char[name.size() + 1];
    strncpy(ptr->name, name.c_str(), name.size() + 1);
    ptr->size = size;

    return ptr;
}

inline static SharemindTdbType * SharemindTdbType_new(const char * domain, const char * name, const uint64_t size) {
    assert(domain);
    assert(name);

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

inline static void SharemindTdbType_delete(const SharemindTdbType * ptr) {
    assert(ptr);

    delete[] ptr->name;
    delete[] ptr->domain;
    delete ptr;
}

inline static SharemindTdbValue * SharemindTdbValue_new(const std::string & typeDomain, const std::string & typeName, const uint64_t typeSize, const void * buffer, const uint64_t size) {
    SharemindTdbValue * ptr = new SharemindTdbValue;

    ptr->type = SharemindTdbType_new(typeDomain, typeName, typeSize);
    if (buffer) {
        assert(size > 0);
        ptr->buffer = ::operator new(size);
        memcpy(ptr->buffer, buffer, size);
    } else {
        assert(size == 0);
        ptr->buffer = nullptr;
    }
    ptr->size = size;

    return ptr;
}

inline static SharemindTdbValue * SharemindTdbValue_new(const char * typeDomain, const char * typeName, const uint64_t typeSize, const void * buffer, const uint64_t size) {
    assert(typeDomain);
    assert(typeName);

    SharemindTdbValue * ptr = new SharemindTdbValue;

    ptr->type = SharemindTdbType_new(typeDomain, typeName, typeSize);
    if (buffer) {
        assert(size > 0);
        ptr->buffer = ::operator new(size);
        memcpy(ptr->buffer, buffer, size);
    } else {
        assert(size == 0);
        ptr->buffer = nullptr;
    }
    ptr->size = size;

    return ptr;
}

inline static void SharemindTdbValue_delete(const SharemindTdbValue * ptr) {
    assert(ptr);

    ::operator delete(ptr->buffer);
    SharemindTdbType_delete(ptr->type);
    delete ptr;
}

#endif /* SHAREMIND_MOD_TABLEDB_TDBTYPESUTIL_H */
