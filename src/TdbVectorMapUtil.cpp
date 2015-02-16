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
#include <string>
#include "TdbVectorMap.h"


namespace {
extern "C" {

SharemindTdbVectorMap * SharemindTdbVectorMapUtil_new_map(
        SharemindTdbVectorMapUtil * util,
        SharemindDataStore * datastore)
{
    assert(util);
    assert(util->internal);
    assert(datastore);

    sharemind::TdbVectorMapUtil * const u =
            static_cast<sharemind::TdbVectorMapUtil *>(util->internal);
    try {
        sharemind::TdbVectorMap * const map = u->newVectorMap(datastore);
        return map ? map->getWrapper() : nullptr;
    } catch (...) {
        return nullptr;
    }
}

bool SharemindTdbVectorMapUtil_delete_map(SharemindTdbVectorMapUtil * util,
                                          SharemindDataStore * datastore,
                                          const uint64_t vmapId)
{
    assert(util);
    assert(util->internal);
    assert(datastore);

    sharemind::TdbVectorMapUtil * const u =
            static_cast<sharemind::TdbVectorMapUtil *>(util->internal);
    try {
        return u->deleteVectorMap(datastore, vmapId);
    } catch (...) {
        return false;
    }
}

SharemindTdbVectorMap * SharemindTdbVectorMapUtil_get_map(
        SharemindTdbVectorMapUtil * util,
        SharemindDataStore * datastore,
        const uint64_t vmapId)
{
    assert(util);
    assert(util->internal);
    assert(datastore);

    sharemind::TdbVectorMapUtil * const u =
            static_cast<sharemind::TdbVectorMapUtil *>(util->internal);
    try {
        sharemind::TdbVectorMap * const map =
                u->getVectorMap(datastore, vmapId);
        return map ? map->getWrapper() : nullptr;
    } catch (...) {
        return nullptr;
    }
}

} // extern "C" {

template <class T>
void destroy(void * ptr) noexcept { delete static_cast<T *>(ptr); }

} // anonymous namespace

namespace sharemind {

TdbVectorMapUtil::TdbVectorMapUtil()
    : m_wrapper{this,
                &SharemindTdbVectorMapUtil_new_map,
                &SharemindTdbVectorMapUtil_delete_map,
                &SharemindTdbVectorMapUtil_get_map}
{}

TdbVectorMap * TdbVectorMapUtil::newVectorMap(SharemindDataStore * dataStore)
        const
{
    assert(dataStore);

    uint64_t vmapId = 0;
    std::string s;

    // Generate an unique identifier:
    do {
        s = std::to_string(++vmapId);
    } while (!!dataStore->get(dataStore, s.c_str()));

    // Store the map:
    TdbVectorMap * const map = new TdbVectorMap{vmapId};
    if (dataStore->set(dataStore, s.c_str(), map, &destroy<TdbVectorMap>))
        return map;

    delete map;
    return nullptr;
}

bool TdbVectorMapUtil::deleteVectorMap(SharemindDataStore * dataStore,
                                       const uint64_t vmapId) const
{
    assert(dataStore);
    return dataStore->remove(dataStore, std::to_string(vmapId).c_str());
}

TdbVectorMap * TdbVectorMapUtil::getVectorMap(SharemindDataStore * dataStore,
                                              const uint64_t vmapId) const
{
    assert(dataStore);
    return static_cast<TdbVectorMap *>(
                dataStore->get(dataStore, std::to_string(vmapId).c_str()));
}

} /* namespace sharemind { */
