cmake_minimum_required (VERSION 2.6)
set (PNAME LockFreeHashTable)
project (${PNAME})

FILE(GLOB_RECURSE HEADERS "inc/*.h")
file(GLOB SOURCES "src/*.cpp")

include_directories(inc)

add_executable(
    ${PNAME}
    ${HEADERS} ${SOURCES}
)