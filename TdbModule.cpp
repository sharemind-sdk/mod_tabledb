/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#include "TdbModule.h"

#include <sstream>

#define SHAREMIND_INTERNAL__
#include <sharemind/dbcommon/DataSource.h>
#include <sharemind/dbcommon/DataSourceManager.h>

#include "TdbVectorMap.h"
#include "TdbVectorMapUtil.h"


namespace sharemind {

TdbModule::TdbModule(const LogHard::Logger & logger,
                     SharemindDataStoreManager & dataStoreManager,
                     SharemindConsensusFacility & consensusService,
                     const std::string & config,
                     const std::set<std::string> & signatures)
    : m_logger(logger, "[TdbModule]")
    , m_dataStoreManager(dataStoreManager)
    , m_dbModuleLoader(new moduleLoader::ModuleLoader(signatures, m_logger))
    , m_dataSourceManager(new DataSourceManager)
    , m_mapUtil(new TdbVectorMapUtil())
{
    // Load module configuration
    if (!m_configuration.load(config)) {
        m_logger.error() << "Failed to process given module configuration: "
                         << m_configuration.lastErrorMessage();
        throw ConfigurationException("Failed to parse configuration!");
    }

    // Set database module facilities
    #define SET_FACILITY(n,w) \
        try { \
            m_dbModuleLoader->setModuleFacility((n), (w)); \
        } catch (...) { \
            std::throw_with_nested( \
                InitializationException( \
                    "Failed setting module facility \"" n "\"!")); \
        }
    SET_FACILITY("Logger", &const_cast<LogHard::Logger &>(m_logger));
    SET_FACILITY("DataStoreManager", &m_dataStoreManager);
    SET_FACILITY("DataSourceManager", m_dataSourceManager->getWrapper());
    SET_FACILITY("TdbVectorMapUtil", m_mapUtil->getWrapper());
    SET_FACILITY("ConsensusService", &consensusService);
    #undef SET_FACILITY

    // Load database modules
    for (const TdbConfiguration::DbModuleEntry & cfgDbMod
         : m_configuration.dbModuleList())
    {
        SharemindModule * const m = m_dbModuleLoader->addModule(
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
    for (const TdbConfiguration::DataSourceEntry & cfgDs
         : m_configuration.dataSourceList())
    {
        if (!m_dbModuleLoader->hasModule(cfgDs.dbModule)) {
            m_logger.error() << "Data source \"" << cfgDs.name
                             << "\" uses an unknown database module: \""
                             << cfgDs.dbModule << "\".";
            throw ConfigurationException("Configuration contained unknown "
                                         "module references!");
        }

        if (!m_dataSourceManager->addDataSource(cfgDs.name, cfgDs.dbModule, cfgDs.configurationFile)) {
            m_logger.error() << "Data source \"" << cfgDs.name
                             << "\" has duplicate configuration entries.";
            throw ConfigurationException("Configuration contained duplicate "
                                         "configuration entries!");
        }
    }

}

TdbModule::~TdbModule() { }

bool TdbModule::getErrorCode(const SharemindModuleApi0x1SyscallContext * ctx,
        const std::string & dsName,
        SharemindTdbError & code) const
{
    // Get error store
    SharemindDataStore * const errors = m_dataStoreManager.get_datastore(
                                                 &m_dataStoreManager,
                                                 ctx,
                                                 "mod_tabledb/errors");
    if (!errors) {
        m_logger.error() << "Failed to get process data store.";
        return false;
    }

    const SharemindTdbError * const err = static_cast<SharemindTdbError *>(errors->get(errors, dsName.c_str()));
    code = err ? *err : SHAREMIND_TDB_OK;

    return true;
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
    DataSource * src = m_dataSourceManager->getDataSource(dsName);
    if (!src) {
        m_logger.error() << "Data source \"" << dsName << "\" is not defined.";
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    }

    // Get the system call object
    const SharemindSyscallBinding * sb = m_dbModuleLoader->getSyscall(src->module(), signature);
    if (!sb) {
        m_logger.error()
            << "Data source \"" << dsName << "\" database module \""
            << src->module() << "\" has no system call with signature \""
            << signature << "\".";
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    }

    // Do the system call
    SharemindSyscallContext sc = *c;
    sc.moduleHandle = sb->moduleHandle;

    return (*(sb->wrapper.callable))(args, num_args, refs, crefs, returnValue, &sc);
}

bool TdbModule::newVectorMap(const SharemindModuleApi0x1SyscallContext * ctx,
                             uint64_t & stmtId)
{
    SharemindDataStore * const maps = m_dataStoreManager.get_datastore(
                                          &m_dataStoreManager,
                                          ctx,
                                          "mod_tabledb/vector_maps");
    if (!maps) {
        m_logger.error() << "Failed to get process data store.";
        return false;
    }

    TdbVectorMap * const map = m_mapUtil->newVectorMap(maps);
    if (!map)
        return false;

    stmtId = map->getId();

    return true;
}

bool TdbModule::deleteVectorMap(const SharemindModuleApi0x1SyscallContext * ctx,
                                const uint64_t stmtId)
{
    SharemindDataStore * const maps = m_dataStoreManager.get_datastore(
                                          &m_dataStoreManager,
                                          ctx,
                                          "mod_tabledb/vector_maps");
    if (!maps) {
        m_logger.error() << "Failed to get process data store.";
        return false;
    }

    return m_mapUtil->deleteVectorMap(maps, stmtId);
}

TdbVectorMap * TdbModule::getVectorMap(const SharemindModuleApi0x1SyscallContext * ctx,
                                       const uint64_t stmtId) const
{
    SharemindDataStore * const maps = m_dataStoreManager.get_datastore(
                                          &m_dataStoreManager,
                                          ctx,
                                          "mod_tabledb/vector_maps");
    if (!maps) {
        m_logger.error() << "Failed to get process data store.";
        return nullptr;
    }

    return m_mapUtil->getVectorMap(maps, stmtId);
}

} /* namespace sharemind { */
