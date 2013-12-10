/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#include <cassert>

#define SHAREMIND_INTERNAL__
#include "TdbVectorMap.h"

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

SharemindTdbVectorMapError SharemindTdbVectorMap_size_index(SharemindTdbVectorMap * map, const char * key, size_t * size) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(size);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *size = m->size<SharemindTdbIndex>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_at_index(SharemindTdbVectorMap * map, const char * key, const uint64_t n, SharemindTdbIndex ** idx) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(idx);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *idx = &m->at<SharemindTdbIndex>(key, n);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_push_back_index(SharemindTdbVectorMap * map, const char * key, SharemindTdbIndex * idx) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->push_back<SharemindTdbIndex>(key, idx);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_pop_back_index(SharemindTdbVectorMap * map, const char * key) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->pop_back<SharemindTdbIndex>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_clear_index(SharemindTdbVectorMap * map, const char * key) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->clear<SharemindTdbIndex>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
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
    assert(key);
    assert(rv);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *rv = m->count<SharemindTdbIndex>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_size_string(SharemindTdbVectorMap * map, const char * key, size_t * size) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(size);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *size = m->size<SharemindTdbString>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_at_string(SharemindTdbVectorMap * map, const char * key, const uint64_t n, SharemindTdbString ** str) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(str);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *str = &m->at<SharemindTdbString>(key, n);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_push_back_string(SharemindTdbVectorMap * map, const char * key, SharemindTdbString * str) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(str);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->push_back<SharemindTdbString>(key, str);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_pop_back_string(SharemindTdbVectorMap * map, const char * key) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->pop_back<SharemindTdbString>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_clear_string(SharemindTdbVectorMap * map, const char * key) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->clear<SharemindTdbString>(key);
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
    assert(key);
    assert(rv);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *rv = m->count<SharemindTdbString>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_size_type(SharemindTdbVectorMap * map, const char * key, size_t * size) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(size);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *size = m->size<SharemindTdbType>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_at_type(SharemindTdbVectorMap * map, const char * key, const uint64_t n, SharemindTdbType ** type) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(type);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *type = &m->at<SharemindTdbType>(key, n);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_push_back_type(SharemindTdbVectorMap * map, const char * key, SharemindTdbType * type) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(type);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->push_back<SharemindTdbType>(key, type);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_pop_back_type(SharemindTdbVectorMap * map, const char * key) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->pop_back<SharemindTdbType>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_clear_type(SharemindTdbVectorMap * map, const char * key) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->clear<SharemindTdbType>(key);
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
    assert(key);
    assert(rv);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *rv = m->count<SharemindTdbType>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_size_value(SharemindTdbVectorMap * map, const char * key, size_t * size) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(size);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *size = m->size<SharemindTdbValue>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_at_value(SharemindTdbVectorMap * map, const char * key, const uint64_t n, SharemindTdbValue ** val) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(val);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *val = &m->at<SharemindTdbValue>(key, n);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_push_back_value(SharemindTdbVectorMap * map, const char * key, SharemindTdbValue * val) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(val);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->push_back<SharemindTdbValue>(key, val);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_pop_back_value(SharemindTdbVectorMap * map, const char * key) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->pop_back<SharemindTdbValue>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_clear_value(SharemindTdbVectorMap * map, const char * key) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->clear<SharemindTdbValue>(key);
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
    assert(key);
    assert(rv);

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

SharemindTdbVectorMapError SharemindTdbVectorMap_prev_batch(SharemindTdbVectorMap * map, bool * b) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *b = m->previousBatch();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

SharemindTdbVectorMapError SharemindTdbVectorMap_next_batch(SharemindTdbVectorMap * map, bool * b) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *b = m->nextBatch();
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

    m_wrapper.size_index = &SharemindTdbVectorMap_size_index;
    m_wrapper.at_index = &SharemindTdbVectorMap_at_index;
    m_wrapper.push_back_index = &SharemindTdbVectorMap_push_back_index;
    m_wrapper.pop_back_index = &SharemindTdbVectorMap_pop_back_index;
    m_wrapper.clear_index = &SharemindTdbVectorMap_clear_index;
    m_wrapper.get_index_vector = &SharemindTdbVectorMap_get_index_vector;
    m_wrapper.set_index_vector = &SharemindTdbVectorMap_set_index_vector;
    m_wrapper.is_index_vector = &SharemindTdbVectorMap_is_index_vector;

    m_wrapper.size_string = &SharemindTdbVectorMap_size_string;
    m_wrapper.at_string = &SharemindTdbVectorMap_at_string;
    m_wrapper.push_back_string = &SharemindTdbVectorMap_push_back_string;
    m_wrapper.pop_back_string = &SharemindTdbVectorMap_pop_back_string;
    m_wrapper.clear_string = &SharemindTdbVectorMap_clear_string;
    m_wrapper.get_string_vector = &SharemindTdbVectorMap_get_string_vector;
    m_wrapper.set_string_vector = &SharemindTdbVectorMap_set_string_vector;
    m_wrapper.is_string_vector = &SharemindTdbVectorMap_is_string_vector;

    m_wrapper.size_type = &SharemindTdbVectorMap_size_type;
    m_wrapper.at_type = &SharemindTdbVectorMap_at_type;
    m_wrapper.push_back_type = &SharemindTdbVectorMap_push_back_type;
    m_wrapper.pop_back_type = &SharemindTdbVectorMap_pop_back_type;
    m_wrapper.clear_type = &SharemindTdbVectorMap_clear_type;
    m_wrapper.get_type_vector = &SharemindTdbVectorMap_get_type_vector;
    m_wrapper.set_type_vector = &SharemindTdbVectorMap_set_type_vector;
    m_wrapper.is_type_vector = &SharemindTdbVectorMap_is_type_vector;

    m_wrapper.size_value = &SharemindTdbVectorMap_size_value;
    m_wrapper.at_value = &SharemindTdbVectorMap_at_value;
    m_wrapper.push_back_value = &SharemindTdbVectorMap_push_back_value;
    m_wrapper.pop_back_value = &SharemindTdbVectorMap_pop_back_value;
    m_wrapper.clear_value = &SharemindTdbVectorMap_clear_value;
    m_wrapper.get_value_vector = &SharemindTdbVectorMap_get_value_vector;
    m_wrapper.set_value_vector = &SharemindTdbVectorMap_set_value_vector;
    m_wrapper.is_value_vector = &SharemindTdbVectorMap_is_value_vector;

    m_wrapper.count = &SharemindTdbVectorMap_count;
    m_wrapper.erase = &SharemindTdbVectorMap_erase;
    m_wrapper.clear = &SharemindTdbVectorMap_clear;
    m_wrapper.get_id = &SharemindTdbVectorMap_get_id;

    m_wrapper.set_batch = &SharemindTdbVectorMap_set_batch;
    m_wrapper.prev_batch = &SharemindTdbVectorMap_prev_batch;
    m_wrapper.next_batch = &SharemindTdbVectorMap_next_batch;
    m_wrapper.add_batch = &SharemindTdbVectorMap_add_batch;
    m_wrapper.batch_count = &SharemindTdbVectorMap_batch_count;
    m_wrapper.reset = &SharemindTdbVectorMap_reset;
}

} /* namespace sharemind { */
