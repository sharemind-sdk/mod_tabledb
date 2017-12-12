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

#include <cassert>
#include <LogHard/Logger.h>
#include <sharemind/compiler-support/GccIsNothrowDestructible.h>
#include <sharemind/datastoreapi.h>
#include <sharemind/module-apis/api_0x1.h>
#include <vector>
#include "TdbModule.h"
#include "TdbTypesUtil.h"

#include "TdbVectorMap.h"


namespace {

using namespace sharemind;

template < size_t NumArgs
         , bool   NeedReturnValue = false
         , size_t NumRefs = 0
         , size_t NumCRefs = 0
         >
struct SyscallArgs {
    static inline bool check(SharemindCodeBlock * args,
                             size_t num_args,
                             const SharemindModuleApi0x1Reference* refs,
                             const SharemindModuleApi0x1CReference* crefs,
                             SharemindCodeBlock * returnValue)
    {
        (void) args;

        if (num_args != NumArgs) {
            return false;
        }

        if (NeedReturnValue && ! returnValue) {
            return false;
        }

        if (refs) {
            size_t i = 0;
            for (; refs[i].pData; ++ i);
            if (i != NumRefs)
                return false;
        }
        else {
            if (NumRefs != 0)
                return false;
        }

        if (crefs) {
            size_t i = 0;
            for (; crefs[i].pData; ++ i);
            if (i != NumCRefs)
                return false;
        }
        else {
            if (NumCRefs != 0)
                return false;
        }

        return true;
    }
};

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_error_code,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<0u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue)) {
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;
    }
    /* The other arguments will be checked by the submodules */
    if (!crefs || !crefs[0].pData)
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    try {
        const std::string dsName(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

        SharemindTdbError err = SHAREMIND_TDB_OK;
        if (!m->getErrorCode(c, dsName, err))
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        returnValue->int64[0] = err;

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

#define MOD_TABLEDB_FORWARD_SYSCALL(syscallName) \
    SHAREMIND_MODULE_API_0x1_SYSCALL(syscallName, \
                                     args, num_args, refs, crefs, \
                                     returnValue, c) \
    { \
        /* The other arguments will be checked by the submodules */ \
        if (!crefs || !crefs[0].pData) \
            return SHAREMIND_MODULE_API_0x1_INVALID_CALL; \
        if ((crefs[0u].size == 0u) \
            || (static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u]\
                != '\0')) \
            return SHAREMIND_MODULE_API_0x1_INVALID_CALL; \
        try { \
            std::string const dsName( \
                    static_cast<const char *>(crefs[0u].pData), \
                    crefs[0u].size - 1u); \
            sharemind::TdbModule & m = \
                    *static_cast<sharemind::TdbModule *>(c->moduleHandle); \
            return m.doSyscall(dsName, #syscallName, args, num_args, refs, \
                               crefs, returnValue, c); \
        } catch (const std::bad_alloc &) { \
            return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY; \
        } catch (...) { \
            return SHAREMIND_MODULE_API_0x1_MODULE_ERROR; \
        } \
    }

MOD_TABLEDB_FORWARD_SYSCALL(tdb_open)
MOD_TABLEDB_FORWARD_SYSCALL(tdb_close)
MOD_TABLEDB_FORWARD_SYSCALL(tdb_tbl_create)
MOD_TABLEDB_FORWARD_SYSCALL(tdb_tbl_create2)
MOD_TABLEDB_FORWARD_SYSCALL(tdb_tbl_delete)
MOD_TABLEDB_FORWARD_SYSCALL(tdb_tbl_exists)
MOD_TABLEDB_FORWARD_SYSCALL(tdb_tbl_col_count)
MOD_TABLEDB_FORWARD_SYSCALL(tdb_tbl_col_names)
MOD_TABLEDB_FORWARD_SYSCALL(tdb_tbl_col_types)
MOD_TABLEDB_FORWARD_SYSCALL(tdb_tbl_row_count)
MOD_TABLEDB_FORWARD_SYSCALL(tdb_insert_row)
MOD_TABLEDB_FORWARD_SYSCALL(tdb_insert_row2)
MOD_TABLEDB_FORWARD_SYSCALL(tdb_read_col)
MOD_TABLEDB_FORWARD_SYSCALL(tdb_stmt_exec)
MOD_TABLEDB_FORWARD_SYSCALL(tdb_table_names)

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_new,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<0u, true, 0u, 0u>::check(args, num_args, refs, crefs, returnValue)) {
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;
    }

    try {
        sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

        uint64_t vmapId = 0;
        if (!m->newVectorMap(c, vmapId))
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        returnValue->uint64[0] = vmapId;

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_delete,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, false, 0u, 0u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    try {
        const uint64_t vmapId = args[0].uint64[0];

        sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

        if (!m->deleteVectorMap(c, vmapId))
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_size_index,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        returnValue->uint64[0] = map->size<SharemindTdbIndex>(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_at_index,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<2u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const uint64_t num = args[1].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        const SharemindTdbIndex & idx = map->at<SharemindTdbIndex>(name, num);

        returnValue->uint64[0] = idx.idx;

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_push_back_index,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<2u, false, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const uint64_t val = args[1].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        SharemindTdbIndex * idx = SharemindTdbIndex_new(val);
        try {
            map->push_back<SharemindTdbIndex>(name, idx);
        } catch (...) {
            SharemindTdbIndex_delete(idx);
            throw;
        }

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_pop_back_index,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, false, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        map->pop_back<SharemindTdbIndex>(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_clear_index,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, false, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        map->clear<SharemindTdbIndex>(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_is_index_vector,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        returnValue->uint64[0] = map->count<SharemindTdbIndex>(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_size_string,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        returnValue->uint64[0] = map->size<SharemindTdbString>(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_at_string,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<2u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0u].uint64[0u];
        const uint64_t num = args[1u].uint64[0u];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        const SharemindTdbString & str = map->at<SharemindTdbString>(name, num);

        const uint64_t mem_size = strlen(str.str) + 1u;
        const uint64_t mem_hndl = (* c->publicAlloc)(c, mem_size);
        char * const mem_ptr = static_cast<char *>((* c->publicMemPtrData)(c, mem_hndl));
        strncpy(mem_ptr, str.str, mem_size);
        returnValue[0].uint64[0] = mem_hndl;

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_push_back_string,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, false, 0u, 2u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || crefs[1u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0'
            || static_cast<const char *>(crefs[1u].pData)[crefs[1u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);
        const std::string str(static_cast<const char *>(crefs[1u].pData), crefs[1u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        auto * const s = SharemindTdbString_new2(str.c_str(), str.size());
        try {
            map->push_back<SharemindTdbString>(name, s);
        } catch (...) {
            SharemindTdbString_delete(s);
            throw;
        }

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_pop_back_string,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, false, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        map->pop_back<SharemindTdbString>(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_clear_string,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, false, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        map->clear<SharemindTdbString>(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_is_string_vector,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        returnValue->uint64[0] = map->count<SharemindTdbString>(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_size_type,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        returnValue->uint64[0] = map->size<SharemindTdbType>(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_at_type_domain,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<2u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const uint64_t num = args[1].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        const SharemindTdbType & t = map->at<SharemindTdbType>(name, num);

        const char * str = t.domain;
        const uint64_t mem_size = strlen(str) + 1u;
        const uint64_t mem_hndl = (* c->publicAlloc)(c, mem_size);
        char * const mem_ptr = static_cast<char *>((* c->publicMemPtrData)(c, mem_hndl));
        strncpy(mem_ptr, str, mem_size);
        returnValue[0].uint64[0] = mem_hndl;

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_at_type_name,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<2u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const uint64_t num = args[1].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        const SharemindTdbType & t = map->at<SharemindTdbType>(name, num);

        const char * str = t.name;
        const uint64_t mem_size = strlen(str) + 1u;
        const uint64_t mem_hndl = (* c->publicAlloc)(c, mem_size);
        char * const mem_ptr = static_cast<char *>((* c->publicMemPtrData)(c, mem_hndl));
        strncpy(mem_ptr, str, mem_size);
        returnValue[0].uint64[0] = mem_hndl;

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_at_type_size,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<2u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const uint64_t num = args[1].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        const SharemindTdbType & t = map->at<SharemindTdbType>(name, num);
        returnValue[0].uint64[0] = t.size;

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_push_back_type,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<2u, false, 0u, 3u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || crefs[1u].size == 0u
            || crefs[2u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0'
            || static_cast<const char *>(crefs[1u].pData)[crefs[1u].size - 1u] != '\0'
            || static_cast<const char *>(crefs[2u].pData)[crefs[2u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const uint64_t typeSize = args[1].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        auto * const t =
                SharemindTdbType_new2(
                    static_cast<const char *>(crefs[1u].pData),
                    crefs[1u].size - 1u,
                    static_cast<const char *>(crefs[2u].pData),
                    crefs[2u].size - 1u,
                    typeSize);
        try {
            map->push_back<SharemindTdbType>(name, t);
        } catch (...) {
            SharemindTdbType_delete(t);
            throw;
        }

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_pop_back_type,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, false, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        map->pop_back<SharemindTdbType>(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_clear_type,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, false, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        map->clear<SharemindTdbType>(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_is_type_vector,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        returnValue->uint64[0] = map->count<SharemindTdbType>(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_size_value,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        returnValue->uint64[0] = map->size<SharemindTdbValue>(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_at_value_type_domain,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<2u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const uint64_t num = args[1].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        const SharemindTdbValue & v = map->at<SharemindTdbValue>(name, num);

        const char * str = v.type->domain;
        const uint64_t mem_size = strlen(str) + 1u;
        const uint64_t mem_hndl = (* c->publicAlloc)(c, mem_size);
        char * const mem_ptr = static_cast<char *>((* c->publicMemPtrData)(c, mem_hndl));
        strncpy(mem_ptr, str, mem_size);
        returnValue[0].uint64[0] = mem_hndl;

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_at_value_type_name,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<2u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const uint64_t num = args[1].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        const SharemindTdbValue & v = map->at<SharemindTdbValue>(name, num);

        const char * str = v.type->name;
        const uint64_t mem_size = strlen(str) + 1u;
        const uint64_t mem_hndl = (* c->publicAlloc)(c, mem_size);
        char * const mem_ptr = static_cast<char *>((* c->publicMemPtrData)(c, mem_hndl));
        strncpy(mem_ptr, str, mem_size);
        returnValue[0].uint64[0] = mem_hndl;

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_at_value_type_size,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<2u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const uint64_t num = args[1].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        const SharemindTdbValue & v = map->at<SharemindTdbValue>(name, num);
        returnValue[0].uint64[0] = v.type->size;

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_at_value,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (num_args != 2)
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (refs && refs[1u].pData != nullptr)
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (refs && refs[0u].size == 0u)
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (!crefs || crefs[1u].pData != nullptr)
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const uint64_t num = args[1].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        const SharemindTdbValue & v = map->at<SharemindTdbValue>(name, num);

        if (refs) {
            // TODO: the following is a workaround! We are always allocating one
            // byte too much as VM does not allow us to allocate 0 sized memory block.

            // If the buffer size equal the type size, we assume it is a scalar
            // value and the workaround does not apply to it.
            if (refs[0u].size != v.type->size && refs[0u].size - 1 != v.size)
                return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

            if (v.buffer) memcpy(refs[0u].pData, v.buffer, v.size);
        }

        if (returnValue)
            returnValue[0].uint64[0] = v.size;

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_push_back_value,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<2u, false, 0u, 4u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || crefs[1u].size == 0u
            || crefs[2u].size == 0u
            || crefs[3u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0'
            || static_cast<const char *>(crefs[1u].pData)[crefs[1u].size - 1u] != '\0'
            || static_cast<const char *>(crefs[2u].pData)[crefs[2u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[3u].size == 0u)
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const uint64_t typeSize = args[1].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        uint64_t bufSize = 0;
        // If the buffer size equal the type size, we assume it is a scalar
        // value and the workaround does not apply to it.
        if (crefs[3u].size == typeSize) {
            bufSize = crefs[3u].size;
        } else {
            // TODO: the following is a workaround! We are always allocating one
            // byte too much as VM does not allow us to allocate 0 sized memory block.
            bufSize = crefs[3u].size - 1;
        }

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        auto * const v =
                SharemindTdbValue_new2(
                    static_cast<const char *>(crefs[1u].pData),
                    crefs[1u].size - 1u,
                    static_cast<const char *>(crefs[2u].pData),
                    crefs[2u].size - 1u,
                    typeSize,
                    bufSize ? crefs[3u].pData : nullptr,
                    bufSize);

        try {
            map->push_back<SharemindTdbValue>(name, v);
        } catch (...) {
            SharemindTdbValue_delete(v);
            throw;
        }

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_pop_back_value,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, false, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        map->pop_back<SharemindTdbValue>(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_clear_value,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, false, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[1u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        map->clear<SharemindTdbValue>(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_is_value_vector,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        returnValue->uint64[0] = map->count<SharemindTdbValue>(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_count,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, true, 0u, 1u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        returnValue->uint64[0] = map->count(name);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_erase,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (num_args != 1)
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (refs)
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (!crefs || crefs[1].pData != nullptr)
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    if (crefs[0u].size == 0u
            || static_cast<const char *>(crefs[0u].pData)[crefs[0u].size - 1u] != '\0')
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const std::string name(static_cast<const char *>(crefs[0u].pData), crefs[0u].size - 1u);

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        uint64_t rv = map->erase(name);

        if (returnValue)
            returnValue->uint64[0] = rv;

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_clear,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, false, 0u, 0u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        map->clear();

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_reset,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, false, 0u, 0u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        map->reset();

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_set_batch,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<2u, false, 0u, 0u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];
        const uint64_t num = args[1].uint64[0];

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        map->setBatch(num);

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_add_batch,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, false, 0u, 0u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        map->addBatch();

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_SYSCALL(tdb_vmap_batch_count,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, true, 0u, 0u>::check(args, num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    sharemind::TdbModule * m = static_cast<sharemind::TdbModule *>(c->moduleHandle);

    try {
        const uint64_t vmapId = args[0].uint64[0];

        sharemind::TdbVectorMap * map = m->getVectorMap(c, vmapId);
        if (!map)
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

        returnValue->uint64[0] = map->batchCount();

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const sharemind::TdbVectorMap::Exception & e) {
        m->logger().error() << e.what();
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }
}

} /* namespace { */

extern "C" {

SHAREMIND_MODULE_API_MODULE_INFO("tabledb",
                                 0x00010000,
                                 0x1);

SHAREMIND_MODULE_API_0x1_INITIALIZER(c) __attribute__ ((visibility("default")));
SHAREMIND_MODULE_API_0x1_INITIALIZER(c) {
    assert(c);

    /*
     * Get facilities
     */
    const SharemindModuleApi0x1Facility * flog = c->getModuleFacility(c, "Logger");
    if (!flog || !flog->facility)
        return SHAREMIND_MODULE_API_0x1_MISSING_FACILITY;

    const SharemindModuleApi0x1Facility * fconsensus = c->getModuleFacility(c, "ConsensusService");
    SharemindConsensusFacility * consensusService;
    if (!fconsensus || !fconsensus->facility) {
        consensusService = nullptr;
    } else {
        consensusService = static_cast<SharemindConsensusFacility *>(fconsensus->facility);
    }

    const LogHard::Logger & logger =
            *static_cast<const LogHard::Logger *>(flog->facility);

    /*
     * Check for the module configuration
     */
    if (!c->conf) {
        logger.error() << "No module configuration given.";
        return SHAREMIND_MODULE_API_0x1_INVALID_MODULE_CONFIGURATION;
    }

    /*
     * Initialize the module handle
     */
    try {
        try {
            c->moduleHandle =
                    new sharemind::TdbModule(
                        logger,
                        consensusService,
                        c->conf,
                        // List of required submodule syscall signatures:
                        std::vector<std::string>{
                            "tdb_open",
                            "tdb_close",
                            "tdb_table_names",
                            "tdb_tbl_create",
                            "tdb_tbl_create2",
                            "tdb_tbl_delete",
                            "tdb_tbl_exists",
                            "tdb_tbl_col_count",
                            "tdb_tbl_col_names",
                            "tdb_tbl_col_types",
                            "tdb_tbl_row_count",
                            // "tdb_delete_col",
                            // "tdb_delete_row",
                            // "tdb_insert_col",
                            "tdb_insert_row",
                            "tdb_insert_row2",
                            "tdb_read_col",
                            // "tdb_read_row",
                            // "tdb_update_col",
                            // "tdb_update_row",
                            "tdb_stmt_exec"
                        });
        } catch (...) {
            logger.printCurrentException();
            throw;
        }
        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (TdbModule::ConfigurationException const &) {
        return SHAREMIND_MODULE_API_0x1_INVALID_MODULE_CONFIGURATION;
    } catch (TdbModule::InitializationException const &) {
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    }
}

SHAREMIND_MODULE_API_0x1_DEINITIALIZER(c) __attribute__ ((visibility("default")));
SHAREMIND_MODULE_API_0x1_DEINITIALIZER(c) {
    assert(c);
    assert(c->moduleHandle);
    static_assert(is_nothrow_destructible<sharemind::TdbModule>::value, "");
    delete static_cast<sharemind::TdbModule *>(c->moduleHandle);
    c->moduleHandle = nullptr;
}

SHAREMIND_MODULE_API_0x1_SYSCALL_DEFINITIONS(

    /* Error handling */
      { "tdb_error_code",                   &tdb_error_code }

    /* High level database operations */
    , { "tdb_open",                         &tdb_open }
    , { "tdb_close",                        &tdb_close }
    , { "tdb_table_names",                  &tdb_table_names }

    /* Table database API */
    , { "tdb_tbl_create",                   &tdb_tbl_create }
    , { "tdb_tbl_create2",                  &tdb_tbl_create2 }
    , { "tdb_tbl_delete",                   &tdb_tbl_delete }
    , { "tdb_tbl_exists",                   &tdb_tbl_exists }
    , { "tdb_tbl_col_count",                &tdb_tbl_col_count }
    , { "tdb_tbl_col_names",                &tdb_tbl_col_names }
    , { "tdb_tbl_col_types",                &tdb_tbl_col_types }
    , { "tdb_tbl_row_count",                &tdb_tbl_row_count }
    //, { "tdb_delete_col", &tdb_delete_col }
    //, { "tdb_delete_row", &tdb_delete_row }
    //, { "tdb_insert_col", &tdb_insert_col }
    , { "tdb_insert_row",                   &tdb_insert_row }
    , { "tdb_insert_row2",                  &tdb_insert_row2 }
    , { "tdb_read_col",                     &tdb_read_col }
    //, { "tdb_read_row",   &tdb_read_row }
    //, { "tdb_update_col", &tdb_update_col }
    //, { "tdb_update_row", &tdb_update_row }

    /* Table database statement API */
    , { "tdb_stmt_exec",                    &tdb_stmt_exec }

    /* Parameter and result vector map API */
    /* Constructor/Destructor */
    , { "tdb_vmap_new",                     &tdb_vmap_new }
    , { "tdb_vmap_delete",                  &tdb_vmap_delete }

    /* Value manipulation */
    , { "tdb_vmap_size_index",              &tdb_vmap_size_index }
    , { "tdb_vmap_at_index",                &tdb_vmap_at_index }
    , { "tdb_vmap_push_back_index",         &tdb_vmap_push_back_index }
    , { "tdb_vmap_pop_back_index",          &tdb_vmap_pop_back_index }
    , { "tdb_vmap_clear_index",             &tdb_vmap_clear_index }
    , { "tdb_vmap_is_index_vector",         &tdb_vmap_is_index_vector }
    , { "tdb_vmap_size_string",             &tdb_vmap_size_string }
    , { "tdb_vmap_at_string",               &tdb_vmap_at_string }
    , { "tdb_vmap_push_back_string",        &tdb_vmap_push_back_string }
    , { "tdb_vmap_pop_back_string",         &tdb_vmap_pop_back_string }
    , { "tdb_vmap_clear_string",            &tdb_vmap_clear_string }
    , { "tdb_vmap_is_string_vector",        &tdb_vmap_is_string_vector }
    , { "tdb_vmap_size_type",               &tdb_vmap_size_type }
    , { "tdb_vmap_at_type_domain",          &tdb_vmap_at_type_domain }
    , { "tdb_vmap_at_type_name",            &tdb_vmap_at_type_name }
    , { "tdb_vmap_at_type_size",            &tdb_vmap_at_type_size }
    , { "tdb_vmap_push_back_type",          &tdb_vmap_push_back_type }
    , { "tdb_vmap_pop_back_type",           &tdb_vmap_pop_back_type }
    , { "tdb_vmap_clear_type",              &tdb_vmap_clear_type }
    , { "tdb_vmap_is_type_vector",          &tdb_vmap_is_type_vector }
    , { "tdb_vmap_size_value",              &tdb_vmap_size_value }
    , { "tdb_vmap_at_value_type_domain",    &tdb_vmap_at_value_type_domain }
    , { "tdb_vmap_at_value_type_name",      &tdb_vmap_at_value_type_name }
    , { "tdb_vmap_at_value_type_size",      &tdb_vmap_at_value_type_size }
    , { "tdb_vmap_at_value",                &tdb_vmap_at_value }
    , { "tdb_vmap_push_back_value",         &tdb_vmap_push_back_value }
    , { "tdb_vmap_pop_back_value",          &tdb_vmap_pop_back_value }
    , { "tdb_vmap_clear_value",             &tdb_vmap_clear_value }
    , { "tdb_vmap_is_value_vector",         &tdb_vmap_is_value_vector }
    , { "tdb_vmap_count",                   &tdb_vmap_count }
    , { "tdb_vmap_erase",                   &tdb_vmap_erase }
    , { "tdb_vmap_clear",                   &tdb_vmap_clear }

    /* Batch manipulation */
    , { "tdb_vmap_reset",                   &tdb_vmap_reset }
    , { "tdb_vmap_set_batch",               &tdb_vmap_set_batch }
    , { "tdb_vmap_add_batch",               &tdb_vmap_add_batch }
    , { "tdb_vmap_batch_count",             &tdb_vmap_batch_count }
);

} /* extern "C" { */
