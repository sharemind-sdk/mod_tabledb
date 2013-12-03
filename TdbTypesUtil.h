/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#ifndef SHAREMIND_MOD_TABLEDB_TDBTYPESUTIL_H
#define SHAREMIND_MOD_TABLEDB_TDBTYPESUTIL_H

#include <string>

#include "tdbtypes.h"


namespace sharemind {

TdbIndex * TdbIndex_new(const uint64_t idx);
void TdbIndex_delete(const TdbIndex * ptr);

TdbString * TdbString_new(const std::string & str);
TdbString * TdbString_new(const char * str);
void TdbString_delete(const TdbString * ptr);

TdbType * TdbType_new(const std::string & domain, const std::string & name, const uint64_t size);
TdbType * TdbType_new(const char * domain, const char * name, const uint64_t size);
void TdbType_delete(const TdbType * ptr);

TdbValue * TdbValue_new(const std::string & typeDomain, const std::string & typeName, const uint64_t typeSize, const void * buffer, const uint64_t size);
TdbValue * TdbValue_new(const char * typeDomain, const char * typeName, const uint64_t typeSize, const void * buffer, const uint64_t size);
void TdbValue_delete(const TdbValue * ptr);

}; /* namespace sharemind { */

#endif /* SHAREMIND_MOD_TABLEDB_TDBTYPESUTIL_H */
