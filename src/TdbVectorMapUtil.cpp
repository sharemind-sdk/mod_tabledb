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

#include "TdbVectorMapUtil.h"

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
                                       const uint64_t vmapId) const noexcept
{
    assert(dataStore);
    return dataStore->remove(dataStore, std::to_string(vmapId).c_str());
}

TdbVectorMap * TdbVectorMapUtil::getVectorMap(
        SharemindDataStore * dataStore,
        const uint64_t vmapId) const noexcept
{
    assert(dataStore);
    return static_cast<TdbVectorMap *>(
                dataStore->get(dataStore, std::to_string(vmapId).c_str()));
}

} /* namespace sharemind { */
