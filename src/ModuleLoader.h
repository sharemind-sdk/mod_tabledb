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
#include <map>
#include <memory>
#include <set>
#include <sharemind/DebugOnly.h>
#include <sharemind/libmodapi/libmodapi.h>
#include <sharemind/likely.h>
#include <sharemind/MakeUnique.h>
#include <string>
#include <vector>


namespace sharemind {

class __attribute__ ((visibility("internal"))) ModuleLoader {

private: /* Types: */

    using SyscallMap =
            std::map<std::string, std::unique_ptr<SharemindSyscallWrapper> >;

public: /* Methods: */

    ModuleLoader(const std::set<std::string> & signatures,
                 const LogHard::Logger & logger)
        : m_reqSignatures(signatures)
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

    SharemindModule * addModule(const std::string & filename,
                                const std::string & config = std::string())
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

            const char * const moduleName = SharemindModule_name(m);
            assert(moduleName);
            if (unlikely(m_moduleSyscallMap.count(moduleName))) {
                m_logger.error() << "Module name \"" << moduleName
                                 << "\" is already provided by another module.";
                throw GracefulException();
            }


            /* Load system calls */
            assert(!m_moduleSyscallMap.count(moduleName));
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
                            makeUnique<SharemindSyscallWrapper>(
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
        } catch (const std::exception & e) {
            m_logger.error() << "Error loading database module " << filename
                             << ": " << e.what();
            SharemindModule_free(m);
            return nullptr;
        } catch (const GracefulException &) {
            SharemindModule_free(m);
            return nullptr;
        }
    }

    inline bool hasModule(const std::string & module) const {
        return m_moduleSyscallMap.find(module) != m_moduleSyscallMap.end();
    }

    SharemindSyscallWrapper getSyscall(const std::string & module,
                                       const std::string & signature)
            const
    {
        auto const msit(m_moduleSyscallMap.find(module));
        if (msit == m_moduleSyscallMap.end())
            return { nullptr, nullptr };

        const SyscallMap & syscallMap = msit->second;
        SyscallMap::const_iterator sit = syscallMap.find(signature);
        if (sit == syscallMap.end())
            return { nullptr, nullptr };

        return *(sit->second);
    }

    inline void setModuleFacility(const char * name,
                                  void * facility,
                                  void * context = nullptr)
    {
        assert(name);
        const auto r = SharemindModuleApi_setModuleFacility(m_modApi,
                                                            name,
                                                            facility,
                                                            context);
        if (r != SHAREMIND_MODULE_API_OK)
            throw std::bad_alloc(); /// \todo Throw a better exception
    }

    inline const SharemindFacility * moduleFacility(const char * name) {
        assert(name);
        return SharemindModuleApi_moduleFacility(m_modApi, name);
    }

private: /* Fields: */

    std::vector<SharemindModule *> m_modules;
    SharemindModuleApi * m_modApi;
    std::map<std::string, SyscallMap> m_moduleSyscallMap;

    std::set<std::string> m_reqSignatures;

    const LogHard::Logger m_logger;

}; /* class ModuleLoader { */

} /* namespace sharemind { */

#endif /* SHAREMIND_MOD_TABLEDB_LOADER_H */
