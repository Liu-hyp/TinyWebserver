set(LIB_NAME http)
file(GLOB SOURCES
        http_connect.cpp
        http_connect.h
        )
add_library(${LIB_NAME} STATIC ${SOURCES})