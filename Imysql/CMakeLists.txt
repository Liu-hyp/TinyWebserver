set(LIB_NAME Imysql)
file(GLOB SOURCES
sql_connection_pool.cpp
sql_connection_pool.h
        )
add_library(${LIB_NAME} STATIC ${SOURCES})