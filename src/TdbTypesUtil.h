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


inline SharemindTdbType * SharemindTdbType_new(std::string const & domain,
                                               std::string const & name,
                                               std::uint64_t const size);

inline SharemindTdbType * SharemindTdbType_new(char const * domain,
                                               char const * name,
                                               std::uint64_t const size);

inline void SharemindTdbType_delete(SharemindTdbType * ptr);

namespace sharemind {
namespace mod_tabledb {

struct TdbString: SharemindTdbString {

    TdbString(std::string const & s) {
        auto const size = s.size() + 1u;
        str = new char[size];
        std::strncpy(str, s.c_str(), size);
    }

    TdbString(char const * const s) {
        assert(str);
        auto const size = std::strlen(s);
        str = new char[size];
        std::strncpy(str, s, size);
    }

    ~TdbString() noexcept { delete[] str; }

}; /* struct TdbString */

struct TdbType: SharemindTdbType {

    TdbType(std::string const & domain_,
            std::string const & name_,
            std::uint64_t const size_)
    {
        {
            auto const domainSize = domain_.size() + 1u;
            domain = new char[domainSize];
            std::strncpy(domain, domain_.c_str(), domainSize);
        }
        try {
            {
                auto const nameSize = name_.size() + 1u;
                name = new char[nameSize];
                std::strncpy(name, name_.c_str(), nameSize);
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
            auto const domainSize = std::strlen(domain_);
            domain = new char[domainSize];
            strncpy(domain, domain_, domainSize);
        }
        try {
            {
                auto const nameSize = std::strlen(name_);
                name = new char[nameSize];
                std::strncpy(name, name_, nameSize);
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

    TdbValue(std::string const & typeDomain,
             std::string const & typeName,
             std::uint64_t const typeSize,
             void const * buffer_,
             std::uint64_t const size_)
    {
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

inline SharemindTdbIndex * SharemindTdbIndex_new(std::uint64_t const idx) {
    auto const ptr = new SharemindTdbIndex;
    ptr->idx = idx;
    return ptr;
}

inline void SharemindTdbIndex_delete(SharemindTdbIndex const * ptr) {
    assert(ptr);
    delete ptr;
}

inline SharemindTdbString * SharemindTdbString_new(std::string const & str)
{ return new sharemind::mod_tabledb::TdbString(str); }

inline SharemindTdbString * SharemindTdbString_new(char const * str)
{ return new sharemind::mod_tabledb::TdbString(str); }

inline void SharemindTdbString_delete(SharemindTdbString * ptr) {
    assert(ptr);
    delete static_cast<sharemind::mod_tabledb::TdbString *>(ptr);
}

inline SharemindTdbType * SharemindTdbType_new(std::string const & domain,
                                               std::string const & name,
                                               std::uint64_t const size)
{ return new sharemind::mod_tabledb::TdbType(domain, name, size); }

inline SharemindTdbType * SharemindTdbType_new(char const * domain,
                                               char const * name,
                                               std::uint64_t const size)
{ return new sharemind::mod_tabledb::TdbType(domain, name, size); }

inline void SharemindTdbType_delete(SharemindTdbType * ptr) {
    assert(ptr);
    delete static_cast<sharemind::mod_tabledb::TdbType *>(ptr);
}

inline SharemindTdbValue * SharemindTdbValue_new(std::string const & typeDomain,
                                                 std::string const & typeName,
                                                 std::uint64_t const typeSize,
                                                 void const * buffer,
                                                 std::uint64_t const size)
{
    using sharemind::mod_tabledb::TdbValue;
    return new TdbValue(typeDomain, typeName, typeSize, buffer, size);
}

inline SharemindTdbValue * SharemindTdbValue_new(char const * typeDomain,
                                                 char const * typeName,
                                                 std::uint64_t const typeSize,
                                                 void const * buffer,
                                                 std::uint64_t const size)
{
    using sharemind::mod_tabledb::TdbValue;
    return new TdbValue(typeDomain, typeName, typeSize, buffer, size);
}

inline void SharemindTdbValue_delete(SharemindTdbValue * ptr) {
    assert(ptr);
    delete static_cast<sharemind::mod_tabledb::TdbValue *>(ptr);
}

#endif /* SHAREMIND_MOD_TABLEDB_TDBTYPESUTIL_H */
