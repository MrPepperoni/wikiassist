cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(wiki_DEPS)
include(ExternalProject)

set(EXTERNALS_WORK_DIR ${PROJECT_BINARY_DIR}/externals)
if( NOT EXTERNALS_INSTALL_DIR )
    set( EXTERNALS_INSTALL_DIR ${PROJECT_SOURCE_DIR}/../local )
endif()

list( APPEND CMAKE_PREFIX_PATH ${EXTERNALS_INSTALL_DIR} )

set( CONNECTOR_C_URL
    "https://dev.mysql.com/get/Downloads/Connector-C/mysql-connector-c-6.1.6-src.tar.gz"
    CACHE STRING "Location of mysql connector/c source package")
set( CONNECTOR_C_HASH
    "2222433012c415871958b61bc4f3683e1ebe77e3389f698b267058c12533ea78"
    )

ExternalProject_Add(ext_connectorc
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/connectorc"
        BINARY_DIR "${EXTERNALS_WORK_DIR}/build/connectorc"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        URL "${CONNECTOR_C_URL}"
        URL_HASH SHA256=${CONNECTOR_C_HASH}
        CMAKE_ARGS
            -G ${CMAKE_GENERATOR}
            -DCMAKE_INSTALL_PREFIX=${EXTERNALS_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

# use archive 1.1.6, 1.1.7 doesn't compile with the available connector/c versions
set( CONNECTOR_CPP_URL
    "https://downloads.mysql.com/archives/get/file/mysql-connector-c++-1.1.6.tar.gz"
    CACHE STRING "Location of mysql connector/c++ source package")
set( CONNECTOR_CPP_HASH
    "ad710b3900cae3be94656825aa70319cf7a96e1ad46bf93e07275f3606f69447"
    )

ExternalProject_Add(ext_connectorcpp
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/connectorcpp"
        BINARY_DIR "${EXTERNALS_WORK_DIR}/build/connectorcpp"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        URL "${CONNECTOR_CPP_URL}"
        #   URL_HASH SHA256=${CONNECTOR_CPP_HASH}
        CMAKE_ARGS
            -G ${CMAKE_GENERATOR}
            -DCMAKE_INSTALL_PREFIX=${EXTERNALS_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

