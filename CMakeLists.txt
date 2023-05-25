cmake_minimum_required(VERSION 3.25.2)
project(llvm-pointer-deref)

if(NOT DEFINED ENV{LLVM_INSTALL_DIR})
    message(FATAL_ERROR "Missing environment variable: LLVM_INSTALL_DIR")
endif()
if(NOT DEFINED ENV{LLVM_INCLUDE_DIRS})
    message(FATAL_ERROR "Missing environment variable: LLVM_INCLUDE_DIRS" )
endif()

set(LLVM_INSTALL_DIR $ENV{LLVM_INSTALL_DIR})
set(LLVM_INCLUDE_DIRS $ENV{LLVM_INCLUDE_DIRS})
message("LLVM_INSTALL_DIR set to:" ${LLVM_INSTALL_DIR})
message("LLVM_INCLUDE_DIRS set to:" ${LLVM_INCLUDE_DIRS})

list(APPEND CMAKE_PREFIX_PATH "${LLVM_INSTALL_DIR}/lib/cmake/llvm/")

find_package(LLVM 17 REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS}) # comment out this line to make it work

set(CMAKE_CXX_STANDARD 17 CACHE STRING "")

if(NOT LLVM_ENABLE_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

add_library(PointerDeref SHARED pointer-deref.cpp)