cmake_minimum_required(VERSION 3.9.4)
project(cpp)
include(ExternalProject)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(LLVM_ENABLE_RTTI ON)
set(CMAKE_CXX_FLAGS "-frtti")
set(ROCKSDB_PREFIX ${CMAKE_SOURCE_DIR}/deps/rocksdb)

ExternalProject_Add(rocksdblib
    GIT_REPOSITORY https://github.com/facebook/rocksdb.git
    GIT_TAG 7e1f37eb4fc711dbf3ecc9610178931f00754de8
    GIT_PROGRESS true
    PREFIX ${ROCKSDB_PREFIX}
    CMAKE_ARGS -DCMAKE_CXX_FLAGS="-frtti" -DCMAKE_INSTALL_PREFIX:PATH=    ${ROCKSDB_PREFIX} -DCMAKE_POSITION_INDEPENDENT_CODE=ON
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --target rocksdb -- -j8
    INSTALL_COMMAND ""
)
include_directories(${ROCKSDB_PREFIX}/src/rocksdblib/include/)
link_directories(${ROCKSDB_PREFIX}/src/rocksdblib-build/)

include_directories(".")

add_library(ron STATIC
    ron/const.hpp
    ron/uuid.hpp
    ron/op.hpp
    ron/text.hpp
    ron/frames.hpp
    ron/status.hpp
    ron/ron.hpp
    ron/slice.hpp
    ron/status.cc
    ron/uuid.cc
    ron/uuid-parser.cc
    ron/op.cc
    ron/text.cc
    ron/text-parser.cc
    ron/text-builder.cc
    )
add_executable(test-uuid ron/test/uuid.cc)
add_executable(test-text ron/test/text.cc)
target_link_libraries(test-uuid ron)
target_link_libraries(test-text ron)

add_library(rdt INTERFACE)
target_include_directories(rdt INTERFACE rdt/)

add_executable(test-lww rdt/test/lww.cc)
target_link_libraries(test-lww ron rdt)

add_executable(swarmdb 
    db/replica.hpp
    db/replica.cc
    db/main.cc
    )
add_dependencies(swarmdb rocksdblib)
target_link_libraries(swarmdb ron rocksdb pthread z bz2 snappy)
