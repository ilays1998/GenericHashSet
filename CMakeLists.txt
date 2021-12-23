cmake_minimum_required(VERSION 3.20)
project(ex4_ilaysoffer C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ex4_ilaysoffer
        Hash.h
        HashSet.c
        HashSet.h
        Node.c
        Node.h
        Utils.h
        tests.c test_runner.c
        )
