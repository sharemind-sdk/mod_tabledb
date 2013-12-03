/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#ifndef SHAREMIND_MOD_TABLEDB_TDBVECTORMAPUTIL_H
#define SHAREMIND_MOD_TABLEDB_TDBVECTORMAPUTIL_H

#ifndef SHAREMIND_INTERNAL__
#error SHAREMIND_INTERNAL__ not defined
#endif

#include "tdbvectormapapi.h"


namespace sharemind {

class IRandom;
class TdbVectorMap;

class __attribute__ ((visibility("internal"))) TdbVectorMapUtil {
private: /* Types: */

    typedef ::TdbVectorMapUtil TdbVectorMapUtilWrapper;

public: /* Methods: */

    TdbVectorMapUtil(IRandom & rng);

    TdbVectorMap * newVectorMap(DataStore * dataStore) const;
    bool deleteVectorMap(DataStore * dataStore, const uint64_t vmapid) const;
    TdbVectorMap * getVectorMap(DataStore * dataStore, const uint64_t vmapId) const;

    inline TdbVectorMapUtilWrapper * getWrapper() { return &m_wrapper; }
    inline const TdbVectorMapUtilWrapper * getWrapper() const { return &m_wrapper; }

private: /* Fields: */

    TdbVectorMapUtilWrapper m_wrapper;
    IRandom & m_rng;

}; /* class TdbVectorMapUtil { */

} /* namespace sharemind { */

#endif /* SHAREMIND_MOD_TABLEDB_TDBVECTORMAPUTIL_H */
