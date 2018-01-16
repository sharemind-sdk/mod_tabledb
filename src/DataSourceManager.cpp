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

#include "DataSourceManager.h"

#include <cassert>
#include <sharemind/MakeUnique.h>
#include "DataSource.h"


extern "C" {

SharemindDataSource * SharemindDataSourceManager_get_source(
        SharemindDataSourceManager * mgr,
        const char * name)
{
    assert(mgr);
    assert(name);
    try {
        auto & m = sharemind::DataSourceManager::fromWrapper(*mgr);
        auto * const src = m.getDataSource(name);
        return src ? src->getWrapper() : nullptr;
    } catch (...) {
        return nullptr;
    }
}

} /* extern "C" { */


namespace sharemind  {

DataSourceManager::DataSourceManager()
    : ::SharemindDataSourceManager{&SharemindDataSourceManager_get_source}
{}

bool DataSourceManager::addDataSource(std::string name,
                                      std::string dbModule,
                                      std::string config)
{
    auto ds(makeUnique<DataSource>(std::move(name),
                                   std::move(dbModule),
                                   std::move(config)));
    return m_dataSources.emplace(ds->name(), std::move(ds)).second;
}

DataSource * DataSourceManager::getDataSource(std::string const & name) const {
    auto const it(m_dataSources.find(name));
    return (it != m_dataSources.end()) ? it->second.get() : nullptr;
}

} /* namespace sharemind { */
