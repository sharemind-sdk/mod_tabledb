/*
 * Copyright (C) 2015-2017 Cybernetica
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

#ifndef SHAREMIND_MOD_TDB_TDBMODULE_H
#define SHAREMIND_MOD_TDB_TDBMODULE_H

#include <exception>
#include <LogHard/Logger.h>
#include <sharemind/datastoreapi.h>
#include <sharemind/libconsensusservice.h>
#include <sharemind/module-apis/api_0x1.h>
#include <string>
#include <utility>
#include <vector>
#include "DataSourceManager.h"
#include "ModuleLoader.h"
#include "TdbVectorMapUtil.h"
#include "tdberror.h"


namespace sharemind  {

class TdbVectorMap;

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
              SharemindConsensusFacility * consensusService,
              const std::string & config,
              std::vector<std::string> requiredSyscallSignatures);
    ~TdbModule();

    bool getErrorCode(const SharemindModuleApi0x1SyscallContext * ctx,
            const std::string & dsName,
            SharemindTdbError & code) const noexcept;

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
                         const uint64_t vmapId) noexcept;
    TdbVectorMap * getVectorMap(const SharemindModuleApi0x1SyscallContext * ctx,
                                const uint64_t vmapId) const noexcept;

    inline const LogHard::Logger & logger() const noexcept { return m_logger; }

private: /* Methods: */

    template <typename F, typename R, typename ... Args>
    inline auto dataStoreAction(
                SharemindModuleApi0x1SyscallContext const * const ctx,
                char const * const dsName,
                F action,
                R errorValue,
                Args && ... args) const
            noexcept(noexcept(action(std::declval<SharemindDataStore *>(),
                                     std::forward<Args>(args)...)))
            -> decltype(action(std::declval<SharemindDataStore *>(),
                               std::forward<Args>(args)...))
    {
        // Get factory:
        SharemindDataStoreFactory * const factory =
                static_cast<SharemindDataStoreFactory *>(
                    ctx->processFacility(ctx, "DataStoreFactory"));
        if (!factory) {
            m_logger.error() << "Failed to get process data store factory!";
            return errorValue;
        }

        // Get store
        if (SharemindDataStore * const ds =
                factory->get_datastore(factory, dsName))
            return action(ds, std::forward<Args>(args)...);

        m_logger.error() << "Failed to get process data store: " << dsName
                         << '!';
        return errorValue;
    }

private: /* Fields: */

    const LogHard::Logger m_logger;
    ModuleLoader m_dbModuleLoader;
    DataSourceManager m_dataSourceManager;
    TdbVectorMapUtil m_mapUtil;

}; /* class TdbModule { */

} /* namespace sharemind { */

#endif /* SHAREMIND_MOD_TDB_TDBMODULE_H */
