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

#include <cstddef>
#ifdef __cplusplus
#include <cstdint>
#endif
#include <sharemind/miner/Facilities/datastoreapi.h>
#ifndef __cplusplus
#include <stdbool.h>
#include <stdint.h>
#endif
#include "tdbtypes.h"


#ifndef SHAREMIND_ICONST
#ifdef SHAREMIND_INTERNAL__
#define SHAREMIND_ICONST
#else
#define SHAREMIND_ICONST const
#endif
#endif /* SHAREMIND_ICONST */

#ifdef __cplusplus
extern "C" {
#endif

/** Forward declarations: */
struct SharemindTdbVectorMapUtil_;
typedef SharemindTdbVectorMapUtil_ SharemindTdbVectorMapUtil;
struct SharemindTdbVectorMap_;
typedef SharemindTdbVectorMap_ SharemindTdbVectorMap;

/*******************************************************************************
    SharemindTdbVectorMapUtil
*******************************************************************************/

struct SharemindTdbVectorMapUtil_ {
    SHAREMIND_ICONST void * SHAREMIND_ICONST internal;

    SharemindTdbVectorMap * (* new_map)(SharemindTdbVectorMapUtil * util, SharemindDataStore * datastore);
    bool (* delete_map)(SharemindTdbVectorMapUtil * util, SharemindDataStore * datastore, const uint64_t vmapId);
    SharemindTdbVectorMap * (* get_map)(SharemindTdbVectorMapUtil * util, SharemindDataStore * datastore, const uint64_t vmapId);
};

/*******************************************************************************
    SharemindTdbVectorMap
*******************************************************************************/

enum SharemindTdbVectorMapError_ {

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
typedef enum SharemindTdbVectorMapError_ SharemindTdbVectorMapError;

struct SharemindTdbVectorMap_ {
    SHAREMIND_ICONST void * SHAREMIND_ICONST internal;

    SharemindTdbVectorMapError (* get_index_vector)(SharemindTdbVectorMap * map, const char * key, SharemindTdbIndex *** vec, size_t * size);
    SharemindTdbVectorMapError (* set_index_vector)(SharemindTdbVectorMap * map, const char * key, SharemindTdbIndex ** vec, const size_t size);
    SharemindTdbVectorMapError (* is_index_vector)(SharemindTdbVectorMap * map, const char * key, bool * rv);

    SharemindTdbVectorMapError (* get_string_vector)(SharemindTdbVectorMap * map, const char * key, SharemindTdbString *** vec, size_t * size);
    SharemindTdbVectorMapError (* set_string_vector)(SharemindTdbVectorMap * map, const char * key, SharemindTdbString ** vec, const size_t size);
    SharemindTdbVectorMapError (* is_string_vector)(SharemindTdbVectorMap * map, const char * key, bool * rv);

    SharemindTdbVectorMapError (* get_type_vector)(SharemindTdbVectorMap * map, const char * key, SharemindTdbType *** vec, size_t * size);
    SharemindTdbVectorMapError (* set_type_vector)(SharemindTdbVectorMap * map, const char * key, SharemindTdbType ** vec, const size_t size);
    SharemindTdbVectorMapError (* is_type_vector)(SharemindTdbVectorMap * map, const char * key, bool * rv);

    SharemindTdbVectorMapError (* get_value_vector)(SharemindTdbVectorMap * map, const char * key, SharemindTdbValue *** vec, size_t * size);
    SharemindTdbVectorMapError (* set_value_vector)(SharemindTdbVectorMap * map, const char * key, SharemindTdbValue ** vec, const size_t size);
    SharemindTdbVectorMapError (* is_value_vector)(SharemindTdbVectorMap * map, const char * key, bool * rv);

    SharemindTdbVectorMapError (* count)(SharemindTdbVectorMap * map, const char * key, bool * rv);
    SharemindTdbVectorMapError (* erase)(SharemindTdbVectorMap * map, const char * key, bool * rv);
    SharemindTdbVectorMapError (* clear)(SharemindTdbVectorMap * map);

    SharemindTdbVectorMapError (* set_batch)(SharemindTdbVectorMap * map, const size_t n);
    SharemindTdbVectorMapError (* add_batch)(SharemindTdbVectorMap * map);
    SharemindTdbVectorMapError (* batch_count)(SharemindTdbVectorMap * map, size_t * count);
    SharemindTdbVectorMapError (* reset)(SharemindTdbVectorMap * map);

    uint64_t (* get_id)(SharemindTdbVectorMap * map);
};

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SHAREMIND_MOD_TABLEDB_TDBMAPAPI_H */
