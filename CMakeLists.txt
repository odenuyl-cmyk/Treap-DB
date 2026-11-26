cmake_minimum_required(VERSION 4.1)
project(Treap_DB)

set(CMAKE_CXX_STANDARD 14)

include_directories(treaps)

add_executable(Treap_DB
        treaps/treap.h
        main.cpp)
