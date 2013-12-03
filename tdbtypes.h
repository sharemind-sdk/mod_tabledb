/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#ifndef SHAREMIND_MOD_TABLEDB_TDBTYPES_H
#define SHAREMIND_MOD_TABLEDB_TDBTYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct TdbIndex_;
typedef TdbIndex_ TdbIndex;
struct TdbString_;
typedef TdbString_ TdbString;
struct TdbValue_;
typedef TdbValue_ TdbValue;
struct TdbType_;
typedef TdbType_ TdbType;

struct TdbIndex_ {
    uint64_t idx;
};

struct TdbString_ {
    char * str;
};

struct TdbType_ {
    char * domain;
    char * name;
    uint64_t size;
};

struct TdbValue_ {
    TdbType * type;
    void * buffer;
    uint64_t size;
};

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SHAREMIND_MOD_TABLEDB_TDBTYPES_H */
