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

#include "TdbModule.h"

#include <sstream>
#include "DataSource.h"
#include "TdbConfiguration.h"
#include "TdbVectorMap.h"


namespace sharemind {

TdbModule::TdbModule(const LogHard::Logger & logger,
                     SharemindConsensusFacility * consensusService,
                     const std::string & config,
                     std::vector<std::string> requiredSyscallSignatures)
    : m_logger(logger, "[TdbModule]")
    , m_dbModuleLoader(std::move(requiredSyscallSignatures), m_logger)
{
    // Load module configuration
    std::unique_ptr<TdbConfiguration> configuration;
    try {
        configuration = makeUnique<TdbConfiguration>(config);
    } catch (...) {
        std::throw_with_nested(
                    ConfigurationException("Failed to parse configuration!"));
    }

    // Set database module facilities
    #define SET_FACILITY(n,w) \
        try { \
            m_dbModuleLoader.setModuleFacility((n), (w)); \
        } catch (...) { \
            std::throw_with_nested( \
                InitializationException( \
                    "Failed setting module facility \"" n "\"!")); \
        }
    SET_FACILITY("Logger", &const_cast<LogHard::Logger &>(m_logger));
    SET_FACILITY("DataSourceManager", m_dataSourceManager.getWrapper());
    SET_FACILITY("TdbVectorMapUtil", m_mapUtil.getWrapper());
    if (consensusService) {
        SET_FACILITY("ConsensusService", consensusService);
    }
    #undef SET_FACILITY

    // Load database modules
    for (auto const & cfgDbMod : configuration->dbModuleList()) {
        SharemindModule * const m = m_dbModuleLoader.addModule(
                                            cfgDbMod.filename,
                                            cfgDbMod.configurationFile);
        if (!m)
            throw InitializationException("Failed to load database modules!");

        m_logger.info()
            << "Loaded database module \"" << SharemindModule_name(m)
            << "\" (" << SharemindModule_numSyscalls(m)
            << " syscalls) from \"" << cfgDbMod.filename
            << "\" using API version " << SharemindModule_apiVersionInUse(m)
            << '.';
    }

    // Load data sources
    for (auto const & cfgDs : configuration->dataSourceList()) {
        if (!m_dbModuleLoader.hasModule(cfgDs.dbModule)) {
            m_logger.error() << "Data source \"" << cfgDs.name
                             << "\" uses an unknown database module: \""
                             << cfgDs.dbModule << "\".";
            throw ConfigurationException("Configuration contained unknown "
                                         "module references!");
        }

        if (!m_dataSourceManager.addDataSource(cfgDs.name,
                                               cfgDs.dbModule,
                                               cfgDs.configurationFile))
        {
            m_logger.error() << "Data source \"" << cfgDs.name
                             << "\" has duplicate configuration entries.";
            throw ConfigurationException("Configuration contained duplicate "
                                         "configuration entries!");
        }
    }

}

TdbModule::~TdbModule() { }

bool TdbModule::getErrorCode(
        const SharemindModuleApi0x1SyscallContext * ctx,
        const std::string & dsName,
        SharemindTdbError & code) const noexcept
{
    return dataStoreAction(
                ctx,
                "mod_tabledb/errors",
                [&dsName, &code](SharemindDataStore * const errors) noexcept {
                    SharemindTdbError const * const e =
                            static_cast<SharemindTdbError *>(
                                errors->get(errors, dsName.c_str()));
                    code = e ? *e : SHAREMIND_TDB_OK;
                    return true;
                },
                false);
}

SharemindModuleApi0x1Error TdbModule::doSyscall(const std::string & dsName,
                                                const std::string & signature,
                                                SharemindCodeBlock * args,
                                                size_t num_args,
                                                const SharemindModuleApi0x1Reference * refs,
                                                const SharemindModuleApi0x1CReference * crefs,
                                                SharemindCodeBlock * returnValue,
                                                SharemindModuleApi0x1SyscallContext * c) const
{
    // Get the data source object
    DataSource * const src = m_dataSourceManager.getDataSource(dsName);
    if (!src) {
        m_logger.error() << "Data source \"" << dsName << "\" is not defined.";
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    }

    // Get the system call object
    const SharemindSyscallWrapper sw =
            m_dbModuleLoader.getSyscall(src->module(), signature);
    if (!sw.callable) {
        m_logger.error()
            << "Data source \"" << dsName << "\" database module \""
            << src->module() << "\" has no system call with signature \""
            << signature << "\".";
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    }

    // Do the system call
    SharemindSyscallContext sc = *c;
    sc.moduleHandle = sw.internal;

    return (*(sw.callable))(args, num_args, refs, crefs, returnValue, &sc);
}

bool TdbModule::newVectorMap(const SharemindModuleApi0x1SyscallContext * ctx,
                             uint64_t & stmtId)
{
    return dataStoreAction(ctx,
                           "mod_tabledb/vector_maps",
                           [this, &stmtId](SharemindDataStore * const maps) {
                               if (TdbVectorMap * const map =
                                       m_mapUtil.newVectorMap(maps))
                               {
                                   stmtId = map->getId();
                                   return true;
                               }
                               return false;
                           },
                           false);
}

bool TdbModule::deleteVectorMap(const SharemindModuleApi0x1SyscallContext * ctx,
                                const uint64_t stmtId) noexcept
{
    return dataStoreAction(
                ctx,
                "mod_tabledb/vector_maps",
                [this, stmtId](SharemindDataStore * const maps) noexcept
                { return m_mapUtil.deleteVectorMap(maps, stmtId); },
                false);
}

TdbVectorMap * TdbModule::getVectorMap(const SharemindModuleApi0x1SyscallContext * ctx,
                                       const uint64_t stmtId) const noexcept
{
    return dataStoreAction(
                ctx,
                "mod_tabledb/vector_maps",
                [this, stmtId](SharemindDataStore * const maps) noexcept
                { return m_mapUtil.getVectorMap(maps, stmtId); },
                nullptr);
}

} /* namespace sharemind { */
