/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#ifndef SHAREMIND_MOD_TABLEDB_TDBERRORCODE_H
#define SHAREMIND_MOD_TABLEDB_TDBERRORCODE_H

#ifdef __cplusplus
extern "C" {
#endif

enum SharemindTdbError_ {

    /** No error. */
    SHAREMIND_TDB_OK = 0,

    /** An unexpected exception occurred. */
    SHAREMIND_TDB_UNKNOWN_ERROR,

    /** An general error occurred. */
    SHAREMIND_TDB_GENERAL_ERROR,

    /** Could not reach consensus. */
    SHAREMIND_TDB_CONSENSUS_ERROR,

    /** An IO error occurred. */
    SHAREMIND_TDB_IO_ERROR,

    /** An invalid argument was given to the function. */
    SHAREMIND_TDB_INVALID_ARGUMENT,

    /** No such table was found. */
    SHAREMIND_TDB_TABLE_NOT_FOUND,

    /** Cannot create table, already exists. */
    SHAREMIND_TDB_TABLE_ALREADY_EXISTS

};
typedef enum SharemindTdbError_ SharemindTdbError;

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SHAREMIND_MOD_TABLEDB_TDBERRORCODE_H */
