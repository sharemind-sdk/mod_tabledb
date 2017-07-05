/*
 * Copyright (C) 2015 Cybernetica
 *
 * Research/Commercial License Usage
 * Licensees holding a valid Research License or Commercial License
 * for the Software may use this file according to the written
 * agreement between you and Cybernetica.
 *
 * GNU General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl-3.0.html.
 *
 * For further information, please contact us at sharemind@cyber.ee.
 */

#ifndef SHAREMIND_MOD_TABLEDB_TDBTYPESUTIL_H
#define SHAREMIND_MOD_TABLEDB_TDBTYPESUTIL_H

#include <cassert>
#include <cstdint>
#include <cstring>
#include <new>
#include <string>

#include "tdbtypes.h"


inline SharemindTdbIndex * SharemindTdbIndex_new(std::uint64_t const idx) {
    SharemindTdbIndex * ptr = new SharemindTdbIndex;

    ptr->idx = idx;

    return ptr;
}

inline void SharemindTdbIndex_delete(SharemindTdbIndex const * ptr) {
    assert(ptr);

    delete ptr;
}

inline SharemindTdbString * SharemindTdbString_new(std::string const & str) {
    SharemindTdbString * ptr = new SharemindTdbString;

    ptr->str = new char[str.size() + 1];
    strncpy(ptr->str, str.c_str(), str.size() + 1);

    return ptr;
}

inline SharemindTdbString * SharemindTdbString_new(char const * str) {
    assert(str);

    SharemindTdbString * ptr = new SharemindTdbString;

    const size_t len = strlen(str);
    ptr->str = new char[len + 1];
    strncpy(ptr->str, str, len + 1);

    return ptr;
}

inline void SharemindTdbString_delete(SharemindTdbString const * ptr) {
    assert(ptr);

    delete[] ptr->str;
    delete ptr;
}

inline SharemindTdbType * SharemindTdbType_new(std::string const & domain,
                                               std::string const & name,
                                               std::uint64_t const size)
{
    SharemindTdbType * ptr = new SharemindTdbType;

    ptr->domain = new char[domain.size() + 1];
    strncpy(ptr->domain, domain.c_str(), domain.size() + 1);
    ptr->name = new char[name.size() + 1];
    strncpy(ptr->name, name.c_str(), name.size() + 1);
    ptr->size = size;

    return ptr;
}

inline SharemindTdbType * SharemindTdbType_new(char const * domain,
                                               char const * name,
                                               std::uint64_t const size)
{
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

inline void SharemindTdbType_delete(SharemindTdbType const * ptr) {
    assert(ptr);

    delete[] ptr->name;
    delete[] ptr->domain;
    delete ptr;
}

inline SharemindTdbValue * SharemindTdbValue_new(std::string const & typeDomain,
                                                 std::string const & typeName,
                                                 std::uint64_t const typeSize,
                                                 void const * buffer,
                                                 std::uint64_t const size)
{
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

inline SharemindTdbValue * SharemindTdbValue_new(char const * typeDomain,
                                                 char const * typeName,
                                                 std::uint64_t const typeSize,
                                                 void const * buffer,
                                                 std::uint64_t const size)
{
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

inline void SharemindTdbValue_delete(SharemindTdbValue const * ptr) {
    assert(ptr);

    ::operator delete(ptr->buffer);
    SharemindTdbType_delete(ptr->type);
    delete ptr;
}

#endif /* SHAREMIND_MOD_TABLEDB_TDBTYPESUTIL_H */
