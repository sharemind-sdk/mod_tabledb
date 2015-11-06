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

#include <LogHard/Logger.h>
#include <memory>
#include <set>
#include <sharemind/datastoreapi.h>
#include <sharemind/dbcommon/ModuleLoader.h>
#include <sharemind/libmodapi/api_0x1.h>
#include <sharemind/miner/libconsensusservice.h>
#include <stdexcept>
#include "tdberror.h"


namespace sharemind  {

class DataSourceManager;
class TdbVectorMap;
class TdbVectorMapUtil;

class __attribute__ ((visibility("internal"))) TdbModule {

public: /* Types: */

    class Exception: public std::exception {};

    class InitializationException: public Exception {

    public: /* Methods: */

        inline InitializationException(const char * const msg)
            : m_errorStr(msg) {}

        inline const char * what() const noexcept override {
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

    TdbModule(const LogHard::Logger & logger,
              SharemindDataStoreManager & dataStoreManager,
              SharemindConsensusFacility & consensusService,
              const std::string & config,
              const std::set<std::string> & signatures);
    ~TdbModule();

    bool getErrorCode(const SharemindModuleApi0x1SyscallContext * ctx,
            const std::string & dsName,
            SharemindTdbError & code) const;

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

    inline const LogHard::Logger & logger() const noexcept { return m_logger; }

    inline SharemindDataStoreManager & dataStoreManager() { return m_dataStoreManager; }
    inline const SharemindDataStoreManager & dataStoreManager() const { return m_dataStoreManager; }

private: /* Fields: */

    const LogHard::Logger m_logger;

    /* Cached references: */
    SharemindDataStoreManager & m_dataStoreManager;

    TdbConfiguration m_configuration;
    const std::unique_ptr<moduleLoader::ModuleLoader> m_dbModuleLoader;
    const std::unique_ptr<DataSourceManager> m_dataSourceManager;
    const std::unique_ptr<TdbVectorMapUtil> m_mapUtil;

}; /* class TdbModule { */

} /* namespace sharemind { */

#endif /* SHAREMIND_MOD_TDB_TDBMODULE_H */
