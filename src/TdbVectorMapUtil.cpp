/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#define SHAREMIND_INTERNAL__
#include "TdbVectorMapUtil.h"

#include <sharemind/Random/IRandom.h>
#include <sstream>
#include "TdbVectorMap.h"


#ifdef __cplusplus
extern "C" {
#endif

SharemindTdbVectorMap * SharemindTdbVectorMapUtil_new_map(SharemindTdbVectorMapUtil * util, SharemindDataStore * datastore) {
    assert(util);
    assert(util->internal);
    assert(datastore);

    sharemind::TdbVectorMapUtil * u = static_cast<sharemind::TdbVectorMapUtil *>(util->internal);
    try {
        sharemind::TdbVectorMap * map = u->newVectorMap(datastore);
        if (!map)
            return nullptr;

        return map->getWrapper();
    } catch (...) {
        return nullptr;
    }
}

bool SharemindTdbVectorMapUtil_delete_map(SharemindTdbVectorMapUtil * util, SharemindDataStore * datastore, const uint64_t vmapId) {
    assert(util);
    assert(util->internal);
    assert(datastore);

    sharemind::TdbVectorMapUtil * u = static_cast<sharemind::TdbVectorMapUtil *>(util->internal);
    try {
        return u->deleteVectorMap(datastore, vmapId);
    } catch (...) {
        return false;
    }
}

SharemindTdbVectorMap * SharemindTdbVectorMapUtil_get_map(SharemindTdbVectorMapUtil * util, SharemindDataStore * datastore, const uint64_t vmapId) {
    assert(util);
    assert(util->internal);
    assert(datastore);

    sharemind::TdbVectorMapUtil * u = static_cast<sharemind::TdbVectorMapUtil *>(util->internal);
    try {
        sharemind::TdbVectorMap * map = u->getVectorMap(datastore, vmapId);
        if (!map)
            return nullptr;

        return map->getWrapper();
    } catch (...) {
        return nullptr;
    }
}

#ifdef __cplusplus
} /* extern "C" { */
#endif

namespace {

template <class T>
void destroy(void * ptr) noexcept { delete static_cast<T *>(ptr); }

} /* namespace { */

namespace sharemind {

TdbVectorMapUtil::TdbVectorMapUtil()
{
    m_wrapper.internal = this;
    m_wrapper.new_map = &SharemindTdbVectorMapUtil_new_map;
    m_wrapper.delete_map = &SharemindTdbVectorMapUtil_delete_map;
    m_wrapper.get_map = &SharemindTdbVectorMapUtil_get_map;
}

TdbVectorMap * TdbVectorMapUtil::newVectorMap(SharemindDataStore * dataStore) const {
    assert(dataStore);

    uint64_t vmapId = 0;
    std::ostringstream oss; // TODO for C++11 replace with std::to_string()

    // Generate an identifier
    do {
        oss.str(""); oss.clear(); oss << ++vmapId;
    } while (!!dataStore->get(dataStore, oss.str().c_str()));

    // Store the map
    TdbVectorMap * map = new TdbVectorMap(vmapId);
    if (!dataStore->set(dataStore, oss.str().c_str(), map, &destroy<TdbVectorMap>)) {
        delete map;
        return nullptr;
    }

    return map;
}

bool TdbVectorMapUtil::deleteVectorMap(SharemindDataStore * dataStore, const uint64_t vmapId) const {
    assert(dataStore);

    std::ostringstream oss;
    oss << vmapId;

    // Store the map
    return dataStore->remove(dataStore, oss.str().c_str());
}

TdbVectorMap * TdbVectorMapUtil::getVectorMap(SharemindDataStore * dataStore, const uint64_t vmapId) const {
    assert(dataStore);

    std::ostringstream oss;
    oss << vmapId;

    // Get the map from the store
    return static_cast<TdbVectorMap *>(dataStore->get(dataStore, oss.str().c_str()));
}

} /* namespace sharemind { */
