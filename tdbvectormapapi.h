/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#ifndef SHAREMIND_MOD_TABLEDB_TDBMAPAPI_H
#define SHAREMIND_MOD_TABLEDB_TDBMAPAPI_H

#ifndef SHAREMIND_ICONST
#ifdef SHAREMIND_INTERNAL__
#define SHAREMIND_ICONST
#else
#define SHAREMIND_ICONST const
#endif
#endif /* SHAREMIND_ICONST */

#include <cstddef>
#include <stdbool.h>
#include <stdint.h>
#include <sharemind/miner/Facilities/datastoreapi.h>

#include "tdbtypes.h"


#ifdef __cplusplus
extern "C" {
#endif

/** Forward declarations: */
struct TdbVectorMapUtil_;
typedef TdbVectorMapUtil_ TdbVectorMapUtil;
struct TdbVectorMap_;
typedef TdbVectorMap_ TdbVectorMap;

/*******************************************************************************
    TdbVectorMapUtil
*******************************************************************************/

struct TdbVectorMapUtil_ {
    SHAREMIND_ICONST void * SHAREMIND_ICONST internal;

    TdbVectorMap * (* new_map)(TdbVectorMapUtil * util, DataStore * datastore);
    bool (* delete_map)(TdbVectorMapUtil * util, DataStore * datastore, const uint64_t vmapId);
    TdbVectorMap * (* get_map)(TdbVectorMapUtil * util, DataStore * datastore, const uint64_t vmapId);
};

/*******************************************************************************
    TdbVectorMap
*******************************************************************************/

enum TdbVectorMapError_ {

    /** No error. */
    TDB_VECTOR_MAP_OK = 0,

    /** A fatal out of memory condition occurred. */
    TDB_VECTOR_MAP_OUT_OF_MEMORY,

    /** General error. */
    TDB_VECTOR_MAP_GENERAL_ERROR,

    /** An unexpected exception occurred. */
    TDB_VECTOR_MAP_UNKNOWN_ERROR,

    /** No such value was found. */
    TDB_VECTOR_MAP_VALUE_NOT_FOUND,

    /** Type error. */
    TDB_VECTOR_MAP_TYPE_ERROR

};
typedef enum TdbVectorMapError_ TdbVectorMapError;

struct TdbVectorMap_ {
    SHAREMIND_ICONST void * SHAREMIND_ICONST internal;

    TdbVectorMapError (* size_index)(TdbVectorMap * map, const char * key, size_t * size);
    TdbVectorMapError (* at_index)(TdbVectorMap * map, const char * key, const uint64_t n, TdbIndex * idx);
    TdbVectorMapError (* push_back_index)(TdbVectorMap * map, const char * key, TdbIndex * idx);
    TdbVectorMapError (* pop_back_index)(TdbVectorMap * map, const char * key);
    TdbVectorMapError (* clear_index)(TdbVectorMap * map, const char * key);
    TdbVectorMapError (* get_index_vector)(TdbVectorMap * map, const char * key, TdbIndex *** vec, size_t * size);
    TdbVectorMapError (* set_index_vector)(TdbVectorMap * map, const char * key, TdbIndex ** vec, const size_t size);
    TdbVectorMapError (* is_index_vector)(TdbVectorMap * map, const char * key, bool * rv);

    TdbVectorMapError (* size_string)(TdbVectorMap * map, const char * key, size_t * size);
    TdbVectorMapError (* at_string)(TdbVectorMap * map, const char * key, const uint64_t n, TdbString * str);
    TdbVectorMapError (* push_back_string)(TdbVectorMap * map, const char * key, TdbString * str);
    TdbVectorMapError (* pop_back_string)(TdbVectorMap * map, const char * key);
    TdbVectorMapError (* clear_string)(TdbVectorMap * map, const char * key);
    TdbVectorMapError (* get_string_vector)(TdbVectorMap * map, const char * key, TdbString *** vec, size_t * size);
    TdbVectorMapError (* set_string_vector)(TdbVectorMap * map, const char * key, TdbString ** vec, const size_t size);
    TdbVectorMapError (* is_string_vector)(TdbVectorMap * map, const char * key, bool * rv);

    TdbVectorMapError (* size_type)(TdbVectorMap * map, const char * key, size_t * size);
    TdbVectorMapError (* at_type)(TdbVectorMap * map, const char * key, const uint64_t n, TdbType * type);
    TdbVectorMapError (* push_back_type)(TdbVectorMap * map, const char * key, TdbType * type);
    TdbVectorMapError (* pop_back_type)(TdbVectorMap * map, const char * key);
    TdbVectorMapError (* clear_type)(TdbVectorMap * map, const char * key);
    TdbVectorMapError (* get_type_vector)(TdbVectorMap * map, const char * key, TdbType *** vec, size_t * size);
    TdbVectorMapError (* set_type_vector)(TdbVectorMap * map, const char * key, TdbType ** vec, const size_t size);
    TdbVectorMapError (* is_type_vector)(TdbVectorMap * map, const char * key, bool * rv);

    TdbVectorMapError (* size_value)(TdbVectorMap * map, const char * key, size_t * size);
    TdbVectorMapError (* at_value)(TdbVectorMap * map, const char * key, const uint64_t n, TdbValue * val);
    TdbVectorMapError (* push_back_value)(TdbVectorMap * map, const char * key, TdbValue * val);
    TdbVectorMapError (* pop_back_value)(TdbVectorMap * map, const char * key);
    TdbVectorMapError (* clear_value)(TdbVectorMap * map, const char * key);
    TdbVectorMapError (* get_value_vector)(TdbVectorMap * map, const char * key, TdbValue *** vec, size_t * size);
    TdbVectorMapError (* set_value_vector)(TdbVectorMap * map, const char * key, TdbValue ** vec, const size_t size);
    TdbVectorMapError (* is_value_vector)(TdbVectorMap * map, const char * key, bool * rv);

    TdbVectorMapError (* count)(TdbVectorMap * map, const char * key, bool * rv);
    TdbVectorMapError (* erase)(TdbVectorMap * map, const char * key, bool * rv);
    TdbVectorMapError (* clear)(TdbVectorMap * map);

    TdbVectorMapError (* set_batch)(TdbVectorMap * map, const size_t n);
    TdbVectorMapError (* prev_batch)(TdbVectorMap * map, bool * b);
    TdbVectorMapError (* next_batch)(TdbVectorMap * map, bool * b);
    TdbVectorMapError (* add_batch)(TdbVectorMap * map);
    TdbVectorMapError (* batch_count)(TdbVectorMap * map, size_t * count);
    TdbVectorMapError (* reset)(TdbVectorMap * map);

    uint64_t (* get_id)(TdbVectorMap * map);
};

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SHAREMIND_MOD_TABLEDB_TDBMAPAPI_H */
