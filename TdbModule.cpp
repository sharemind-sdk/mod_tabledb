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
#include <boost/foreach.hpp>
#include <sharemind/common/Logger/Debug.h>
#include <sharemind/dbcommon/ModuleLoader/ModuleLoader.h>

#define SHAREMIND_INTERNAL__
#include <sharemind/dbcommon/DataSource.h>
#include <sharemind/dbcommon/DataSourceManager.h>

#include "TdbVectorMap.h"
#include "TdbVectorMapUtil.h"


namespace { SHAREMIND_DEFINE_PREFIXED_LOGS("[TdbModule] "); }

namespace {

template <class T> void destroy(void * ptr) throw() { delete static_cast<T *>(ptr); }

} /* namespace { */

namespace sharemind {

TdbModule::TdbModule(ILogger & logger, IRandom & rng, SharemindDataStoreManager & dataStoreManager, const std::string & config, const std::set<std::string> & signatures)
    : m_logger(logger)
    , m_dataStoreManager(dataStoreManager)
    , m_dbModuleLoader(new moduleLoader::ModuleLoader(signatures))
    , m_dataSourceManager(new DataSourceManager)
    , m_mapUtil(new TdbVectorMapUtil(rng))
{
    // Load module configuration
    if (!m_configuration.load(config))
        throw ConfigurationException("Failed to process given module configuration: " +
                                     m_configuration.getLastErrorMessage());

    // Set database module facilities
    if (!m_dbModuleLoader->setModuleFacility("Logger", &m_logger))
        throw InitializationException("Failed setting module facility 'Logger'.");

    if (!m_dbModuleLoader->setModuleFacility("DataStoreManager", &m_dataStoreManager))
        throw InitializationException("Failed setting module facility 'DataStoreManager'.");

    if (!m_dbModuleLoader->setModuleFacility("DataSourceManager", m_dataSourceManager->getWrapper()))
        throw InitializationException("Failed setting module facility 'DataSourceManager'.");

    if (!m_dbModuleLoader->setModuleFacility("TdbVectorMapUtil", m_mapUtil->getWrapper()))
        throw InitializationException("Failed setting module facility 'TdbVectorMapUtil'.");

    // Load database modules
    BOOST_FOREACH (const TdbConfiguration::DbModuleEntry & cfgDbMod, m_configuration.getDbModuleList()) {
        SharemindModule * m = m_dbModuleLoader->addModule(cfgDbMod.filename, cfgDbMod.configurationFile);
        if (!m) {
            std::ostringstream oss;
            oss << "Failed loading database module '" << cfgDbMod.filename << "': "
                << m_dbModuleLoader->lastError();
            throw InitializationException(oss.str());
        }
        LogNormal(m_logger)
            << "Loaded database module \"" << SharemindModule_get_name(m)
            << "\" (" << SharemindModule_get_num_syscalls(m)
            << " syscalls) from \"" << cfgDbMod.filename << "\" using API version "
            << SharemindModule_get_api_version_in_use(m) << '.';
    }

    // Load data sources
    BOOST_FOREACH (const TdbConfiguration::DataSourceEntry &cfgDs, m_configuration.getDataSourceList()) {
        if (!m_dbModuleLoader->hasModule(cfgDs.dbModule)) {
            std::ostringstream oss;
            oss << "Data source \"" << cfgDs.name
                << "\" uses an unknown database module: \""
                << cfgDs.dbModule << "\".";
            throw ConfigurationException(oss.str());
        }

        if (!m_dataSourceManager->addDataSource(cfgDs.name, cfgDs.dbModule, cfgDs.configurationFile)) {
            std::ostringstream oss;
            oss << "Data source \"" << cfgDs.name
                << "\" has duplicate configuration entries.";
            throw InitializationException(oss.str());
        }
    }

}

TdbModule::~TdbModule() { }

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
        LogError(m_logger) << "Data source \"" << dsName << "\" is not defined.";
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    }

    // Get the system call object
    const SharemindSyscallBinding * sb = m_dbModuleLoader->getSyscall(src->module(), signature);
    if (!sb) {
        LogError(m_logger)
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

bool TdbModule::newVectorMap(const void * process, uint64_t & stmtId) {
    SharemindDataStore * maps = m_dataStoreManager.get_datastore(&m_dataStoreManager,
                                                        process,
                                                        "mod_tabledb/vector_maps");
    if (!maps) {
        LogError(m_logger) << "Failed to get process data store.";
        return false;
    }

    TdbVectorMap * map = m_mapUtil->newVectorMap(maps);
    if (!map)
        return false;

    stmtId = map->getId();

    return true;
}

bool TdbModule::deleteVectorMap(const void * process, const uint64_t stmtId) {
    SharemindDataStore * maps = m_dataStoreManager.get_datastore(&m_dataStoreManager,
                                                        process,
                                                        "mod_tabledb/vector_maps");
    if (!maps) {
        LogError(m_logger) << "Failed to get process data store.";
        return false;
    }

    return m_mapUtil->deleteVectorMap(maps, stmtId);
}

TdbVectorMap * TdbModule::getVectorMap(const void * process, const uint64_t stmtId) const {
    SharemindDataStore * maps = m_dataStoreManager.get_datastore(&m_dataStoreManager,
                                                        process,
                                                        "mod_tabledb/vector_maps");
    if (!maps) {
        LogError(m_logger) << "Failed to get process data store.";
        return NULL;
    }

    return m_mapUtil->getVectorMap(maps, stmtId);
}

} /* namespace sharemind { */
