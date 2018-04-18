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
    SHAREMIND_TDB_TABLE_ALREADY_EXISTS,

    /** Missing facility. */
    SHAREMIND_TDB_MISSING_FACILITY

};
typedef enum SharemindTdbError_ SharemindTdbError;

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SHAREMIND_MOD_TABLEDB_TDBERRORCODE_H */
