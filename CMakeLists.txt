cmake_minimum_required(VERSION 3.9.4)
project(cpp)
include(ExternalProject)

#  S E T T I N G S

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(LLVM_ENABLE_RTTI ON)
set(CMAKE_CXX_FLAGS "-frtti")
set(ROCKSDB_PREFIX ${CMAKE_SOURCE_DIR}/deps/rocksdb)
set(CXXOPTS_PREFIX ${CMAKE_SOURCE_DIR}/deps/cxxopts)

include_directories(".")

#  D E P E N D E N C I E S

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

ExternalProject_Add(cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG 00e8ecc482b79c04942fc2c08773618348bd0a6a
    PREFIX ${CXXOPTS_PREFIX}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CXXOPTS_PREFIX}
    BUILD_COMMAND ${CMAKE_COMMAND} --build .
    INSTALL_COMMAND ""
    )

include_directories(${CXXOPTS_PREFIX}/src/cxxopts/include)

#  L I B R O N

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

#  L I B R O N  U N I T  T E S T S

add_executable(test-uuid ron/test/uuid.cc)
target_link_libraries(test-uuid ron)

add_executable(test-text ron/test/text.cc)
target_link_libraries(test-text ron)

#  L I B R D T  ( H E A D E R  O N L Y )

add_library(rdt INTERFACE)
target_include_directories(rdt INTERFACE rdt/)

#  R D T  U N I T  T E S T S

add_executable(test-lww rdt/test/lww.cc)
target_link_libraries(test-lww ron rdt)

#  S W A R M D B  C L I

add_executable(swarmdb 
    db/replica.hpp
    db/replica.cc
    db/main.cc
    )
add_dependencies(swarmdb rocksdblib cxxopts)
target_link_libraries(swarmdb ron rocksdb pthread z bz2 snappy)
