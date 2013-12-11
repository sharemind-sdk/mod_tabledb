 /*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#ifndef SHAREMIND_MOD_TDB_TDBMODULE_H
#define SHAREMIND_MOD_TDB_TDBMODULE_H

#include <set>
#include <stdexcept>
#include <boost/scoped_ptr.hpp>
#include <sharemind/common/Logger/Debug.h>
#include <sharemind/common/Logger/ILogger.h>
#include <sharemind/libmodapi/api_0x1.h>
#include <sharemind/miner/Facilities/datastoreapi.h>
#include "TdbConfiguration.h"


namespace sharemind  {

class DataSourceManager;
class IRandom;
class TdbVectorMap;
class TdbVectorMapUtil;

namespace moduleLoader {

class ModuleLoader;

} /* namespace moduleLoader { */

class __attribute__ ((visibility("internal"))) TdbModule {

public: /* Types: */

    class Exception: public std::runtime_error {

    public: /* Methods: */

        inline Exception(const std::string & msg)
            : std::runtime_error(msg) {}

    };

    class ConfigurationException: public Exception {

    public: /* Methods: */

        inline ConfigurationException(const std::string & msg)
            : Exception(msg) {}

    };

    class InitializationException: public Exception {

    public: /* Methods: */

        inline InitializationException(const std::string & msg)
            : Exception(msg) {}

    };

public: /* Methods: */

    TdbModule(ILogger & logger, IRandom & rng, SharemindDataStoreManager & dataStoreManager, const std::string & config, const std::set<std::string> & signatures);
    ~TdbModule();

    SharemindModuleApi0x1Error doSyscall(const std::string & dsName,
                                         const std::string & signature,
                                         SharemindCodeBlock * args,
                                         size_t num_args,
                                         const SharemindModuleApi0x1Reference * refs,
                                         const SharemindModuleApi0x1CReference * crefs,
                                         SharemindCodeBlock * returnValue,
                                         SharemindModuleApi0x1SyscallContext * c) const;

    bool newVectorMap(const void * process, uint64_t & vmapId);
    bool deleteVectorMap(const void * process, const uint64_t vmapId);
    TdbVectorMap * getVectorMap(const void * process, const uint64_t vmapId) const;

    inline ILogger::Wrapped & logger() { return m_logger; }
    inline const ILogger::Wrapped & logger() const { return m_logger; }

    inline SharemindDataStoreManager & dataStoreManager() { return m_dataStoreManager; }
    inline const SharemindDataStoreManager & dataStoreManager() const { return m_dataStoreManager; }

private: /* Fields: */

    mutable ILogger::Wrapped m_logger;

    /* Cached references: */
    SharemindDataStoreManager & m_dataStoreManager;

    TdbConfiguration m_configuration;
    boost::scoped_ptr<moduleLoader::ModuleLoader> m_dbModuleLoader;
    boost::scoped_ptr<DataSourceManager> m_dataSourceManager;
    boost::scoped_ptr<TdbVectorMapUtil> m_mapUtil;

}; /* class TdbModule { */

} /* namespace sharemind { */

#endif /* SHAREMIND_MOD_TDB_TDBMODULE_H */
