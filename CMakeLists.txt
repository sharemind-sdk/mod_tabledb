#
# This file is a part of the Sharemind framework.
# Copyright (C) Cybernetica AS
#
# All rights are reserved. Reproduction in whole or part is prohibited
# without the written consent of the copyright owner. The usage of this
# code is subject to the appropriate license agreement.
#

# The tabledb module

# Find packages:
SET(Boost_USE_STATIC_RUNTIME OFF)

FOREACH(dir IN LISTS CMAKE_PREFIX_PATH)
  INCLUDE_DIRECTORIES(BEFORE SYSTEM "${dir}/include")
ENDFOREACH()

FIND_PACKAGE(Boost COMPONENTS thread REQUIRED)
INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
)

ADD_DEFINITIONS("-DSHAREMIND_LOGLEVEL_${LOGLEVEL}"
                "-D__STDC_LIMIT_MACROS"
                "-D__STDC_CONSTANT_MACROS"
                "-D__STDC_FORMAT_MACROS")

SET(TABLEDB_SOURCES
    TdbConfiguration.cpp
    TdbModule.cpp
    TdbTypesUtil.cpp
    TdbVectorMap.cpp
    TdbVectorMapUtil.cpp
    mod_tabledb.cpp
)

SET(TABLEDB_HEADERS
    TdbConfiguration.h
    TdbModule.h
    TdbTypesUtil.h
    TdbVectorMap.h
    TdbVectorMapUtil.h
    tdbtypes.h
    tdbvectormapapi.h
)

# compile the library
ADD_LIBRARY(sharemind_mod_tabledb MODULE ${TABLEDB_SOURCES} ${TABLEDB_HEADERS})
SET_TARGET_PROPERTIES(sharemind_mod_tabledb PROPERTIES
                      COMPILE_FLAGS "-O2 -ggdb")

# Use these libraries during linking
TARGET_LINK_LIBRARIES(sharemind_mod_tabledb

################################################################################
##  THE FOLLOWING LIBRARIES ARE CURRENTLY PROVIDED HERE ONLY BECAUSE OUR      ##
##  INTERFACES ARE EPICALLY BROKEN. THIS LINK-LEVEL DEPENDENCY HACK MUST BE   ##
##  REMOVED, BECAUSE IT MAY CAUSE RUNTIME DYNAMIC LINKER ERRORS.              ##
################################################################################

    sharemind_dbcommon
)

# Install the library and headers
INSTALL(TARGETS sharemind_mod_tabledb
        LIBRARY DESTINATION "lib")
