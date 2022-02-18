#
# This file is a part of the Sharemind framework.
# Copyright (C) Cybernetica AS
#
# All rights are reserved. Reproduction in whole or part is prohibited
# without the written consent of the copyright owner. The usage of this
# code is subject to the appropriate license agreement.
#

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(SharemindModTableDb VERSION 0.4.0 LANGUAGES CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

FIND_PACKAGE(SharemindCMakeHelpers 1.6 REQUIRED)


FIND_PACKAGE(Boost 1.62 REQUIRED)
FIND_PACKAGE(LogHard 0.5.0 REQUIRED)
FIND_PACKAGE(SharemindCHeaders 1.3.0 REQUIRED)
FIND_PACKAGE(SharemindCxxHeaders 0.9.0 REQUIRED)
FIND_PACKAGE(SharemindDataStoreApi 0.1.0 REQUIRED)
FIND_PACKAGE(SharemindLibAccessControlProcessFacility 0.3.0 REQUIRED)
FIND_PACKAGE(SharemindLibConfiguration 0.5.0 REQUIRED)
FIND_PACKAGE(SharemindLibConsensusService 0.2.0 REQUIRED)
FIND_PACKAGE(SharemindLibDbCommon 0.2.0 REQUIRED)
FIND_PACKAGE(SharemindLibModapi 0.4.0 REQUIRED)
FIND_PACKAGE(SharemindLibProcessFacility 0.2.0 REQUIRED)
FIND_PACKAGE(SharemindModuleApis 1.1.0 REQUIRED)


# Headers:
SET(SharemindModTableDb_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/TdbTypesUtil.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/tdberror.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/tdbtypes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/tdbvectormapapi.h"
)
INSTALL(FILES ${SharemindModTableDb_HEADERS}
        DESTINATION "include/sharemind/mod_tabledb"
        COMPONENT "dev")


# The module:
FILE(GLOB SharemindModTableDb_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
FILE(GLOB SharemindModTableDb_HEADERS_P "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
LIST(REMOVE_ITEM SharemindModTableDb_HEADERS_P ${SharemindModTableDb_HEADERS})
SharemindAddSharedLibrary(ModTableDb
    OUTPUT_NAME "sharemind_mod_tabledb"
    # TODO: Not MODULE, because mod_tabledb_* need to link back to it.
    SOURCES
        ${SharemindModTableDb_SOURCES}
        ${SharemindModTableDb_HEADERS}
        ${SharemindModTableDb_HEADERS_P}
    )
TARGET_INCLUDE_DIRECTORIES(ModTableDb
    INTERFACE # TODO Remove when ModTableDb is converted back to AGAIN again:
        # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src> # TODO
        $<INSTALL_INTERFACE:include>
    )
TARGET_COMPILE_DEFINITIONS(ModTableDb
    PRIVATE
        "SHAREMIND_INTERNAL_"
    PUBLIC
        "__STDC_LIMIT_MACROS"
        "__STDC_CONSTANT_MACROS"
        "__STDC_FORMAT_MACROS"
    )
TARGET_LINK_LIBRARIES(ModTableDb
    PRIVATE
        Boost::boost
        LogHard::LogHard
        Sharemind::CHeaders
        Sharemind::CxxHeaders
        Sharemind::LibAccessControlProcessFacility
        Sharemind::LibConfiguration
        Sharemind::LibConsensusService
        Sharemind::LibDbCommon
        Sharemind::LibModapi
        Sharemind::LibProcessFacility
        Sharemind::ModuleApis
    PUBLIC
        Sharemind::DataStoreApi
    )
SharemindCreateCMakeFindFilesForTarget(ModTableDb
    DEPENDENCIES
        "SharemindDataStoreApi 0.1.0"
    )

# Configuration files:
INSTALL(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/packaging/configs/sharemind/"
        DESTINATION "/etc/sharemind/"
        COMPONENT "lib"
        EXCLUDE_FROM_ALL)

# Packaging:
SharemindSetupPackaging()
SharemindAddComponentPackage("lib"
    NAME "libsharemind-mod-tabledb"
    DESCRIPTION "Sharemind TableDB module"
    DEB_SECTION "libs"
    DEB_DEPENDS
        "libsharemind-modapi (>= 0.4.0)"
        "libsharemind-configuration (>= 0.5.0)"
        "libloghard (>= 0.5.0)"
        "libstdc++6 (>= 4.8.0)"
        "libc6 (>= 2.19)"
  DEB_EXTRA_CONTROL_FILES
      "${CMAKE_CURRENT_SOURCE_DIR}/packaging/debian/conffiles"
)
SharemindAddComponentPackage("dev"
    NAME "libsharemind-mod-tabledb-dev"
    DESCRIPTION "Sharemind TableDB module development headers"
    DEB_SECTION "libdevel"
    DEB_DEPENDS
        "libsharemind-mod-tabledb (= ${SharemindModTableDb_DEB_lib_PACKAGE_VERSION})"
        "libsharemind-data-store-api-dev (>= 0.1.0)"
        "libc6-dev (>= 2.19)"
)
SharemindAddComponentPackage("debug"
    NAME "libsharemind-mod-tabledb-dbg"
    DESCRIPTION "Sharemind TableDb module debug symbols"
    DEB_SECTION "debug"
    DEB_DEPENDS
        "libsharemind-mod-tabledb (= ${SharemindModTableDb_DEB_lib_PACKAGE_VERSION})"
)
SharemindPackagingFinalize()
