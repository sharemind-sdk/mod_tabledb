#
# This file is a part of the Sharemind framework.
# Copyright (C) Cybernetica AS
#
# All rights are reserved. Reproduction in whole or part is prohibited
# without the written consent of the copyright owner. The usage of this
# code is subject to the appropriate license agreement.
#

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(SharemindModTableDb VERSION 0.2.0 LANGUAGES CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

FIND_PACKAGE(SharemindCMakeHelpers 1.2 REQUIRED)

SharemindSetProjectVersion()


FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(LogHard REQUIRED)
FIND_PACKAGE(SharemindCHeaders 1.1.0 REQUIRED)
FIND_PACKAGE(SharemindCxxHeaders REQUIRED)
FIND_PACKAGE(SharemindLibAccessControlProcessFacility REQUIRED)
FIND_PACKAGE(SharemindLibConfiguration REQUIRED)
FIND_PACKAGE(SharemindLibConsensusService REQUIRED)
FIND_PACKAGE(SharemindLibDataStoreManager REQUIRED)
FIND_PACKAGE(SharemindLibDbCommon REQUIRED)
FIND_PACKAGE(SharemindLibModapi REQUIRED)
FIND_PACKAGE(SharemindLibProcessFacility REQUIRED)
FIND_PACKAGE(SharemindModuleApis REQUIRED)


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
SharemindSetCxx11CompileOptions()
FILE(GLOB SharemindModTableDb_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
FILE(GLOB SharemindModTableDb_HEADERS_P "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
LIST(REMOVE_ITEM SharemindModTableDb_HEADERS_P ${SharemindModTableDb_HEADERS})
SharemindAddSharedLibrary("sharemind_mod_tabledb"
    OUTPUT_NAME "sharemind_mod_tabledb"
    MODULE
    SOURCES
        ${SharemindModTableDb_SOURCES}
        ${SharemindModTableDb_HEADERS}
        ${SharemindModTableDb_HEADERS_P}
    INCLUDE_DIRECTORIES
        ${Boost_INCLUDE_DIRS}
        ${LogHard_INCLUDE_DIRS}
        ${SharemindCHeaders_INCLUDE_DIRS}
        ${SharemindCxxHeaders_INCLUDE_DIRS}
        ${SharemindLibAccessControlProcessFacility_INCLUDE_DIRS}
        ${SharemindLibConfiguration_INCLUDE_DIRS}
        ${SharemindLibConsensusService_INCLUDE_DIRS}
        ${SharemindLibDataStoreManager_INCLUDE_DIRS}
        ${SharemindLibDbCommon_INCLUDE_DIRS}
        ${SharemindLibModapi_INCLUDE_DIRS}
        ${SharemindLibProcessFacility_INCLUDE_DIRS}
        ${SharemindModuleApis_INCLUDE_DIRS}
    COMPILE_DEFINITIONS
        "SHAREMIND_INTERNAL_"
    LEGACY_DEFINITIONS
        ${LogHard_DEFINITIONS}
        ${SharemindCHeaders_DEFINITIONS}
        ${SharemindCxxHeaders_DEFINITIONS}
        ${SharemindLibAccessControlProcessFacility_DEFINITIONS}
        ${SharemindLibConfiguration_DEFINITIONS}
        ${SharemindLibConsensusService_DEFINITIONS}
        ${SharemindLibDataStoreManager_DEFINITIONS}
        ${SharemindLibDbCommon_DEFINITIONS}
        ${SharemindLibModapi_DEFINITIONS}
        ${SharemindLibProcessFacility_DEFINITIONS}
    LINK_LIBRARIES
        ${Boost_LIBRARIES}
        ${LogHard_LIBRARIES}
        ${SharemindCHeaders_LIBRARIES}
        ${SharemindCxxHeaders_LIBRARIES}
        ${SharemindLibConfiguration_LIBRARIES}
        ${SharemindLibConsensusService_LIBRARIES}
        ${SharemindLibDbCommon_LIBRARIES}
        ${SharemindLibModapi_LIBRARIES}
)


# Install CMake files:
SharemindCreateCMakeFindFiles(
    INCLUDE_DIRS
        ${SharemindLibDataStoreManager_INCLUDE_DIRS}
        "${CMAKE_INSTALL_PREFIX}/include"
    DEFINITIONS
        ${SharemindLibDataStoreManager_DEFINITIONS}
        "-D__STDC_LIMIT_MACROS"
        "-D__STDC_CONSTANT_MACROS"
        "-D__STDC_FORMAT_MACROS"
    LIBRARIES
        ${SharemindLibModapi_LIBRARIES}
        "-L${CMAKE_INSTALL_PREFIX}/lib"
        "-lsharemind_mod_tabledb"
)


# Packaging:
SharemindSetupPackaging()
SharemindAddComponentPackage("lib"
    NAME "libsharemind-mod-tabledb"
    DESCRIPTION "Sharemind TableDB module"
    DEB_SECTION "libs"
    DEB_DEPENDS
        "libsharemind-modapi (>= 0.3.0)"
        "libsharemind-configuration (>= 0.1.0)"
        "libloghard (>= 0.1.0)"
        "libstdc++6 (>= 4.8.0)"
        "libc6 (>= 2.19)"
)
SharemindAddComponentPackage("dev"
    NAME "libsharemind-mod-tabledb-dev"
    DESCRIPTION "Sharemind TableDB module development headers"
    DEB_SECTION "libdevel"
    DEB_DEPENDS
        "libsharemind-mod-tabledb (= ${SharemindModTableDb_DEB_lib_PACKAGE_VERSION})"
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
