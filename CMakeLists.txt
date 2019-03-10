cmake_minimum_required(VERSION 3.9.4)
project(cpp)
include(ExternalProject)
enable_testing()

#  S E T T I N G S}

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(LLVM_ENABLE_RTTI ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -fPIC")
set(DEPS_PREFIX ${CMAKE_SOURCE_DIR}/deps)
set(ROCKSDB_PREFIX ${DEPS_PREFIX}/build/rocksdb)
#set(CXXOPTS_PREFIX ${DEPS_PREFIX}/build/cxxopts)
set(CRYPTO_PREFIX ${DEPS_PREFIX}/build/botan)
set(snappy_PREFIX ${DEPS_PREFIX}/build/snappy)
set(GFLAGS_PREFIX ${DEPS_PREFIX}/build/gflags)
set(DMP_PREFIX ${DEPS_PREFIX}/build/dmp)

include_directories(".")
include_directories(${DEPS_PREFIX}/include/)
link_directories(${DEPS_PREFIX}/lib/)

include(ProcessorCount)
ProcessorCount(CORES)
if(CORES EQUAL 0)
    set(CORES 1)
endif()
set(MAKE_FLAGS "-j${CORES}")

#  D E P E N D E N C I E S

ExternalProject_Add(
    gflags_lib
    PREFIX ${GFLAGS_PREFIX}
    GIT_REPOSITORY git@github.com:gflags/gflags.git
    GIT_TAG 28f50e0
    CMAKE_ARGS ${CMAKE_ARGS} -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS} -DCMAKE_INSTALL_PREFIX:PATH=${DEPS_PREFIX} 
    CMAKE_GENERATOR "Unix Makefiles"
    BUILD_COMMAND $(MAKE) ${MAKE_FLAGS}
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND make install
)
add_library(gflags STATIC IMPORTED)
set_property(TARGET gflags PROPERTY IMPORTED_LOCATION ${DEPS_PREFIX}/lib/libgflags.a)
add_dependencies(gflags gflags_lib)

ExternalProject_Add(
    snappy_lib
    PREFIX ${snappy_PREFIX}
    GIT_REPOSITORY https://github.com/google/snappy.git
    GIT_TAG d4be414dd66026d6736a3741e8a85779ba4ea949
    CMAKE_ARGS ${CMAKE_ARGS} -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS} -DCMAKE_INSTALL_PREFIX:PATH=${DEPS_PREFIX}
    CMAKE_GENERATOR "Unix Makefiles"
    BUILD_COMMAND $(MAKE) ${MAKE_FLAGS}

    BUILD_IN_SOURCE 1
    INSTALL_COMMAND make install
)
#include_directories(${snappy_PREFIX}/include)
add_library(snappy STATIC IMPORTED)
set_property(TARGET snappy PROPERTY IMPORTED_LOCATION ${DEPS_PREFIX}/lib/libsnappy.a)
add_dependencies(snappy snappy_lib)

ExternalProject_Add(rocksdblib
    GIT_REPOSITORY https://github.com/facebook/rocksdb.git
    GIT_TAG v5.15.10
    GIT_PROGRESS true
    UPDATE_COMMAND ""
    PREFIX ${ROCKSDB_PREFIX}
    CMAKE_ARGS ${CMAKE_ARGS} -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS} -DCMAKE_INSTALL_PREFIX:PATH=${DEPS_PREFIX}
    CMAKE_GENERATOR "Unix Makefiles"
    BUILD_COMMAND make rocksdb ${MAKE_FLAGS}
    INSTALL_COMMAND cp -r ${ROCKSDB_PREFIX}/src/rocksdblib/include/rocksdb ${DEPS_PREFIX}/include && cp librocksdb.a ${DEPS_PREFIX}/lib
)
add_library(rocksdb STATIC IMPORTED)
set_property(TARGET rocksdb PROPERTY IMPORTED_LOCATION ${DEPS_PREFIX}/lib/librocksdb.a)
add_dependencies(rocksdb rocksdblib)

