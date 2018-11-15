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

#ifndef SHAREMIND_MOD_TABLEDB_LOADER_H
#define SHAREMIND_MOD_TABLEDB_LOADER_H

#include <algorithm>
#include <cassert>
#include <cstring>
#include <LogHard/Logger.h>
#include <memory>
#include <sharemind/DebugOnly.h>
#include <sharemind/libmodapi/libmodapi.h>
#include <sharemind/likely.h>
#include <sharemind/SimpleUnorderedStringMap.h>
#include <string>
#include <vector>


namespace sharemind {

class __attribute__ ((visibility("internal"))) ModuleLoader {

private: /* Types: */

    using SyscallMap =
            SimpleUnorderedStringMap<std::unique_ptr<SharemindSyscallWrapper> >;

public: /* Methods: */

    ModuleLoader(std::vector<std::string> requiredSyscallSignatures,
                 LogHard::Logger const & logger)
        : m_reqSignatures(std::move(requiredSyscallSignatures))
        , m_logger(logger, "ModuleLoader:")
    {
        /// \todo Throw a better exception
        m_modApi = SharemindModuleApi_new(nullptr, nullptr, nullptr);
        if (unlikely(!m_modApi))
            throw std::bad_alloc();
    }

    ~ModuleLoader() {
        std::for_each(m_modules.begin(),
                      m_modules.end(),
                      &SharemindModule_free);
        m_modules.clear();
        SharemindModuleApi_free(m_modApi);
    }

    SharemindModule * addModule(std::string const & filename,
                                std::string const & config = std::string())
    {
        struct GracefulException {};
        SharemindModule * const m =
                SharemindModuleApi_newModule(m_modApi,
                                             filename.c_str(),
                                             config.c_str());
        if (unlikely(!m)) {
            m_logger.error()
                    << "Error while loading module \"" << filename << "\": "
                    << SharemindModuleApi_lastErrorString(m_modApi);
            return nullptr;
        }

        try {
            SharemindModuleApiError e = SharemindModule_init(m);
            if (unlikely(e != SHAREMIND_MODULE_API_OK)) {
                m_logger.error()
                        << "Error initializing module: Module returned "
                        << SharemindModuleApiError_toString(e);
                throw GracefulException();
            }

            char const * const moduleName = SharemindModule_name(m);
            assert(moduleName);
            if (unlikely(m_moduleSyscallMap.find(moduleName)
                         != m_moduleSyscallMap.end()))
            {
                m_logger.error() << "Module name \"" << moduleName
                                 << "\" is already provided by another module.";
                throw GracefulException();
            }

            /* Load system calls */
            SyscallMap syscallMap;
            for (auto const & required : m_reqSignatures) {
                auto * const sc =
                        SharemindModule_findSyscall(m, required.c_str());
                if (!sc) {
                    m_logger.fatal() << "Required system call \"" << required
                                     << "\" not defined in module!";
                    throw GracefulException();
                }
                SHAREMIND_DEBUG_ONLY(auto const rv =)
                        syscallMap.emplace(
                            required,
                            std::make_unique<SharemindSyscallWrapper>(
                                SharemindSyscall_wrapper(sc)));
                assert(rv.second);
            }
            auto rv = m_moduleSyscallMap.emplace(moduleName,
                                                 std::move(syscallMap));
            assert(rv.second);
            try {
                m_modules.push_back(m);
            } catch (...) {
                m_moduleSyscallMap.erase(std::move(rv.first));
                throw;
            }
            return m;
        } catch (std::exception const & e) {
            m_logger.error() << "Error loading database module " << filename
                             << ": " << e.what();
            SharemindModule_free(m);
            return nullptr;
        } catch (GracefulException const &) {
            SharemindModule_free(m);
            return nullptr;
        }
    }

    bool hasModule(std::string const & module) const
    { return m_moduleSyscallMap.find(module) != m_moduleSyscallMap.end(); }

    SharemindSyscallWrapper getSyscall(std::string const & module,
                                       std::string const & signature) const
    {
        auto const msit(m_moduleSyscallMap.find(module));
        if (msit == m_moduleSyscallMap.end())
            return { nullptr, nullptr };

        auto const & syscallMap = msit->second;
        auto const sit = syscallMap.find(signature);
        if (sit == syscallMap.end())
            return { nullptr, nullptr };

        return *(sit->second);
    }

    void setModuleFacility(char const * name,
                           void * facility,
                           void * context = nullptr)
    {
        assert(name);
        auto const r = SharemindModuleApi_setModuleFacility(m_modApi,
                                                            name,
                                                            facility,
                                                            context);
        if (r != SHAREMIND_MODULE_API_OK)
            throw std::bad_alloc(); /// \todo Throw a better exception
    }

    SharemindFacility const * moduleFacility(char const * name) {
        assert(name);
        return SharemindModuleApi_moduleFacility(m_modApi, name);
    }

private: /* Fields: */

    std::vector<SharemindModule *> m_modules;
    SharemindModuleApi * m_modApi;
    SimpleUnorderedStringMap<SyscallMap> m_moduleSyscallMap;

    std::vector<std::string> m_reqSignatures;

    LogHard::Logger const m_logger;

}; /* class ModuleLoader { */

} /* namespace sharemind { */

#endif /* SHAREMIND_MOD_TABLEDB_LOADER_H */
