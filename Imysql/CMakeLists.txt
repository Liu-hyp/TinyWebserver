set(LIB_NAME Imysql)
file(GLOB SOURCES
        sql_connection_pool.cpp
        coonpool.cpp
        mysql_conn.cpp
        )
add_library(${LIB_NAME} STATIC ${SOURCES})