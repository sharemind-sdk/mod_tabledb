#
# This file is a part of the Sharemind framework.
# Copyright (C) Cybernetica AS
#
# All rights are reserved. Reproduction in whole or part is prohibited
# without the written consent of the copyright owner. The usage of this
# code is subject to the appropriate license agreement.
#
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
PROJECT(SHAREMIND_MOD_TABLEDB)

SET(SHAREMIND_MOD_TABLEDB_VERSION "0.1.0")

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

ADD_COMPILE_OPTIONS(
  "-std=c++11" "-Wall" "-Wextra" "-O2" "-fPIC"
  "$<$<NOT:$<STREQUAL:$<CONFIGURATION>,Release>>:-ggdb>"
  "$<$<NOT:$<STREQUAL:$<CONFIGURATION>,Release>>:-fno-omit-frame-pointer>"
  "$<$<STREQUAL:$<CONFIGURATION>,Release>:-DNDEBUG>"
  "$<$<STREQUAL:$<CONFIGURATION>,Release>:-fomit-frame-pointer>"
)

FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(LogHard REQUIRED)
FIND_PACKAGE(SharemindCHeaders REQUIRED)
FIND_PACKAGE(SharemindCxxHeaders REQUIRED)
FIND_PACKAGE(SharemindLibConfiguration REQUIRED)
FIND_PACKAGE(SharemindLibConsensusService REQUIRED)
FIND_PACKAGE(SharemindLibDataStoreManager REQUIRED)
FIND_PACKAGE(SharemindLibDbCommon REQUIRED)
FIND_PACKAGE(SharemindLibmodapi REQUIRED)
INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    ${LogHard_INCLUDE_DIRS}
    ${SharemindCHeaders_INCLUDE_DIRS}
    ${SharemindCxxHeaders_INCLUDE_DIRS}
    ${SharemindLibConfiguration_INCLUDE_DIRS}
    ${SharemindLibConsensusService_INCLUDE_DIRS}
    ${SharemindLibDataStoreManager_INCLUDE_DIRS}
    ${SharemindLibDbCommon_INCLUDE_DIRS}
    ${SharemindLibmodapi_INCLUDE_DIRS}
)

ADD_DEFINITIONS(
    "-DSHAREMIND_INTERNAL_"
    "-D__STDC_LIMIT_MACROS"
    "-D__STDC_CONSTANT_MACROS"
    "-D__STDC_FORMAT_MACROS"
    ${LogHard_DEFINITIONS}
    ${SharemindCHeaders_DEFINITIONS}
    ${SharemindCxxHeaders_DEFINITIONS}
    ${SharemindLibConfiguration_DEFINITIONS}
    ${SharemindLibConsensusService_DEFINITIONS}
    ${SharemindLibDataStoreManager_DEFINITIONS}
    ${SharemindLibDbCommon_DEFINITIONS}
    ${SharemindLibmodapi_DEFINITIONS}
)

FILE(GLOB_RECURSE TABLEDB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
FILE(GLOB_RECURSE TABLEDB_HEADERS_P "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
SET(TABLEDB_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/TdbTypesUtil.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/tdberror.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/tdbtypes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/tdbvectormapapi.h"
)
LIST(REMOVE_ITEM TABLEDB_HEADERS_P ${TABLEDB_HEADERS})

ADD_LIBRARY(sharemind_mod_tabledb MODULE
            ${TABLEDB_SOURCES} ${TABLEDB_HEADERS} ${TABLEDB_HEADERS_P})
TARGET_LINK_LIBRARIES(sharemind_mod_tabledb
    ${Boost_LIBRARIES}
    ${LogHard_LIBRARIES}
    ${SharemindCHeaders_LIBRARIES}
    ${SharemindCxxHeaders_LIBRARIES}
    ${SharemindLibConfiguration_LIBRARIES}
    ${SharemindLibConsensusService_LIBRARIES}
    ${SharemindLibDbCommon_LIBRARIES}
)

INSTALL(TARGETS sharemind_mod_tabledb LIBRARY DESTINATION "lib")
INSTALL(FILES ${TABLEDB_HEADERS} DESTINATION "include/sharemind/mod_tabledb")

SET(SHAREMIND_MOD_TABLEDB_INSTALL_INCLUDE_DIRS
    "${CMAKE_INSTALL_PREFIX}/include")
SET(SHAREMIND_MOD_TABLEDB_INSTALL_LIBRARIES)
SET(SHAREMIND_MOD_TABLEDB_INSTALL_DEFINITIONS
    "-D__STDC_LIMIT_MACROS"
    "-D__STDC_CONSTANT_MACROS"
    "-D__STDC_FORMAT_MACROS"
)

CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/SharemindModTableDbConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/SharemindModTableDbConfig.cmake" @ONLY)
INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/SharemindModTableDbConfig.cmake"
        DESTINATION "lib/SharemindModTableDb")
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/SharemindModTableDbConfigVersion.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/SharemindModTableDbConfigVersion.cmake" @ONLY)
INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/SharemindModTableDbConfigVersion.cmake"
        DESTINATION "lib/SharemindModTableDb")
