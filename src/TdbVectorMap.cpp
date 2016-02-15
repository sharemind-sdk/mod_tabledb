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

#define SHAREMIND_INTERNAL_
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
    assert(map->internal);
    try {
        static_cast<sharemind::TdbVectorMap *>(map->internal)
                ->getCArray<SharemindTdbIndex>(key, *vec, *size);
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
    assert(map->internal);
    try {
        static_cast<sharemind::TdbVectorMap *>(map->internal)
                ->setCArray<SharemindTdbIndex>(key, vec, size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_is_index_vector(
        SharemindTdbVectorMap * map,
        const char * key,
        bool * rv)
{
    assert(map);
    assert(map->internal);
    try {
        *rv = static_cast<sharemind::TdbVectorMap *>(map->internal)
                ->count<SharemindTdbIndex>(key);
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
    assert(map->internal);
    try {
        static_cast<sharemind::TdbVectorMap *>(map->internal)
                ->getCArray<SharemindTdbString>(key, *vec, *size);
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
    assert(map->internal);
    try {
        static_cast<sharemind::TdbVectorMap *>(map->internal)
                ->setCArray<SharemindTdbString>(key, vec, size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_is_string_vector(
        SharemindTdbVectorMap * map,
        const char * key,
        bool * rv)
{
    assert(map);
    assert(map->internal);
    try {
        *rv = static_cast<sharemind::TdbVectorMap *>(map->internal)
                ->count<SharemindTdbString>(key);
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
    assert(map->internal);
    try {
        static_cast<sharemind::TdbVectorMap *>(map->internal)
                ->getCArray<SharemindTdbType>(key, *vec, *size);
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
    assert(map->internal);
    try {
        static_cast<sharemind::TdbVectorMap *>(map->internal)
                ->setCArray<SharemindTdbType>(key, vec, size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_is_type_vector(
        SharemindTdbVectorMap * map,
        const char * key,
        bool * rv)
{
    assert(map);
    assert(map->internal);
    try {
        *rv = static_cast<sharemind::TdbVectorMap *>(map->internal)
                ->count<SharemindTdbType>(key);
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
    assert(map->internal);
    try {
        static_cast<sharemind::TdbVectorMap *>(map->internal)
                ->getCArray<SharemindTdbValue>(key, *vec, *size);
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
    assert(map->internal);
    try {
        static_cast<sharemind::TdbVectorMap *>(map->internal)
                ->setCArray<SharemindTdbValue>(key, vec, size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_is_value_vector(
        SharemindTdbVectorMap * map,
        const char * key,
        bool * rv)
{
    assert(map);
    assert(map->internal);
    try {
        *rv = static_cast<sharemind::TdbVectorMap *>(map->internal)
                ->count<SharemindTdbValue>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_count(
        SharemindTdbVectorMap * map,
        const char * key,
        bool * rv)
{
    assert(map);
    assert(map->internal);
    assert(key);
    assert(rv);
    try {
        *rv = static_cast<sharemind::TdbVectorMap *>(map->internal)->count(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_erase(
        SharemindTdbVectorMap * map,
        const char * key,
        bool * rv)
{
    assert(map);
    assert(map->internal);
    assert(key);
    assert(rv);
    try {
        *rv = static_cast<sharemind::TdbVectorMap *>(map->internal)->erase(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_clear(
        SharemindTdbVectorMap * map)
{
    assert(map);
    assert(map->internal);
    try {
        static_cast<sharemind::TdbVectorMap *>(map->internal)->clear();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_set_batch(
        SharemindTdbVectorMap * map,
        const size_t n)
{
    assert(map);
    assert(map->internal);
    try {
        static_cast<sharemind::TdbVectorMap *>(map->internal)->setBatch(n);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_add_batch(
        SharemindTdbVectorMap * map)
{
    assert(map);
    assert(map->internal);
    try {
        static_cast<sharemind::TdbVectorMap *>(map->internal)->addBatch();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_batch_count(
        SharemindTdbVectorMap * map,
        size_t * count)
{
    assert(map);
    assert(map->internal);
    try {
        *count = static_cast<sharemind::TdbVectorMap *>(map->internal)
                    ->batchCount();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_reset(
        SharemindTdbVectorMap * map)
{
    assert(map);
    assert(map->internal);
    try {
        static_cast<sharemind::TdbVectorMap *>(map->internal)->reset();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

uint64_t SharemindTdbVectorMap_get_id(SharemindTdbVectorMap * map) {
    assert(map);
    assert(map->internal);
    return static_cast<sharemind::TdbVectorMap *>(map->internal)->getId();
}

} // extern "C" {
} // anonymous namespace

namespace sharemind {

TdbVectorMap::TdbVectorMap(const uint64_t id)
    : m_wrapper{this,
                &SharemindTdbVectorMap_get_index_vector,
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
