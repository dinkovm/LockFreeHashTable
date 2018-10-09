cmake_minimum_required (VERSION 2.6)
project (LockFreeHashTable)

include_directories(inc)

file(GLOB SOURCES "src/*.cpp")

add_executable(LockFreeHashTable ${SOURCES})