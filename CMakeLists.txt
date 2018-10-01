cmake_minimum_required(VERSION 3.9.4)
project(cpp)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(LLVM_ENABLE_RTTI ON)
add_library(ron STATIC
    ron/const.hpp
    ron/uuid.hpp
    ron/uuid.cc
    ron/uuid-parser.cc
    )
add_executable(test-uuid ron/test-uuid.cc)
target_link_libraries(test-uuid ron)
