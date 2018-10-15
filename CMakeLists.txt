cmake_minimum_required(VERSION 3.9.4)
project(cpp)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(LLVM_ENABLE_RTTI ON)
set(CMAKE_CXX_FLAGS "-frtti")

include_directories(".")

add_library(ron STATIC
    ron/const.hpp
    ron/uuid.hpp
    ron/op.hpp
    ron/text.hpp
    ron/frames.hpp
    ron/status.hpp
    ron/ron.hpp
    ron/uuid.cc
    ron/uuid-parser.cc
    ron/op.cc
    ron/text.cc
    ron/text-parser.cc
    ron/text-builder.cc
        ron/slice.hpp)
add_executable(test-uuid ron/test/uuid.cc)
add_executable(test-text ron/test/text.cc)
target_link_libraries(test-uuid ron)
target_link_libraries(test-text ron)

add_library(rdt STATIC
    rdt/rdt.hpp
    rdt/merge.hpp
    rdt/lww.hpp
    )
target_link_libraries(rdt ron)

add_executable(test-lww rdt/test/lww.cc)
target_link_libraries(test-lww ron rdt)
