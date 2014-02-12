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

#include "TdbConfiguration.h"

#include <memory>
#include <set>
#include <sharemind/common/Logger/Debug.h>
#include <sharemind/common/Logger/ILogger.h>
#include <sharemind/dbcommon/ModuleLoader.h>
#include <sharemind/libmodapi/api_0x1.h>
#include <sharemind/miner/Facilities/datastoreapi.h>
#include <sharemind/miner/Facilities/libconsensusservice.h>
#include <sharemind/miner/Facilities/libprocessfacility.h>
#include <stdexcept>


namespace sharemind  {

class DataSourceManager;
class TdbVectorMap;
class TdbVectorMapUtil;

class __attribute__ ((visibility("internal"))) TdbModule {

public: /* Types: */

    typedef ILogger::Wrapped Logger;

    class Exception: public std::exception {};

    class InitializationException: public Exception {

    public: /* Methods: */

        inline InitializationException(const char * const msg)
            : m_errorStr(msg) {}

        inline virtual const char * what() const noexcept {
            return m_errorStr;
        }

        const char * const m_errorStr;

    };

    class ConfigurationException: public InitializationException {

    public: /* Methods: */

        inline ConfigurationException(const char * const msg)
            : InitializationException(msg) {}

    };

public: /* Methods: */

    TdbModule(ILogger & logger,
              SharemindDataStoreManager & dataStoreManager,
              SharemindConsensusFacility & consensusService,
              SharemindProcessFacility & processFacility,
              const std::string & config,
              const std::set<std::string> & signatures);
    ~TdbModule();

    SharemindModuleApi0x1Error doSyscall(const std::string & dsName,
                                         const std::string & signature,
                                         SharemindCodeBlock * args,
                                         size_t num_args,
                                         const SharemindModuleApi0x1Reference * refs,
                                         const SharemindModuleApi0x1CReference * crefs,
                                         SharemindCodeBlock * returnValue,
                                         SharemindModuleApi0x1SyscallContext * c) const;

    bool newVectorMap(const SharemindModuleApi0x1SyscallContext * ctx,
                      uint64_t & vmapId);
    bool deleteVectorMap(const SharemindModuleApi0x1SyscallContext * ctx,
                         const uint64_t vmapId);
    TdbVectorMap * getVectorMap(const SharemindModuleApi0x1SyscallContext * ctx,
                                const uint64_t vmapId) const;

    inline Logger & logger() { return m_logger; }
    inline const Logger & logger() const { return m_logger; }

    inline SharemindDataStoreManager & dataStoreManager() { return m_dataStoreManager; }
    inline const SharemindDataStoreManager & dataStoreManager() const { return m_dataStoreManager; }

private: /* Fields: */

    mutable Logger m_logger;

    /* Cached references: */
    SharemindDataStoreManager & m_dataStoreManager;

    TdbConfiguration m_configuration;
    const std::unique_ptr<moduleLoader::ModuleLoader<Logger> > m_dbModuleLoader;
    const std::unique_ptr<DataSourceManager> m_dataSourceManager;
    const std::unique_ptr<TdbVectorMapUtil> m_mapUtil;

}; /* class TdbModule { */

} /* namespace sharemind { */

#endif /* SHAREMIND_MOD_TDB_TDBMODULE_H */