ExternalProject_Add(botan-project
    GIT_REPOSITORY https://github.com/randombit/botan.git
    GIT_TAG a792728e8941b62761052f5e0d288ba13a016c77
    PREFIX ${CRYPTO_PREFIX}
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ./configure.py --cxxflags=${CMAKE_CXX_FLAGS} --ldflags=${CMAKE_EXE_LINKER_FLAGS} --cc-bin=${CMAKE_CXX_COMPILER} --prefix=${DEPS_PREFIX} --minimized-build --enable-modules=sha3,sha2_64
    BUILD_IN_SOURCE 1
    BUILD_COMMAND make ${MAKE_FLAGS} libbotan-2.a
    INSTALL_COMMAND cp -RL build/include/botan ${DEPS_PREFIX}/include && cp libbotan-2.a ${DEPS_PREFIX}/lib
    )
ExternalProject_Get_Property(botan-project install_dir)
add_library(botan STATIC IMPORTED db/test/chains.cc)
set_property(TARGET botan PROPERTY IMPORTED_LOCATION ${DEPS_PREFIX}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}botan-2${CMAKE_STATIC_LIBRARY_SUFFIX})
include_directories(${DEPS_PREFIX}/include/botan-2)

ExternalProject_Add(
    dmp_diff_lib
    PREFIX ${DMP_PREFIX}
    GIT_REPOSITORY git@github.com:gritzko/myers-diff.git
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND cp ${DMP_PREFIX}/src/dmp_diff_lib/dmp_diff.hpp ${DEPS_PREFIX}/include/
)


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
    ron/vv.hpp
    ron/opmeta.hpp
    ron/status.cc
    ron/uuid.cc
    ron/uuid-parser.cc
    ron/op.cc
    ron/text.cc
    ron/text-parser.cc
    ron/string-parser.cc
    ron/text-builder.cc
    ron/hash.cc
    ron/inc_stack.hpp
        )
add_dependencies(ron botan-project)
set_property(TARGET ron PROPERTY POSITION_INDEPENDENT_CODE TRUE)

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

add_executable(test14-rga rdt/test/rga.cc)
target_link_libraries(test14-rga ron rdt)
add_test(RGA test14-rga)

#  S W A R M D B

set(SWARMDB_SOURCES
        db/replica.hpp
        db/replica.cc
        db/map/const.hpp
        db/map/chain.cc
        db/map/op.cc
        db/map/yarn.cc
        db/map/meta.cc
        db/map/map.hpp
        db/map/csv.cc
        db/map/txt.cc
    )

add_library(swarmdb SHARED
    ${SWARMDB_SOURCES}
        )
add_dependencies(swarmdb rocksdb)
add_dependencies(swarmdb dmp_diff_lib)
target_link_libraries(swarmdb pthread z bz2 snappy botan ron rocksdb)
set_property(TARGET swarmdb PROPERTY POSITION_INDEPENDENT_CODE TRUE)

#  S W A R M D B  U N I T  T E S T S

add_executable(test20-chains db/test/chains.cc)
add_dependencies(test20-chains swarmdb)
target_link_libraries(test20-chains swarmdb)
add_test(CHAINS test20-chains)

add_executable(test21-mappers db/test/mappers.cc)
add_dependencies(test21-mappers swarmdb)
target_link_libraries(test21-mappers swarmdb)
add_test(MAPPERS test21-mappers)

#  S W A R M D B  C L I

add_executable(swarmdb-bin
    ${SWARMDB_SOURCES}
    db/fs.cc
    db/main.cc
    )
add_dependencies(swarmdb-bin rocksdb)
target_link_libraries(swarmdb-bin pthread z bz2 snappy gflags botan ron rocksdb)
SET_TARGET_PROPERTIES(swarmdb-bin PROPERTIES OUTPUT_NAME swarmdb)
add_dependencies(swarmdb-bin dmp_diff_lib)

add_test(bb-lww swarmdb --test ${CMAKE_SOURCE_DIR}/test/rdt/lww.ron)
add_test(bb-rga swarmdb --test ${CMAKE_SOURCE_DIR}/test/rdt/rga.ron)

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
