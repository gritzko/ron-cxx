cmake_minimum_required(VERSION 3.9.4)
project(RON C CXX)
include(ExternalProject)
include(GNUInstallDirs)
enable_testing()
FIND_PACKAGE ( Threads REQUIRED )

#  S E T T I N G S

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(LLVM_ENABLE_RTTI ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -fPIC")
set(DEPS_PREFIX ${CMAKE_BINARY_DIR}/deps)
set(ROCKSDB_PREFIX ${DEPS_PREFIX}/build/rocksdb)
#set(CXXOPTS_PREFIX ${DEPS_PREFIX}/build/cxxopts)
set(CRYPTO_PREFIX ${DEPS_PREFIX}/build/botan)
set(snappy_PREFIX ${DEPS_PREFIX}/build/snappy)
set(DMP_PREFIX ${DEPS_PREFIX}/build/dmp)
set(BTREE_PREFIX ${DEPS_PREFIX}/build/btree)
set(GTEST_PREFIX ${DEPS_PREFIX}/build/gtest)

include_directories(${DEPS_PREFIX}/include/)
link_directories(${DEPSLIBDIR}/)
set(FIND_LIBRARY_USE_LIB64_PATHS ON)

#  B U I L D  O P T I O N S

include(ProcessorCount)
ProcessorCount(CORES)
if(CORES EQUAL 0)
    set(CORES 1)
endif()
set(MAKE_FLAGS "-j${CORES}")

set(BOTAN_CC gcc)
set(TEST_CXX_FLAGS -g -O1)
set(TEST_LDD_FLAGS -g -O1)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(BOTAN_CC clang)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  set(BOTAN_CC clang)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(BOTAN_CC gcc)
endif()

set(LIBDIR "${CMAKE_INSTALL_LIBDIR}")
set(DEPSLIBDIR "${DEPS_PREFIX}/${LIBDIR}")

file(MAKE_DIRECTORY "${DEPS_PREFIX}/include/" "${DEPS_PREFIX}/${LIBDIR}/")

set(FWD_ARGS ${CMAKE_ARGS} --parallel=${CORES} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS} -DCMAKE_INSTALL_LIBDIR=${LIBDIR} -DCMAKE_INSTALL_PREFIX=${DEPS_PREFIX})

set(ROCKS_CMAKE_ARGS ${FWD_ARGS} -DWITH_TESTS=OFF -DWITH_TOOLS=OFF -DFAIL_ON_WARNINGS=OFF -DWITH_GFLAGS=OFF)
set(SNAPPY_CMAKE_ARGS ${FWD_ARGS} -DSNAPPY_BUILD_TESTS=OFF)

option(WITH_ASAN "build with ASAN" OFF)
if(WITH_ASAN)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
  set(ROCKS_CMAKE_ARGS ${ROCKS_CMAKE_ARGS} -DWITH_ASAN=ON)
endif()

option(WITH_TSAN "build with TSAN" OFF)
if(WITH_TSAN)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread -pie")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -fPIC")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread -fPIC")
  set(ROCKS_CMAKE_ARGS ${ROCKS_CMAKE_ARGS} -DWITH_TSAN=ON)
endif()

option(WITH_UBSAN "build with UBSAN" OFF)
if(WITH_UBSAN)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined")
  set(ROCKS_CMAKE_ARGS ${ROCKS_CMAKE_ARGS} -DWITH_UBSAN=ON)
endif()

