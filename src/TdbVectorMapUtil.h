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

#ifndef SHAREMIND_MOD_TABLEDB_TDBVECTORMAPUTIL_H
#define SHAREMIND_MOD_TABLEDB_TDBVECTORMAPUTIL_H

#ifndef SHAREMIND_INTERNAL__
#error SHAREMIND_INTERNAL__ not defined
#endif

#include "tdbvectormapapi.h"


namespace sharemind {

class TdbVectorMap;

class __attribute__ ((visibility("internal"))) TdbVectorMapUtil {

private: /* Types: */

    typedef SharemindTdbVectorMapUtil TdbVectorMapUtilWrapper;

public: /* Methods: */

    TdbVectorMapUtil();

    TdbVectorMap * newVectorMap(SharemindDataStore * dataStore) const;

    bool deleteVectorMap(SharemindDataStore * dataStore,
                         const uint64_t vmapid) const;

    TdbVectorMap * getVectorMap(SharemindDataStore * dataStore,
                                const uint64_t vmapId) const;

    inline TdbVectorMapUtilWrapper * getWrapper() { return &m_wrapper; }

    inline const TdbVectorMapUtilWrapper * getWrapper() const
    { return &m_wrapper; }

private: /* Fields: */

    TdbVectorMapUtilWrapper m_wrapper;

}; /* class TdbVectorMapUtil { */

} /* namespace sharemind { */

#endif /* SHAREMIND_MOD_TABLEDB_TDBVECTORMAPUTIL_H */
