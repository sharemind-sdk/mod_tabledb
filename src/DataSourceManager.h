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

#ifndef SHAREMIND_MOD_TABLEDB_DATASOURCEMANAGER_H
#define SHAREMIND_MOD_TABLEDB_DATASOURCEMANAGER_H

#include <map>
#include <memory>
#include <sharemind/dbcommon/datasourceapi.h>
#include <sharemind/SimpleUnorderedStringMap.h>
#include <string>
#include "DataSource.h"


namespace sharemind  {

class __attribute__ ((visibility("internal"))) DataSourceManager
    : ::SharemindDataSourceManager
{

private: /* Types: */

    using Wrapper = ::SharemindDataSourceManager;
    using DataSourcesContainer =
            SimpleUnorderedStringMap<std::unique_ptr<DataSource> >;

public: /* Methods: */

    DataSourceManager();

    bool addDataSource(std::string name,
                       std::string dbModule,
                       std::string config);

    template <typename ... Args>
    DataSource * getDataSource(Args && ... args) const
            noexcept(noexcept(std::declval<DataSourcesContainer const &>().find(
                                  std::forward<Args>(args)...)))
    {
        auto const it(m_dataSources.find(std::forward<Args>(args)...));
        return (it != m_dataSources.end()) ? it->second.get() : nullptr;
    }

    static DataSourceManager & fromWrapper(Wrapper & wrapper) noexcept
    { return static_cast<DataSourceManager &>(wrapper); }

    inline Wrapper * getWrapper() { return this; }
    inline Wrapper const * getWrapper() const { return this; }

private: /* Fields: */

    DataSourcesContainer m_dataSources;

}; /* class DataSourceManager { */

} /* namespace sharemind { */

#endif /* SHAREMIND_MOD_TABLEDB_DATASOURCEMANAGER_H */
