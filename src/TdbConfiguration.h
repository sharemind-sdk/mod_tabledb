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

#ifndef SHAREMIND_MOD_TABLEDB_TDBCONFIGURATION_H
#define SHAREMIND_MOD_TABLEDB_TDBCONFIGURATION_H

#include <map>
#include <sharemind/ConfigurationInterpolation.h>
#include <string>
#include <vector>


namespace sharemind {

class __attribute__ ((visibility("internal"))) TdbConfiguration {

public: /* Types: */

    struct DbModuleEntry {
        std::string filename;
        std::string configurationFile;
    };
    using DbModuleList = std::vector<DbModuleEntry>;

    struct DataSourceEntry {
        std::string name;
        std::string dbModule;
        std::string configurationFile;
    };
    using DataSourceList = std::vector<DataSourceEntry>;

public: /* Methods: */

    /**
      Loads the configuration from the given file.
      \param[in] filename the file that contains the configuration.
      \returns whether the configuration was successfully loaded.
    */
    bool load(std::string const & filename);

    inline DbModuleList const & dbModuleList() const
    { return m_dbModuleList; }

    inline DataSourceList const & dataSourceList() const
    { return m_dataSourceList; }

    inline const std::string & lastErrorMessage() { return m_lastErrorMessage; }

private: /* Fields: */

    ConfigurationInterpolation m_interpolate;
    DbModuleList m_dbModuleList;
    DataSourceList m_dataSourceList;

    std::string m_lastErrorMessage;

}; /* class TdbConfiguration { */

} /* namespace sharemind { */

#endif /* SHAREMIND_MOD_TABLEDB_TDBCONFIGURATION_H */
