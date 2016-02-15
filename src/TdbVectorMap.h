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

#ifndef SHAREMIND_MOD_TABLEDB_TDBVECTORMAP_H
#define SHAREMIND_MOD_TABLEDB_TDBVECTORMAP_H

#ifndef SHAREMIND_INTERNAL_
#error SHAREMIND_INTERNAL_ not defined
#endif

#include <stdexcept>
#include <typeinfo>
#include <map>
#include <vector>
#include <boost/any.hpp>
#include <boost/checked_delete.hpp>
#include <boost/ptr_container/clone_allocator.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "TdbTypesUtil.h"
#include "tdbtypes.h"
#include "tdbvectormapapi.h"


namespace sharemind {

// We want a custom clone allocator and deleter for some of the types, however
// we don't want to propagate the new_clone and delete_clone functions to all
// the other ptr_containers used elsewhere.
struct tdb_heap_clone_allocator {
public: /* Methods: */

    template<class U>
    static U * allocate_clone(const U & r) {
        return new_clone(r);
    }

    template<class U>
    static void deallocate_clone(const U * r) {
        delete_clone(r);
    }

private: /* Methods: */

    template<class T>
    static inline T * new_clone(const T & r) {
        return boost::new_clone(r);
    }

    template<class T>
    static inline T * new_clone(const T * r) {
        return boost::new_clone(r);
    }

    template<class T>
    static inline void delete_clone(const T * r) {
        boost::delete_clone(r);
    }

    static inline SharemindTdbString * new_clone(const SharemindTdbString & r) {
        SharemindTdbString * res = new SharemindTdbString;

        const size_t len = strlen(r.str);
        res->str = new char[len + 1];
        strncpy(res->str, r.str, len + 1);

        return res;
    }

    static inline void delete_clone(const SharemindTdbString * r) {
        delete[] r->str;
        boost::checked_delete(r);
    }

    static inline SharemindTdbType * new_clone(const SharemindTdbType & r) {
        SharemindTdbType * res = new SharemindTdbType(r);

        if (r.domain) {
            size_t len = strlen(r.domain);
            res->domain = new char[len + 1];
            strncpy(res->domain, r.domain, len + 1);
        }
        if (r.name) {
            size_t len = strlen(r.name);
            res->name = new char[len + 1];
            strncpy(res->name, r.name, len + 1);
        }

        return res;
    }

    static inline void delete_clone(const SharemindTdbType * r) {
        delete[] r->domain;
        delete[] r->name;
        boost::checked_delete(r);
    }

    static inline SharemindTdbValue * new_clone(const SharemindTdbValue & r) {
        SharemindTdbValue * res = new SharemindTdbValue;

        res->type = new_clone(*r.type);
        res->buffer = ::operator new(r.size);
        memcpy(res->buffer, r.buffer, r.size);
        res->size = r.size;

        return res;
    }

    static inline void delete_clone(const SharemindTdbValue * r) {
        ::operator delete(r->buffer);
        delete_clone(r->type);
        boost::checked_delete(r);
    }

};

class __attribute__ ((visibility("internal"))) TdbVectorMap {
public: /* Types: */

    class __attribute__ ((visibility("internal"))) Exception: public std::runtime_error {
    public: /* Methods: */

        inline Exception(const std::string & msg)
            : std::runtime_error(msg)
        { }

    };

    class __attribute__ ((visibility("internal"))) TypeException: public Exception {
    public: /* Methods: */

        inline TypeException(const std::string & msg)
            : Exception(msg)
        { }

    };

    class __attribute__ ((visibility("internal"))) NotFoundException: public Exception {
    public: /* Methods: */

        inline NotFoundException(const std::string & msg)
            : Exception(msg)
        { }

    };

private: /* Types: */

    typedef SharemindTdbVectorMap TdbVectorMapWrapper;

    typedef std::map<std::string, boost::any> AnyValueMap;

    typedef tdb_heap_clone_allocator CA;

public: /* Methods: */

    TdbVectorMap(const uint64_t id);

    template<typename V>
    typename boost::ptr_vector<V, CA>::size_type size(const std::string & key) const {
        // Check if the vector exists
        AnyValueMap::const_iterator it = m_currentBatch->find(key);
        if (it == m_currentBatch->end())
            throw NotFoundException("Failed to get \"" + key + "\": vector not found.");

        // Check if the vector has the right type
        const boost::ptr_vector<V, CA> * vec = boost::any_cast<boost::ptr_vector<V, CA> >(&it->second);
        if (!vec)
            throw TypeException("Failed to get \"" + key + "\": Stored type does not match the expected type.");
        return vec->size();
    }

    template<typename V>
    typename boost::ptr_vector<V, CA>::reference at(const std::string & key, typename boost::ptr_vector<V, CA>::size_type n) {
        // Check if the vector exists
        AnyValueMap::iterator it = m_currentBatch->find(key);
        if (it == m_currentBatch->end())
            throw NotFoundException("Failed to get \"" + key + "\": vector not found.");

        // Check if the vector has the right type
        boost::ptr_vector<V, CA> * vec = boost::any_cast<boost::ptr_vector<V, CA> >(&it->second);
        if (!vec)
            throw TypeException("Failed to get \"" + key + "\": Stored type does not match the expected type.");
        return vec->at(n);
    }

    template<typename V>
    typename boost::ptr_vector<V, CA>::const_reference at(const std::string & key, typename boost::ptr_vector<V, CA>::size_type n) const {
        // Check if the vector exists
        AnyValueMap::const_iterator it = m_currentBatch->find(key);
        if (it == m_currentBatch->end())
            throw NotFoundException("Failed to get \"" + key + "\": vector not found.");

        // Check if the vector has the right type
        const boost::ptr_vector<V, CA> * vec = boost::any_cast<boost::ptr_vector<V, CA> >(&it->second);
        if (!vec)
            throw TypeException("Failed to get \"" + key + "\": Stored type does not match the expected type.");
        return vec->at(n);
    }

