/*
 * Copyright (C) 2017 Cybernetica
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

#ifndef SHAREMIND_MOD_TABLEDB_TDBTYPESUTIL_H
#define SHAREMIND_MOD_TABLEDB_TDBTYPESUTIL_H

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#endif
#include "tdbtypes.h"


#ifdef __cplusplus
extern "C" {
#endif

SharemindTdbIndex * SharemindTdbIndex_new(uint64_t const idx);

void SharemindTdbIndex_delete(SharemindTdbIndex const * ptr);

SharemindTdbString * SharemindTdbString_new(char const * str);

SharemindTdbString * SharemindTdbString_new2(char const * str, size_t strSize);

void SharemindTdbString_delete(SharemindTdbString * ptr);

SharemindTdbType * SharemindTdbType_new(char const * domain,
                                        char const * name,
                                        uint64_t const size);

SharemindTdbType * SharemindTdbType_new2(char const * domain,
                                         size_t domainSize,
                                         char const * name,
                                         size_t nameSize,
                                         uint64_t const size);

void SharemindTdbType_delete(SharemindTdbType * ptr);

SharemindTdbValue * SharemindTdbValue_new(char const * typeDomain,
                                          char const * typeName,
                                          uint64_t const typeSize,
                                          void const * buffer,
                                          uint64_t const size);

SharemindTdbValue * SharemindTdbValue_new2(char const * typeDomain,
                                           size_t typeDomainSize,
                                           char const * typeName,
                                           size_t typeNameSize,
                                           uint64_t const typeSize,
                                           void const * buffer,
                                           uint64_t const size);

void SharemindTdbValue_delete(SharemindTdbValue * ptr);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SHAREMIND_MOD_TABLEDB_TDBTYPESUTIL_H */
