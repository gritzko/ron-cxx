cmake_minimum_required(VERSION 3.9.4)
project(cpp)
include(ExternalProject)
enable_testing()

#  S E T T I N G S

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(LLVM_ENABLE_RTTI ON)
set(CMAKE_CXX_FLAGS "-frtti -fno-exceptions")
set(ROCKSDB_PREFIX ${CMAKE_SOURCE_DIR}/deps/rocksdb)
set(CXXOPTS_PREFIX ${CMAKE_SOURCE_DIR}/deps/cxxopts)
set(CRYPTO_PREFIX ${CMAKE_SOURCE_DIR}/deps/botan)

include_directories(".")

#  D E P E N D E N C I E S

ExternalProject_Add(rocksdblib
    GIT_REPOSITORY https://github.com/facebook/rocksdb.git
    GIT_TAG 7e1f37eb4fc711dbf3ecc9610178931f00754de8
    GIT_PROGRESS true
    UPDATE_COMMAND ""
    PREFIX ${ROCKSDB_PREFIX}
    CMAKE_ARGS -DCMAKE_CXX_FLAGS="-frtti" -DCMAKE_INSTALL_PREFIX:PATH=    ${ROCKSDB_PREFIX} -DCMAKE_POSITION_INDEPENDENT_CODE=ON
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --target rocksdb -- -j8
    INSTALL_COMMAND ""
)
include_directories(${ROCKSDB_PREFIX}/src/rocksdblib/include/)
link_directories(${ROCKSDB_PREFIX}/src/rocksdblib-build/)

find_package(gflags REQUIRED)
find_package(gflags COMPONENTS nothreads_static)

ExternalProject_Add(botan-project
    GIT_REPOSITORY git@github.com:randombit/botan.git
    GIT_TAG a792728e8941b62761052f5e0d288ba13a016c77
    PREFIX ${CRYPTO_PREFIX}
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ./configure.py --prefix=${CRYPTO_PREFIX} --minimized-build --enable-modules=sha3,sha2_64
    BUILD_IN_SOURCE 1
    BUILD_COMMAND make libs
    INSTALL_COMMAND make install
    )
ExternalProject_Get_Property(botan-project install_dir)
add_library(botan STATIC IMPORTED db/test/chains.cc)
set_property(TARGET botan PROPERTY IMPORTED_LOCATION ${CRYPTO_PREFIX}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}botan-2${CMAKE_STATIC_LIBRARY_SUFFIX})
include_directories(${CRYPTO_PREFIX}/include/botan-2)

#  L I B R O N

add_library(ron STATIC
    ron/const.hpp
    ron/encdec.hpp
    ron/uuid.hpp
    ron/op.hpp
    ron/text.hpp
    ron/frames.hpp
    ron/status.hpp
    ron/ron.hpp
    ron/slice.hpp
    ron/hash.hpp
    ron/portable_endian.hpp
    ron/status.cc
    ron/uuid.cc
    ron/uuid-parser.cc
    ron/op.cc
    ron/text.cc
    ron/text-parser.cc
    ron/text-builder.cc
    ron/hash.cc
    ron/vv.hpp ron/opmeta.hpp)
add_dependencies(ron botan-project)

#  L I B R O N  U N I T  T E S T S

add_executable(test01-uuid ron/test/uuid.cc)
target_link_libraries(test01-uuid ron)
add_test(UUID test01-uuid)

add_executable(test02-text ron/test/text.cc)
target_link_libraries(test02-text ron)
add_test(TEXT test02-text)

add_executable(test03-hash ron/test/hash.cc)
target_link_libraries(test03-hash ron botan)
add_test(HASH test03-hash)

add_executable(test04-meta ron/test/meta.cc)
target_link_libraries(test04-meta ron botan)
add_test(HASH test04-meta)

#  L I B R D T  ( H E A D E R  O N L Y )

add_library(rdt INTERFACE)
target_include_directories(rdt INTERFACE rdt/)

#  R D T  U N I T  T E S T S

add_executable(test10-lww rdt/test/lww.cc)
target_link_libraries(test10-lww ron rdt)
add_test(LWW test10-lww)

add_executable(test11-metardt rdt/test/meta.cc)
target_link_libraries(test11-metardt ron rdt)
add_test(META test11-metardt)

add_executable(test12-mx rdt/test/mx.cc)
target_link_libraries(test12-mx ron rdt)
add_test(LWW test12-mx)

#  S W A R M D B

set(SWARMDB_SOURCES
        db/replica.hpp
        db/replica.cc
        db/map/chain.cc
        db/map/op.cc
        db/map/yarn.cc
        db/map/meta.cc
        db/map/map.hpp
        db/map/csv.cc
    )

add_library(swarmdb SHARED
    ${SWARMDB_SOURCES}
        )
add_dependencies(swarmdb rocksdblib)
target_link_libraries(swarmdb ron botan rocksdb pthread z bz2 snappy)

#  S W A R M D B  U N I T  T E S T S

add_executable(test20-chains db/test/chains.cc)
add_dependencies(test20-chains swarmdb)
target_link_libraries(test20-chains swarmdb)
add_test(CHAINS test20-chains)

#  S W A R M D B  C L I

add_executable(swarmdb-bin
    ${SWARMDB_SOURCES}
    db/main.cc
    )
add_dependencies(swarmdb-bin rocksdblib)
target_link_libraries(swarmdb-bin ron botan rocksdb pthread z bz2 snappy gflags)
SET_TARGET_PROPERTIES(swarmdb-bin PROPERTIES OUTPUT_NAME swarmdb)

add_custom_target(
        format
        COMMAND clang-format
        -style='{BasedOnStyle: Google, IndentWidth: 4}'
        -i
        ${CMAKE_SOURCE_DIR}/ron/*.cc
        ${CMAKE_SOURCE_DIR}/ron/*.hpp
        ${CMAKE_SOURCE_DIR}/rdt/*.hpp
        ${CMAKE_SOURCE_DIR}/db/*.cc
        ${CMAKE_SOURCE_DIR}/db/*.hpp
)

#  D O C S

FIND_PACKAGE(Doxygen)
IF(DOXYGEN_FOUND)
    SET(DOXYGEN_INPUT ${CMAKE_SOURCE_DIR})
    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    ADD_CUSTOM_TARGET(doc COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
ELSE(DOXYGEN_FOUND)
    MESSAGE(STATUS "WARNING: Doxygen not found - Reference manual will not be created")
ENDIF(DOXYGEN_FOUND)
