set(LIB_NAME timer)
file(GLOB SOURCES
        timer.cpp
        timer.h
    )
add_library(${LIB_NAME} STATIC ${SOURCES})
