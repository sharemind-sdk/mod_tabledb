/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#define SHAREMIND_INTERNAL__
#include "TdbVectorMap.h"

#include <cassert>


#ifdef __cplusplus
extern "C" {
#endif

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

SharemindTdbVectorMapError SharemindTdbVectorMap_get_index_vector(SharemindTdbVectorMap * map, const char * key, SharemindTdbIndex *** vec, size_t * size) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        m->getCArray<SharemindTdbIndex>(key, *vec, *size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_set_index_vector(SharemindTdbVectorMap * map, const char * key, SharemindTdbIndex ** vec, const size_t size) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        m->setCArray<SharemindTdbIndex>(key, vec, size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_is_index_vector(SharemindTdbVectorMap * map, const char * key, bool * rv) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        *rv = m->count<SharemindTdbIndex>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_get_string_vector(SharemindTdbVectorMap * map, const char * key, SharemindTdbString *** vec, size_t * size) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        m->getCArray<SharemindTdbString>(key, *vec, *size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_set_string_vector(SharemindTdbVectorMap * map, const char * key, SharemindTdbString ** vec, const size_t size) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        m->setCArray<SharemindTdbString>(key, vec, size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_is_string_vector(SharemindTdbVectorMap * map, const char * key, bool * rv) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        *rv = m->count<SharemindTdbString>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_get_type_vector(SharemindTdbVectorMap * map, const char * key, SharemindTdbType *** vec, size_t * size) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        m->getCArray<SharemindTdbType>(key, *vec, *size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_set_type_vector(SharemindTdbVectorMap * map, const char * key, SharemindTdbType ** vec, const size_t size) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        m->setCArray<SharemindTdbType>(key, vec, size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_is_type_vector(SharemindTdbVectorMap * map, const char * key, bool * rv) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        *rv = m->count<SharemindTdbType>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_get_value_vector(SharemindTdbVectorMap * map, const char * key, SharemindTdbValue *** vec, size_t * size) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        m->getCArray<SharemindTdbValue>(key, *vec, *size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_set_value_vector(SharemindTdbVectorMap * map, const char * key, SharemindTdbValue ** vec, const size_t size) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        m->setCArray<SharemindTdbValue>(key, vec, size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_is_value_vector(SharemindTdbVectorMap * map, const char * key, bool * rv) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        *rv = m->count<SharemindTdbValue>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_count(SharemindTdbVectorMap * map, const char * key, bool * rv) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(rv);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *rv = m->count(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_erase(SharemindTdbVectorMap * map, const char * key, bool * rv) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(rv);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *rv = m->erase(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_clear(SharemindTdbVectorMap * map) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->clear();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_set_batch(SharemindTdbVectorMap * map, const size_t n) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->setBatch(n);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_add_batch(SharemindTdbVectorMap * map) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->addBatch();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_batch_count(SharemindTdbVectorMap * map, size_t * count) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *count = m->batchCount();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_reset(SharemindTdbVectorMap * map) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->reset();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

uint64_t SharemindTdbVectorMap_get_id(SharemindTdbVectorMap * map) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    return m->getId();
}

#ifdef __cplusplus
} /* extern "C" { */
#endif

namespace sharemind {

TdbVectorMap::TdbVectorMap(const uint64_t id)
    : m_id(id)
{
    m_batches.push_back(new AnyValueMap);
    m_currentBatch = m_batches.begin();

    m_wrapper.internal = this;

    m_wrapper.get_index_vector = &SharemindTdbVectorMap_get_index_vector;
    m_wrapper.set_index_vector = &SharemindTdbVectorMap_set_index_vector;
    m_wrapper.is_index_vector = &SharemindTdbVectorMap_is_index_vector;

    m_wrapper.get_string_vector = &SharemindTdbVectorMap_get_string_vector;
    m_wrapper.set_string_vector = &SharemindTdbVectorMap_set_string_vector;
    m_wrapper.is_string_vector = &SharemindTdbVectorMap_is_string_vector;

    m_wrapper.get_type_vector = &SharemindTdbVectorMap_get_type_vector;
    m_wrapper.set_type_vector = &SharemindTdbVectorMap_set_type_vector;
    m_wrapper.is_type_vector = &SharemindTdbVectorMap_is_type_vector;

    m_wrapper.get_value_vector = &SharemindTdbVectorMap_get_value_vector;
    m_wrapper.set_value_vector = &SharemindTdbVectorMap_set_value_vector;
    m_wrapper.is_value_vector = &SharemindTdbVectorMap_is_value_vector;

    m_wrapper.count = &SharemindTdbVectorMap_count;
    m_wrapper.erase = &SharemindTdbVectorMap_erase;
    m_wrapper.clear = &SharemindTdbVectorMap_clear;
    m_wrapper.get_id = &SharemindTdbVectorMap_get_id;

    m_wrapper.set_batch = &SharemindTdbVectorMap_set_batch;
    m_wrapper.add_batch = &SharemindTdbVectorMap_add_batch;
    m_wrapper.batch_count = &SharemindTdbVectorMap_batch_count;
    m_wrapper.reset = &SharemindTdbVectorMap_reset;
}

} /* namespace sharemind { */
