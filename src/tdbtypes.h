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

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

struct SharemindTdbIndex_;
typedef SharemindTdbIndex_ SharemindTdbIndex;
struct SharemindTdbString_;
typedef SharemindTdbString_ SharemindTdbString;
struct SharemindTdbValue_;
typedef SharemindTdbValue_ SharemindTdbValue;
struct SharemindTdbType_;
typedef SharemindTdbType_ SharemindTdbType;

struct SharemindTdbIndex_ {
    uint64_t idx;
};

struct SharemindTdbString_ {
    char * str;
};

struct SharemindTdbType_ {
    char * domain;
    char * name;
    uint64_t size;
};

struct SharemindTdbValue_ {
    SharemindTdbType * type;
    void * buffer;
    uint64_t size;
};

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SHAREMIND_MOD_TABLEDB_TDBTYPES_H */
