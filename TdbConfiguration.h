 /*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#ifndef SHAREMIND_MOD_TABLEDB_TDBCONFIGURATION_H
#define SHAREMIND_MOD_TABLEDB_TDBCONFIGURATION_H

#include <map>
#include <string>
#include <vector>


namespace sharemind {

class __attribute__ ((visibility("internal"))) TdbConfiguration {

public: /* Types: */

    struct DbModuleEntry {
        std::string filename;
        std::string configurationFile;
    };

    struct DataSourceEntry {
        std::string name;
        std::string dbModule;
        std::string configurationFile;
    };

public: /* Methods: */

    /**
      Loads the configuration from the given file.
      \param[in] filename the file that contains the configuration.
      \returns whether the configuration was successfully loaded.
    */
    bool load(const std::string & filename);

    inline const std::vector<DbModuleEntry> & dbModuleList() const { return m_dbModuleList; }
    inline const std::vector<DataSourceEntry> & dataSourceList() const { return m_dataSourceList; }

    inline const std::string & lastErrorMessage() { return m_lastErrorMessage; }

private: /* Fields: */

    std::vector<DbModuleEntry> m_dbModuleList;
    std::vector<DataSourceEntry> m_dataSourceList;

    std::string m_lastErrorMessage;

}; /* class TdbConfiguration { */

} /* namespace sharemind { */

#endif /* SHAREMIND_MOD_TABLEDB_TDBCONFIGURATION_H */
