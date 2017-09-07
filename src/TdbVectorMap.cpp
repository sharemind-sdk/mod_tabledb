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

#include "TdbVectorMap.h"

#include <boost/assign/ptr_list_of.hpp>
#include <cassert>


namespace {
extern "C" {

#define TDB_VECTOR_MAP_CATCH_ALL \
    catch (const sharemind::TdbVectorMap::NotFoundException &) { \
        return TDB_VECTOR_MAP_VALUE_NOT_FOUND; \
    } catch (const sharemind::TdbVectorMap::TypeException &) { \
        return TDB_VECTOR_MAP_TYPE_ERROR; \
    } catch (const sharemind::TdbVectorMap::Exception &) { \
        return TDB_VECTOR_MAP_GENERAL_ERROR; \
    } catch (const std::bad_alloc &) { \
        return TDB_VECTOR_MAP_OUT_OF_MEMORY; \
    } catch (...) { \
        return TDB_VECTOR_MAP_UNKNOWN_ERROR; \
    }

SharemindTdbVectorMapError SharemindTdbVectorMap_get_index_vector(
        SharemindTdbVectorMap * map,
        const char * key,
        SharemindTdbIndex *** vec,
        size_t * size)
{
    assert(map);
    try {
        auto & m = sharemind::TdbVectorMap::fromWrapper(*map);
        m.getCArray<SharemindTdbIndex>(key, *vec, *size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_set_index_vector(
        SharemindTdbVectorMap * map,
        const char * key,
        SharemindTdbIndex ** vec,
        const size_t size)
{
    assert(map);
    try {
        auto & m = sharemind::TdbVectorMap::fromWrapper(*map);
        m.setCArray<SharemindTdbIndex>(key, vec, size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_is_index_vector(
        SharemindTdbVectorMap * map,
        const char * key,
        bool * rv)
{
    assert(map);
    try {
        auto & m = sharemind::TdbVectorMap::fromWrapper(*map);
        *rv = m.count<SharemindTdbIndex>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_get_string_vector(
        SharemindTdbVectorMap * map,
        const char * key,
        SharemindTdbString *** vec,
        size_t * size)
{
    assert(map);
    try {
        auto & m = sharemind::TdbVectorMap::fromWrapper(*map);
        m.getCArray<SharemindTdbString>(key, *vec, *size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_set_string_vector(
        SharemindTdbVectorMap * map,
        const char * key,
        SharemindTdbString ** vec,
        const size_t size)
{
    assert(map);
    try {
        auto & m = sharemind::TdbVectorMap::fromWrapper(*map);
        m.setCArray<SharemindTdbString>(key, vec, size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_is_string_vector(
        SharemindTdbVectorMap * map,
        const char * key,
        bool * rv)
{
    assert(map);
    try {
        auto & m = sharemind::TdbVectorMap::fromWrapper(*map);
        *rv = m.count<SharemindTdbString>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_get_type_vector(
        SharemindTdbVectorMap * map,
        const char * key,
        SharemindTdbType *** vec,
        size_t * size)
{
    assert(map);
    try {
        auto & m = sharemind::TdbVectorMap::fromWrapper(*map);
        m.getCArray<SharemindTdbType>(key, *vec, *size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_set_type_vector(
        SharemindTdbVectorMap * map,
        const char * key,
        SharemindTdbType ** vec,
        const size_t size)
{
    assert(map);
    try {
        auto & m = sharemind::TdbVectorMap::fromWrapper(*map);
        m.setCArray<SharemindTdbType>(key, vec, size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_is_type_vector(
        SharemindTdbVectorMap * map,
        const char * key,
        bool * rv)
{
    assert(map);
    try {
        auto & m = sharemind::TdbVectorMap::fromWrapper(*map);
        *rv = m.count<SharemindTdbType>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_get_value_vector(
        SharemindTdbVectorMap * map,
        const char * key,
        SharemindTdbValue *** vec,
        size_t * size)
{
    assert(map);
    try {
        auto & m = sharemind::TdbVectorMap::fromWrapper(*map);
        m.getCArray<SharemindTdbValue>(key, *vec, *size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_set_value_vector(
        SharemindTdbVectorMap * map,
        const char * key,
        SharemindTdbValue ** vec,
        const size_t size)
{
    assert(map);
    try {
        auto & m = sharemind::TdbVectorMap::fromWrapper(*map);
        m.setCArray<SharemindTdbValue>(key, vec, size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_is_value_vector(
        SharemindTdbVectorMap * map,
        const char * key,
        bool * rv)
{
    assert(map);
    try {
        auto & m = sharemind::TdbVectorMap::fromWrapper(*map);
        *rv = m.count<SharemindTdbValue>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_count(
        SharemindTdbVectorMap * map,
        const char * key,
        bool * rv)
{
    assert(map);
    assert(key);
    assert(rv);
    try {
        *rv = sharemind::TdbVectorMap::fromWrapper(*map).count(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_erase(
        SharemindTdbVectorMap * map,
        const char * key,
        bool * rv)
{
    assert(map);
    assert(key);
    assert(rv);
    try {
        *rv = sharemind::TdbVectorMap::fromWrapper(*map).erase(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_clear(
        SharemindTdbVectorMap * map)
{
    assert(map);
    try {
        sharemind::TdbVectorMap::fromWrapper(*map).clear();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_set_batch(
        SharemindTdbVectorMap * map,
        const size_t n)
{
    assert(map);
    try {
        sharemind::TdbVectorMap::fromWrapper(*map).setBatch(n);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_add_batch(
        SharemindTdbVectorMap * map)
{
    assert(map);
    try {
        sharemind::TdbVectorMap::fromWrapper(*map).addBatch();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_batch_count(
        SharemindTdbVectorMap * map,
        size_t * count)
{
    assert(map);
    try {
        *count = sharemind::TdbVectorMap::fromWrapper(*map).batchCount();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_reset(
        SharemindTdbVectorMap * map)
{
    assert(map);
    try {
        sharemind::TdbVectorMap::fromWrapper(*map).reset();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

uint64_t SharemindTdbVectorMap_get_id(SharemindTdbVectorMap * map) {
    assert(map);
    return sharemind::TdbVectorMap::fromWrapper(*map).getId();
}

} // extern "C" {
} // anonymous namespace

namespace sharemind {

TdbVectorMap::TdbVectorMap(const uint64_t id)
    : ::SharemindTdbVectorMap{&SharemindTdbVectorMap_get_index_vector,
                              &SharemindTdbVectorMap_set_index_vector,
                              &SharemindTdbVectorMap_is_index_vector,
                              &SharemindTdbVectorMap_get_string_vector,
                              &SharemindTdbVectorMap_set_string_vector,
                              &SharemindTdbVectorMap_is_string_vector,
                              &SharemindTdbVectorMap_get_type_vector,
                              &SharemindTdbVectorMap_set_type_vector,
                              &SharemindTdbVectorMap_is_type_vector,
                              &SharemindTdbVectorMap_get_value_vector,
                              &SharemindTdbVectorMap_set_value_vector,
                              &SharemindTdbVectorMap_is_value_vector,
                              &SharemindTdbVectorMap_count,
                              &SharemindTdbVectorMap_erase,
                              &SharemindTdbVectorMap_clear,
                              &SharemindTdbVectorMap_set_batch,
                              &SharemindTdbVectorMap_add_batch,
                              &SharemindTdbVectorMap_batch_count,
                              &SharemindTdbVectorMap_reset,
                              &SharemindTdbVectorMap_get_id}
    , m_id{id}
    , m_batches{boost::ptr_vector<AnyValueMap>(boost::assign::ptr_list_of<AnyValueMap>())}
    , m_currentBatch{m_batches.begin()}
{}

} /* namespace sharemind { */
