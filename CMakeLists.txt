cmake_minimum_required(VERSION 4.1)
project(Treap_DB)

set(CMAKE_CXX_STANDARD 14)

include_directories(treaps)

add_executable(Treap_DB
        treaps/treap.h
        main.cpp)

add_executable(Treap_verify
        treaps/treap.h
        treap_test.cpp)

add_executable(Treap_benchmark
        treaps/treap.h
        treap_benchmark.cpp)