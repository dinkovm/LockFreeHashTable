cmake_minimum_required (VERSION 2.6)
set (PNAME LockFreeHashTable)
project (${PNAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -o3 -fgnu-tm -Wall")

FILE(GLOB_RECURSE HEADERS "inc/*.h")
file(GLOB SOURCES "src/*.cpp")

include_directories(inc)

add_executable(
    ${PNAME}
    ${HEADERS} ${SOURCES}
)