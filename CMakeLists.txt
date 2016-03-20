cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(wiki)

set( CMAKE_VERBOSE_MAKEFILE ON )
set( CMAKE_CXX_STANDARD 11 )

set( BUILD_SHARED_LIBS ON )
list( APPEND CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/local )
list( APPEND CMAKE_FIND_ROOT_PATH ${PROJECT_SOURCE_DIR}/local )
link_directories( ${PROJECT_SOURCE_DIR}/local/lib )
include_directories( ${PROJECT_SOURCE_DIR}/local/include )

add_subdirectory( src )

