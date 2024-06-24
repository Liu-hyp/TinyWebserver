cmake_minimum_required(VERSION 3.17)
project(TinyWebserver)
file(GLOB SOURCES
        *.h
        *.cpp
        )
set(CMAKE_CXX_STANDARD 17)
find_path(MYSQL_ROOT_DIR mysql)
add_subdirectory(timer)
#add_subdirectory(Imysql)
add_subdirectory(http)
add_subdirectory(log)
include_directories(/user/include/mysql)
add_executable(TinyWebserver ${SOURCES})
#target_link_libraries(TinyWebserver Imysql)
target_link_libraries(TinyWebserver timer http log mysqlclient)
target_link_libraries(TinyWebserver /usr/lib/x86_64-linux-gnu/libjsoncpp.so)
