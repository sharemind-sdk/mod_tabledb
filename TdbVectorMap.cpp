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

TdbVectorMapError TdbVectorMap_size_index(TdbVectorMap * map, const char * key, size_t * size) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(size);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *size = m->size<TdbIndex>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_at_index(TdbVectorMap * map, const char * key, const uint64_t n, TdbIndex ** idx) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(idx);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *idx = &m->at<TdbIndex>(key, n);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_push_back_index(TdbVectorMap * map, const char * key, TdbIndex * idx) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->push_back<TdbIndex>(key, idx);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_pop_back_index(TdbVectorMap * map, const char * key) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->pop_back<TdbIndex>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_clear_index(TdbVectorMap * map, const char * key) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->clear<TdbIndex>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_get_index_vector(TdbVectorMap * map, const char * key, TdbIndex *** vec, size_t * size) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        m->getCArray<TdbIndex>(key, *vec, *size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_set_index_vector(TdbVectorMap * map, const char * key, TdbIndex ** vec, const size_t size) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        m->setCArray<TdbIndex>(key, vec, size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_is_index_vector(TdbVectorMap * map, const char * key, bool * rv) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(rv);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *rv = m->count<TdbIndex>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_size_string(TdbVectorMap * map, const char * key, size_t * size) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(size);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *size = m->size<TdbString>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_at_string(TdbVectorMap * map, const char * key, const uint64_t n, TdbString ** str) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(str);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *str = &m->at<TdbString>(key, n);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_push_back_string(TdbVectorMap * map, const char * key, TdbString * str) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(str);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->push_back<TdbString>(key, str);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_pop_back_string(TdbVectorMap * map, const char * key) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->pop_back<TdbString>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_clear_string(TdbVectorMap * map, const char * key) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->clear<TdbString>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_get_string_vector(TdbVectorMap * map, const char * key, TdbString *** vec, size_t * size) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        m->getCArray<TdbString>(key, *vec, *size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_set_string_vector(TdbVectorMap * map, const char * key, TdbString ** vec, const size_t size) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        m->setCArray<TdbString>(key, vec, size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_is_string_vector(TdbVectorMap * map, const char * key, bool * rv) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(rv);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *rv = m->count<TdbString>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_size_type(TdbVectorMap * map, const char * key, size_t * size) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(size);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *size = m->size<TdbType>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_at_type(TdbVectorMap * map, const char * key, const uint64_t n, TdbType ** type) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(type);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *type = &m->at<TdbType>(key, n);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_push_back_type(TdbVectorMap * map, const char * key, TdbType * type) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(type);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->push_back<TdbType>(key, type);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_pop_back_type(TdbVectorMap * map, const char * key) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->pop_back<TdbType>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_clear_type(TdbVectorMap * map, const char * key) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->clear<TdbType>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_get_type_vector(TdbVectorMap * map, const char * key, TdbType *** vec, size_t * size) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        m->getCArray<TdbType>(key, *vec, *size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_set_type_vector(TdbVectorMap * map, const char * key, TdbType ** vec, const size_t size) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        m->setCArray<TdbType>(key, vec, size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_is_type_vector(TdbVectorMap * map, const char * key, bool * rv) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(rv);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *rv = m->count<TdbType>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_size_value(TdbVectorMap * map, const char * key, size_t * size) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(size);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *size = m->size<TdbValue>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_at_value(TdbVectorMap * map, const char * key, const uint64_t n, TdbValue ** val) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(val);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *val = &m->at<TdbValue>(key, n);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_push_back_value(TdbVectorMap * map, const char * key, TdbValue * val) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(val);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->push_back<TdbValue>(key, val);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_pop_back_value(TdbVectorMap * map, const char * key) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->pop_back<TdbValue>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_clear_value(TdbVectorMap * map, const char * key) {
    assert(map);
    assert(map->internal);
    assert(key);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->clear<TdbValue>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_get_value_vector(TdbVectorMap * map, const char * key, TdbValue *** vec, size_t * size) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        m->getCArray<TdbValue>(key, *vec, *size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_set_value_vector(TdbVectorMap * map, const char * key, TdbValue ** vec, const size_t size) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);

    try {
        m->setCArray<TdbValue>(key, vec, size);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_is_value_vector(TdbVectorMap * map, const char * key, bool * rv) {
    assert(map);
    assert(map->internal);
    assert(key);
    assert(rv);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *rv = m->count<TdbValue>(key);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_count(TdbVectorMap * map, const char * key, bool * rv) {
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

TdbVectorMapError TdbVectorMap_erase(TdbVectorMap * map, const char * key, bool * rv) {
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

TdbVectorMapError TdbVectorMap_clear(TdbVectorMap * map) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->clear();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_set_batch(TdbVectorMap * map, const size_t n) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->setBatch(n);
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_prev_batch(TdbVectorMap * map, bool * b) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *b = m->previousBatch();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_next_batch(TdbVectorMap * map, bool * b) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *b = m->nextBatch();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_add_batch(TdbVectorMap * map) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->addBatch();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_batch_count(TdbVectorMap * map, size_t * count) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        *count = m->batchCount();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

TdbVectorMapError TdbVectorMap_reset(TdbVectorMap * map) {
    assert(map);
    assert(map->internal);

    sharemind::TdbVectorMap * m = static_cast<sharemind::TdbVectorMap *>(map->internal);
    try {
        m->reset();
        return TDB_VECTOR_MAP_OK;
    } TDB_VECTOR_MAP_CATCH_ALL;
}

uint64_t TdbVectorMap_get_id(TdbVectorMap * map) {
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

    m_wrapper.size_index = &TdbVectorMap_size_index;
    m_wrapper.at_index = &TdbVectorMap_at_index;
    m_wrapper.push_back_index = &TdbVectorMap_push_back_index;
    m_wrapper.pop_back_index = &TdbVectorMap_pop_back_index;
    m_wrapper.clear_index = &TdbVectorMap_clear_index;
    m_wrapper.get_index_vector = &TdbVectorMap_get_index_vector;
    m_wrapper.set_index_vector = &TdbVectorMap_set_index_vector;
    m_wrapper.is_index_vector = &TdbVectorMap_is_index_vector;

    m_wrapper.size_string = &TdbVectorMap_size_string;
    m_wrapper.at_string = &TdbVectorMap_at_string;
    m_wrapper.push_back_string = &TdbVectorMap_push_back_string;
    m_wrapper.pop_back_string = &TdbVectorMap_pop_back_string;
    m_wrapper.clear_string = &TdbVectorMap_clear_string;
    m_wrapper.get_string_vector = &TdbVectorMap_get_string_vector;
    m_wrapper.set_string_vector = &TdbVectorMap_set_string_vector;
    m_wrapper.is_string_vector = &TdbVectorMap_is_string_vector;

    m_wrapper.size_type = &TdbVectorMap_size_type;
    m_wrapper.at_type = &TdbVectorMap_at_type;
    m_wrapper.push_back_type = &TdbVectorMap_push_back_type;
    m_wrapper.pop_back_type = &TdbVectorMap_pop_back_type;
    m_wrapper.clear_type = &TdbVectorMap_clear_type;
    m_wrapper.get_type_vector = &TdbVectorMap_get_type_vector;
    m_wrapper.set_type_vector = &TdbVectorMap_set_type_vector;
    m_wrapper.is_type_vector = &TdbVectorMap_is_type_vector;

    m_wrapper.size_value = &TdbVectorMap_size_value;
    m_wrapper.at_value = &TdbVectorMap_at_value;
    m_wrapper.push_back_value = &TdbVectorMap_push_back_value;
    m_wrapper.pop_back_value = &TdbVectorMap_pop_back_value;
    m_wrapper.clear_value = &TdbVectorMap_clear_value;
    m_wrapper.get_value_vector = &TdbVectorMap_get_value_vector;
    m_wrapper.set_value_vector = &TdbVectorMap_set_value_vector;
    m_wrapper.is_value_vector = &TdbVectorMap_is_value_vector;

    m_wrapper.count = &TdbVectorMap_count;
    m_wrapper.erase = &TdbVectorMap_erase;
    m_wrapper.clear = &TdbVectorMap_clear;
    m_wrapper.get_id = &TdbVectorMap_get_id;

    m_wrapper.set_batch = &TdbVectorMap_set_batch;
    m_wrapper.prev_batch = &TdbVectorMap_prev_batch;
    m_wrapper.next_batch = &TdbVectorMap_next_batch;
    m_wrapper.add_batch = &TdbVectorMap_add_batch;
    m_wrapper.batch_count = &TdbVectorMap_batch_count;
    m_wrapper.reset = &TdbVectorMap_reset;
}

} /* namespace sharemind { */
