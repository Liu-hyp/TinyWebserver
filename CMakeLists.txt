cmake_minimum_required(VERSION 3.17)
project(TinyWebserver)
file(GLOB SOURCES
        *.h
        *.cpp
        )
set(CMAKE_CXX_STANDARD 11)
#如果编译不成功的话可能需要链接库，到时候再来问我
add_subdirectory(timer)
add_subdirectory(Imysql)
add_subdirectory(http)
add_subdirectory(log)
add_executable(TinyWebserver ${SOURCES})
target_link_libraries(TinyWebserver timer Imysql http log)