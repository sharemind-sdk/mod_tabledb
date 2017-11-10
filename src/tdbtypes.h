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
typedef struct SharemindTdbIndex_ SharemindTdbIndex;
struct SharemindTdbString_;
typedef struct SharemindTdbString_ SharemindTdbString;
struct SharemindTdbValue_;
typedef struct SharemindTdbValue_ SharemindTdbValue;
struct SharemindTdbType_;
typedef struct SharemindTdbType_ SharemindTdbType;

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