option (COLOR_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
if (${COLOR_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always ${CMAKE_CXX_FLAGS}")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       set(CMAKE_CXX_FLAGS "-fcolor-diagnostics ${CMAKE_CXX_FLAGS}")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
       set(CMAKE_CXX_FLAGS "-fcolor-diagnostics ${CMAKE_CXX_FLAGS}")
    endif ()
endif ()

#  D E P E N D E N C I E S

ExternalProject_Add(
    gtest_pro
    PREFIX ${GTEST_PREFIX}
    INSTALL_DIR ${DEPS_PREFIX}
    CMAKE_ARGS ${FWD_ARGS}
    URL https://github.com/google/googletest/archive/release-1.8.1.tar.gz
    URL_HASH SHA1=152b849610d91a9dfa1401293f43230c2e0c33f8
    BUILD_IN_SOURCE 1
    UPDATE_COMMAND ""
    BUILD_BYPRODUCTS ${DEPSLIBDIR}/libgtest.a
)
add_library(gtest_static STATIC IMPORTED)
set_property(TARGET gtest_static PROPERTY IMPORTED_LOCATION ${DEPSLIBDIR}/libgtest.a)
add_dependencies(gtest_static gtest_pro)
if (EXISTS ${DEPSLIBDIR}/libgtestd.a)
    file(RENAME ${DEPSLIBDIR}/libgtestd.a ${DEPSLIBDIR}/libgtest.a)
endif()

ExternalProject_Add(
    snappy_pro
    PREFIX ${snappy_PREFIX}
    URL https://github.com/google/snappy/archive/1.1.7.tar.gz
    URL_HASH SHA1=1ec676b842fc96fd8a95b03c12758935e7f257b0
    CMAKE_ARGS ${SNAPPY_CMAKE_ARGS} 
    BUILD_IN_SOURCE 1
    BUILD_BYPRODUCTS ${DEPSLIBDIR}/libsnappy.a
)
#include_directories(${snappy_PREFIX}/include)
add_library(snappy_static STATIC IMPORTED)
set_property(TARGET snappy_static PROPERTY IMPORTED_LOCATION ${DEPSLIBDIR}/libsnappy.a)
add_dependencies(snappy_static snappy_pro)
add_dependencies(snappy_pro gtest_pro)

ExternalProject_Add(
    rocksdb_pro
    URL https://github.com/facebook/rocksdb/archive/v5.18.3.tar.gz
    URL_HASH SHA1=dfaaff14d447d9c05f3dbcb84ba6640f4590f634
    UPDATE_COMMAND ""
    PREFIX "${ROCKSDB_PREFIX}"
    CMAKE_ARGS "${ROCKS_CMAKE_ARGS}"
#    CMAKE_GENERATOR "Unix Makefiles"
#    BUILD_COMMAND ${CMAKE_COMMAND} ${CMAKE_ARGS} --build . --target rocksdb
#    because of Mac OS X we have to use cmake install, which builds the shared version of hte library
#    that increases build time x2, so we are waiting eagerly for https://github.com/facebook/rocksdb/issues/4484
#    and https://github.com/facebook/rocksdb/pull/4972 to be resolved
#    INSTALL_COMMAND cp -r ${ROCKSDB_PREFIX}/src/rocksdb_pro/include/rocksdb ${DEPS_PREFIX}/include && cp librocksdb.a ${DEPS_PREFIX}/lib
    BUILD_IN_SOURCE 1
    BUILD_BYPRODUCTS ${DEPSLIBDIR}/librocksdb.a
)
add_library(rocksdb_static STATIC IMPORTED)
set_property(TARGET rocksdb_static PROPERTY IMPORTED_LOCATION ${DEPSLIBDIR}/librocksdb.a)
add_dependencies(rocksdb_static rocksdb_pro)
add_dependencies(rocksdb_pro snappy_pro)


ExternalProject_Add(
    botan_pro
    URL https://botan.randombit.net/releases/Botan-2.9.0.tgz
    URL_HASH SHA1=e6876bc30a1301f56107ac3dc6d395810df08899
    PREFIX ${CRYPTO_PREFIX}
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ./configure.py --cxxflags=${CMAKE_CXX_FLAGS} --ldflags=${CMAKE_EXE_LINKER_FLAGS} --prefix=${DEPS_PREFIX} --minimized-build --enable-modules=sha3,sha2_64 --cc=${BOTAN_CC} --libdir=${LIBDIR}
    BUILD_IN_SOURCE 1
    BUILD_COMMAND make ${MAKE_FLAGS} libbotan-2.a
    INSTALL_COMMAND cp -RL build/include/botan ${DEPS_PREFIX}/include && cp libbotan-2.a ${DEPSLIBDIR}
    BUILD_BYPRODUCTS ${DEPSLIBDIR}/libbotan-2.a
    )
ExternalProject_Get_Property(botan_pro install_dir)
add_library(botan_static STATIC IMPORTED db/test/chains.cc)
set_property(TARGET botan_static PROPERTY IMPORTED_LOCATION ${DEPSLIBDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}botan-2${CMAKE_STATIC_LIBRARY_SUFFIX})

ExternalProject_Add(
    dmp_diff_header
    PREFIX ${DMP_PREFIX}
    GIT_REPOSITORY https://github.com/gritzko/myers-diff.git
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND cp ${DMP_PREFIX}/src/dmp_diff_header/dmp_diff.hpp ${DEPS_PREFIX}/include/
    UPDATE_DISCONNECTED 1
)

set(BTREE_SRC ${BTREE_PREFIX}/src/cpp_btree_header/)
set(BTREE_HEADERS
    ${BTREE_SRC}/btree.h
    ${BTREE_SRC}/btree_container.h
    ${BTREE_SRC}/btree_map.h
    ${BTREE_SRC}/btree_set.h
)
ExternalProject_Add(
    cpp_btree_header
    PREFIX ${BTREE_PREFIX}
    URL https://github.com/swarmdb/cpp-btree/archive/1.0.1b.tar.gz
    URL_HASH SHA1=a3bad7c27643351733309adecc624307ae86a653
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy ${BTREE_HEADERS} ${DEPS_PREFIX}/include/
    )

#  L I B R O N

list(APPEND RON_HEADERS
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
    ron/inc_stack.hpp
    ron/form.hpp
)

list(APPEND RON_SOURCES
    ron/status.cc
    ron/uuid.cc
    ron/uuid-parser.cc
    ron/op.cc
    ron/text.cc
    ron/text-parser.cc
    ron/string-parser.cc
    ron/text-builder.cc
    ron/cp-parser.cc
    ron/hash.cc
    )

add_library(ron_static STATIC
    ${RON_HEADERS}
    ${RON_SOURCES}
)
add_dependencies(ron_static botan_pro)
set_property(TARGET ron_static PROPERTY POSITION_INDEPENDENT_CODE TRUE)

#  L I B R O N  U N I T  T E S T S

add_executable(test01-uuid ron/test/uuid.cc)
target_compile_options(test01-uuid PRIVATE ${TEST_CXX_FLAGS})
target_link_libraries(test01-uuid ${TEST_LDD_FLAGS} ron_static gtest_static Threads::Threads)
add_test(UUID test01-uuid)

add_executable(test02-text ron/test/text.cc)
target_compile_options(test02-text PRIVATE ${TEST_CXX_FLAGS})
target_link_libraries(test02-text ${TEST_LDD_FLAGS} ron_static gtest_static Threads::Threads)
add_test(TEXT test02-text)

add_executable(test03-hash ron/test/hash.cc)
target_compile_options(test03-hash PRIVATE ${TEST_CXX_FLAGS})
target_link_libraries(test03-hash ${TEST_LDD_FLAGS} ron_static botan_static gtest_static Threads::Threads)
add_test(HASH test03-hash)

add_executable(test04-meta ron/test/meta.cc)
target_compile_options(test04-meta PRIVATE ${TEST_CXX_FLAGS})
target_link_libraries(test04-meta ${TEST_LDD_FLAGS} ron_static botan_static)
add_test(HASH test04-meta)

add_executable(test05-cp ron/test/cp.cc)
target_compile_options(test05-cp PRIVATE ${TEST_CXX_FLAGS})
target_link_libraries(test05-cp ${TEST_LDD_FLAGS} ron_static gtest_static Threads::Threads)
add_test(CODEPOINTS test05-cp)

set(FUZZ_CXX_FLAGS $<$<C_COMPILER_ID:Clang>:-g -O1 -fsanitize=fuzzer>)
set(FUZZ_LDD_FLAGS $<$<C_COMPILER_ID:Clang>:-fsanitize=fuzzer>)

add_executable(fuzz01-text EXCLUDE_FROM_ALL ron/test/fuzz-text.cc)
target_compile_options(fuzz01-text PRIVATE ${FUZZ_CXX_FLAGS})
target_link_libraries(fuzz01-text PRIVATE ${FUZZ_LDD_FLAGS} ron_static)

#  L I B R D T  ( H E A D E R  O N L Y )

list(APPEND RDT_HEADERS
    rdt/chain.hpp
    rdt/log.hpp
    rdt/lww.hpp
    rdt/max.hpp
    rdt/merge.hpp
    rdt/meta.hpp
    rdt/mx.hpp
    rdt/rdt.hpp
    rdt/rga.hpp
)
add_library(rdt_headers INTERFACE)
target_include_directories(rdt_headers INTERFACE rdt/)

#  R D T  U N I T  T E S T S

add_executable(test10-lww rdt/test/lww.cc)
target_compile_options(test10-lww PRIVATE ${TEST_CXX_FLAGS})
target_link_libraries(test10-lww PRIVATE ${TEST_LDD_FLAGS} ron_static rdt_headers Threads::Threads gtest_static)
add_test(LWW test10-lww)

#add_executable(test11-metardt rdt/test/meta.cc)
#target_link_libraries(test11-metardt ron_static rdt_headers)
#add_test(META test11-metardt)

add_executable(test12-mx rdt/test/mx.cc)
target_compile_options(test12-mx PRIVATE ${TEST_CXX_FLAGS})
target_link_libraries(test12-mx PRIVATE ${TEST_LDD_FLAGS} ron_static rdt_headers)
add_test(LWW test12-mx)

add_executable(test14-rga rdt/test/rga.cc)
target_compile_options(test14-rga PRIVATE ${TEST_CXX_FLAGS})
target_link_libraries(test14-rga PRIVATE ${TEST_LDD_FLAGS} ron_static rdt_headers Threads::Threads gtest_static)
add_test(RGA test14-rga)

add_executable(test15-max rdt/test/max.cc)
target_compile_options(test15-max PRIVATE ${TEST_CXX_FLAGS})
target_link_libraries(test15-max PRIVATE ${TEST_LDD_FLAGS} ron_static rdt_headers Threads::Threads gtest_static)
add_test(MAX test15-max)

#  S W A R M D B

list(APPEND SWARMDB_HEADERS
        db/key.hpp
        db/const.hpp
        db/replica.hpp
        db/map.hpp
        db/mem_store.hpp
        db/rocks_store.hpp
        db/joined_store.hpp
)
list(APPEND SWARMDB_HEADERS_map
        db/map/csv.hpp
        db/map/txt.hpp
)

set(SWARMDB_SOURCES
        ${SWARMDB_HEADERS}
        ${SWARMDB_HEADERS_map}
        db/key.cc
        db/replica.cc
        db/rocks_store.cc
    )

add_library(swarmdb_shared SHARED
    ${SWARMDB_SOURCES}
        )
add_dependencies(swarmdb_shared rocksdb_static)
add_dependencies(swarmdb_shared dmp_diff_header)
add_dependencies(swarmdb_shared cpp_btree_header)
target_link_libraries(swarmdb_shared pthread z bz2 snappy_static botan_static ron_static rocksdb_static)
set_property(TARGET swarmdb_shared PROPERTY POSITION_INDEPENDENT_CODE TRUE)
SET_TARGET_PROPERTIES(swarmdb_shared PROPERTIES OUTPUT_NAME swarmdb)

#  S W A R M D B  U N I T  T E S T S

add_executable(test20-chains db/test/chains.cc)
target_compile_options(test20-chains PRIVATE ${TEST_CXX_FLAGS})
add_dependencies(test20-chains swarmdb_shared)
target_link_libraries(test20-chains PRIVATE ${TEST_LDD_FLAGS} swarmdb_shared)
add_test(CHAINS test20-chains)

add_executable(test21-mappers db/test/mappers.cc)
target_compile_options(test21-mappers PRIVATE ${TEST_CXX_FLAGS})
add_dependencies(test21-mappers swarmdb_shared)
target_link_libraries(test21-mappers PRIVATE ${TEST_LDD_FLAGS} swarmdb_shared gtest_static)
add_test(MAPPERS test21-mappers)

add_executable(test22-memstore db/test/memstore.cc)
target_compile_options(test22-memstore PRIVATE ${TEST_CXX_FLAGS})
add_dependencies(test22-memstore swarmdb_shared)
target_link_libraries(test22-memstore PRIVATE ${TEST_LDD_FLAGS} swarmdb_shared gtest_static)
add_test(MEMSTORE test22-memstore)

add_executable(test23-rocksstore db/test/rocks.cc)
target_compile_options(test23-rocksstore PRIVATE ${TEST_CXX_FLAGS})
add_dependencies(test23-rocksstore swarmdb_shared)
target_link_libraries(test23-rocksstore PRIVATE ${TEST_LDD_FLAGS} swarmdb_shared gtest_static)
add_test(ROCKSTORE test23-rocksstore)

add_executable(test24-joinstore db/test/joinstore.cc)
target_compile_options(test24-joinstore PRIVATE ${TEST_CXX_FLAGS})
add_dependencies(test24-joinstore swarmdb_shared)
target_link_libraries(test24-joinstore PRIVATE ${TEST_LDD_FLAGS} swarmdb_shared gtest_static)
add_test(JOINSTORE test24-joinstore)

#  S W A R M D B  C L I

add_executable(swarmdb_bin
    ${SWARMDB_SOURCES}
    db/fs.hpp
    db/main.cc
)
add_dependencies(swarmdb_bin rocksdb_static)
target_link_libraries(swarmdb_bin pthread z bz2 snappy_static botan_static ron_static rocksdb_static)
SET_TARGET_PROPERTIES(swarmdb_bin PROPERTIES OUTPUT_NAME swarmdb)
add_dependencies(swarmdb_bin dmp_diff_header)
add_dependencies(swarmdb_bin cpp_btree_header)

add_test(bb-lww swarmdb test ${CMAKE_SOURCE_DIR}/test/rdt/lww.ron)
add_test(bb-rga swarmdb test ${CMAKE_SOURCE_DIR}/test/rdt/rga.ron)

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
        ${CMAKE_SOURCE_DIR}/db/map/*.cc
)

#  D O C S

if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
FIND_PACKAGE(Doxygen)
IF(DOXYGEN_FOUND)
    SET(DOXYGEN_INPUT ${CMAKE_SOURCE_DIR})
    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    ADD_CUSTOM_TARGET(doc COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
ELSE(DOXYGEN_FOUND)
    MESSAGE(STATUS "WARNING: Doxygen not found - Reference manual will not be created")
ENDIF(DOXYGEN_FOUND)
endif()

#  I N S T A L L
install(TARGETS swarmdb_shared swarmdb_bin
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    )
install(FILES ${RON_HEADERS} DESTINATION include/swarm/ron)
install(FILES ${RDT_HEADERS} DESTINATION include/swarm/rdt)
install(FILES ${SWARMDB_HEADERS} DESTINATION include/swarm/db)
install(FILES ${SWARMDB_HEADERS_map} DESTINATION include/swarm/db/map)
install(FILES ${BTREE_HEADERS} DESTINATION include)
