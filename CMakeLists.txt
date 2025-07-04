cmake_minimum_required(VERSION 3.19)
project(KeyBindHandler)

set(CMAKE_CXX_STANDARD_REQUIRED 11)

set(DIR_LIBRARY_ROOT "${CMAKE_CURRENT_LIST_DIR}")
set(DIR_HEADERS "${DIR_LIBRARY_ROOT}/include")
set(DIR_SOURCES "${DIR_LIBRARY_ROOT}/src")


add_library(KeyBindHandler STATIC
    ${DIR_HEADERS}/KeyBindHandler.hpp
    ${DIR_SOURCES}/KeyBindHandler.cpp
)
add_library(KeyBindHandler::KeyBindHandler ALIAS KeyBindHandler)

target_include_directories(KeyBindHandler PUBLIC ${DIR_HEADERS})

include(GNUInstallDirs)

install(TARGETS KeyBindHandler
        EXPORT KeyBindHandler_targets
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
export( EXPORT KeyBindHandler_targets
        NAMESPACE KeyBindHandler::
        FILE "${CMAKE_INSTALL_LIBDIR}/cmake/KeyBindHandler/KeyBindHandler-config.cmake")
