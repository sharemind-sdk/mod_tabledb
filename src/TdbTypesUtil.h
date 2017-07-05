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

#include "tdbtypes.h"


inline SharemindTdbType * SharemindTdbType_new2(char const * domain,
                                                std::size_t domainSize,
                                                char const * name,
                                                std::size_t nameSize,
                                                std::uint64_t const size);

inline SharemindTdbType * SharemindTdbType_new(char const * domain,
                                               char const * name,
                                               std::uint64_t const size);

inline void SharemindTdbType_delete(SharemindTdbType * ptr);

namespace sharemind {
namespace mod_tabledb {

struct TdbString: SharemindTdbString {

    TdbString(char const * const s, std::size_t const size) {
        str = new char[size + 1u];
        std::memcpy(str, s, size);
        str[size] = '\0';
    }

    TdbString(char const * const s) {
        assert(str);
        auto const allocSize = std::strlen(s) + 1u;
        str = new char[allocSize];
        std::memcpy(str, s, allocSize);
    }

    ~TdbString() noexcept { delete[] str; }

}; /* struct TdbString */

struct TdbType: SharemindTdbType {

    TdbType(char const * const domain_,
            std::size_t domainSize,
            char const * const name_,
            std::size_t nameSize,
            std::uint64_t const size_)
    {
        {
            domain = new char[domainSize + 1u];
            std::memcpy(domain, domain_, domainSize);
            domain[domainSize] = '\0';
        }
        try {
            {
                name = new char[nameSize + 1u];
                std::memcpy(name, name_, nameSize);
                name[nameSize] = '\0';
            }
            size = size_;
        } catch (...) {
            delete[] domain;
            throw;
        }
    }

    TdbType(char const * const domain_,
            char const * const name_,
            std::uint64_t const size_)
    {
        assert(domain_);
        assert(name_);
        {
            auto const allocSize = std::strlen(domain_) + 1u;
            domain = new char[allocSize];
            std::memcpy(domain, domain_, allocSize);
        }
        try {
            {
                auto const allocSize = std::strlen(name_) + 1u;
                name = new char[allocSize];
                std::memcpy(name, name_, allocSize);
            }
            size = size_;
        } catch (...) {
            delete[] domain;
            throw;
        }
    }

    ~TdbType() noexcept {
        delete[] domain;
        delete[] name;
    }

}; /* struct TdbType */

struct TdbValue: SharemindTdbValue {

    TdbValue(char const * const typeDomain,
             std::size_t const typeDomainSize,
             char const * const typeName,
             std::size_t const typeNameSize,
             std::uint64_t const typeSize,
             void const * buffer_,
             std::uint64_t const size_)
    {
        type = SharemindTdbType_new2(typeDomain,
                                     typeDomainSize,
                                     typeName,
                                     typeNameSize,
                                     typeSize);
        try {
            if (buffer_) {
                assert(size_ > 0);
                buffer = ::operator new(size_);
                std::memcpy(buffer, buffer_, size_);
            } else {
                assert(size_ == 0);
                buffer = nullptr;
            }
            size = size_;
        } catch (...) {
            SharemindTdbType_delete(type);
            throw;
        }
    }

    TdbValue(char const * typeDomain,
             char const * typeName,
             std::uint64_t const typeSize,
             void const * buffer_,
             std::uint64_t const size_)
    {
        assert(typeDomain);
        assert(typeName);

        type = SharemindTdbType_new(typeDomain, typeName, typeSize);
        try {
            if (buffer_) {
                assert(size_ > 0);
                buffer = ::operator new(size_);
                std::memcpy(buffer, buffer_, size_);
            } else {
                assert(size_ == 0);
                buffer = nullptr;
            }
            size = size_;
        } catch (...) {
            SharemindTdbType_delete(type);
            throw;
        }
    }

    ~TdbValue() noexcept {
        ::operator delete(buffer);
        SharemindTdbType_delete(type);
    }

}; /* struct TdbValue */

} /* namespace mod_tabledb { */
} /* namespace sharemind { */

inline SharemindTdbIndex * SharemindTdbIndex_new(std::uint64_t const idx)
{ return new (std::nothrow) SharemindTdbIndex{idx}; }

inline void SharemindTdbIndex_delete(SharemindTdbIndex const * ptr) {
    assert(ptr);
    delete ptr;
}

inline SharemindTdbString * SharemindTdbString_new(char const * str) {
    try {
        return new (std::nothrow) sharemind::mod_tabledb::TdbString(str);
    } catch (...) { return nullptr; }
}

inline SharemindTdbString * SharemindTdbString_new2(char const * str,
                                                    std::size_t strSize)
{
    try {
        return new (std::nothrow) sharemind::mod_tabledb::TdbString(str,
                                                                    strSize);
    } catch (...) { return nullptr; }
}

inline void SharemindTdbString_delete(SharemindTdbString * ptr) {
    assert(ptr);
    delete static_cast<sharemind::mod_tabledb::TdbString *>(ptr);
}

inline SharemindTdbType * SharemindTdbType_new(char const * domain,
                                               char const * name,
                                               std::uint64_t const size)
{
    try {
        using Type = sharemind::mod_tabledb::TdbType;
        return new (std::nothrow) Type(domain, name, size);
    } catch (...) { return nullptr; }
}

inline SharemindTdbType * SharemindTdbType_new2(char const * domain,
                                                std::size_t domainSize,
                                                char const * name,
                                                std::size_t nameSize,
                                                std::uint64_t const size)
{
    try {
        using T = sharemind::mod_tabledb::TdbType;
        return new (std::nothrow) T(domain, domainSize, name, nameSize, size);
    } catch (...) { return nullptr; }
}

inline void SharemindTdbType_delete(SharemindTdbType * ptr) {
    assert(ptr);
    delete static_cast<sharemind::mod_tabledb::TdbType *>(ptr);
}

inline SharemindTdbValue * SharemindTdbValue_new(char const * typeDomain,
                                                 char const * typeName,
                                                 std::uint64_t const typeSize,
                                                 void const * buffer,
                                                 std::uint64_t const size)
{
    try {
        return new (std::nothrow) sharemind::mod_tabledb::TdbValue(typeDomain,
                                                                   typeName,
                                                                   typeSize,
                                                                   buffer,
                                                                   size);
    } catch (...) { return nullptr; }
}

inline SharemindTdbValue * SharemindTdbValue_new2(char const * typeDomain,
                                                  std::size_t typeDomainSize,
                                                  char const * typeName,
                                                  std::size_t typeNameSize,
                                                  std::uint64_t const typeSize,
                                                  void const * buffer,
                                                  std::uint64_t const size)
{
    try {
        using sharemind::mod_tabledb::TdbValue;
        return new (std::nothrow) TdbValue(typeDomain,
                                           typeDomainSize,
                                           typeName,
                                           typeNameSize,
                                           typeSize,
                                           buffer,
                                           size);
    } catch (...) { return nullptr; }
}

inline void SharemindTdbValue_delete(SharemindTdbValue * ptr) {
    assert(ptr);
    delete static_cast<sharemind::mod_tabledb::TdbValue *>(ptr);
}

#endif /* SHAREMIND_MOD_TABLEDB_TDBTYPESUTIL_H */