    template<typename V>
    void push_back(const std::string & key, V * val) {
        // Check if the vector exists
        AnyValueMap::iterator it = m_currentBatch->find(key);
        if (it == m_currentBatch->end()) {
            std::pair<AnyValueMap::iterator, bool> rv =
                m_currentBatch->insert(AnyValueMap::value_type(key, boost::ptr_vector<V, CA>()));
            if (!rv.second)
                throw Exception("Failed to store vector \"" + key + "\".");

            it = rv.first;
        }

        // Check if the vector has the right type
        boost::ptr_vector<V, CA> * vec = boost::any_cast<boost::ptr_vector<V, CA> >(&it->second);
        if (!vec)
            throw TypeException("Failed to get \"" + key + "\": Stored type does not match the expected type.");
        vec->push_back(val);
    }

    template<typename V>
    void pop_back(const std::string & key) {
        // Check if the vector exists
        AnyValueMap::iterator it = m_currentBatch->find(key);
        if (it == m_currentBatch->end())
            throw NotFoundException("Failed to get \"" + key + "\": vector not found.");

        // Check if the vector has the right type
        boost::ptr_vector<V, CA> * vec = boost::any_cast<boost::ptr_vector<V, CA> >(&it->second);
        if (!vec)
            throw TypeException("Failed to get \"" + key + "\": Stored type does not match the expected type.");
        vec->pop_back();
    }

    template<typename V>
    void clear(const std::string & key) {
        // Check if the vector exists
        AnyValueMap::iterator it = m_currentBatch->find(key);
        if (it == m_currentBatch->end())
            throw NotFoundException("Failed to get \"" + key + "\": vector not found.");

        // Check if the vector has the right type
        boost::ptr_vector<V, CA> * vec = boost::any_cast<boost::ptr_vector<V, CA> >(&it->second);
        if (!vec)
            throw TypeException("Failed to get \"" + key + "\": Stored type does not match the expected type.");
        vec->clear();
    }

    template<typename V>
    bool count(const std::string & key) const {
        // Check if the vector exists
        AnyValueMap::const_iterator it = m_currentBatch->find(key);
        if (it == m_currentBatch->end())
            return false;

        // Check if the vector has the right type
        return it->second.type() == typeid(boost::ptr_vector<V, CA>);
    }

    bool count(const std::string & key) const {
        // Check if the vector exists
        return m_currentBatch->find(key) != m_currentBatch->end();
    }

    bool erase(const std::string & key) {
        return m_currentBatch->erase(key);
    }

    void clear() {
        m_currentBatch->clear();
    }

    template<typename V>
    void getCArray(const std::string & key, V **& array, typename boost::ptr_vector<V, CA>::size_type & size) {
        // Check if the vector exists
        AnyValueMap::iterator it = m_currentBatch->find(key);
        if (it == m_currentBatch->end())
            throw NotFoundException("Failed to get \"" + key + "\": vector not found.");

        // Check if the vector has the right type
        boost::ptr_vector<V, CA> * vec = boost::any_cast<boost::ptr_vector<V, CA> >(&it->second);
        if (!vec)
            throw TypeException("Failed to get \"" + key + "\": Stored type does not match the expected type.");

        array = vec->c_array();
        size = vec->size();
    }

    template<typename V>
    void setCArray(const std::string & key, V ** array, typename boost::ptr_vector<V, CA>::size_type size) {
        // Check if the vector exists
        AnyValueMap::iterator it = m_currentBatch->find(key);
        if (it != m_currentBatch->end())
            throw Exception("Failed to store \"" + key + "\": vector already exists.");

        std::pair<AnyValueMap::iterator, bool> rv =
            m_currentBatch->insert(AnyValueMap::value_type(key, boost::ptr_vector<V, CA>()));
        if (!rv.second)
            throw Exception("Failed to store vector \"" + key + "\".");

        it = rv.first;

        // Check if the vector has the right type
        boost::ptr_vector<V, CA> * vec = boost::any_cast<boost::ptr_vector<V, CA> >(&it->second);
        if (!vec)
            throw TypeException("Failed to get \"" + key + "\": Stored type does not match the expected type.");
        vec->transfer(vec->begin(), array, size);
    }

    inline void setBatch(const std::vector<AnyValueMap>::size_type n) {
        if (n >= m_batches.size())
            throw Exception("Failed to set batch: batch number out of range.");

        m_currentBatch = m_batches.begin() + n;
    }

    inline void addBatch() {
        m_batches.push_back(new AnyValueMap);
        m_currentBatch = m_batches.end() - 1;
    }

    inline std::vector<AnyValueMap>::size_type batchCount() const {
        return m_batches.size();
    }

    inline void reset() {
        m_batches.clear();
        addBatch();
    }

    inline uint64_t getId() const noexcept { return m_id; }

    inline TdbVectorMapWrapper * getWrapper() noexcept { return &m_wrapper; }

    inline const TdbVectorMapWrapper * getWrapper() const noexcept
    { return &m_wrapper; }

private: /* Fields: */

    TdbVectorMapWrapper m_wrapper;

    uint64_t m_id;
    boost::ptr_vector<AnyValueMap> m_batches;
    boost::ptr_vector<AnyValueMap>::iterator m_currentBatch;

}; /* class TdbVectorMap { */

} /* namespace sharemind { */

#endif /* SHAREMIND_MOD_TABLEDB_TDBVECTORMAP_H */
