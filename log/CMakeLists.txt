set(LIB_NAME log)
file(GLOB SOURCES
        block_queue.h
        log.cpp
        log.h
        )
add_library(${LIB_NAME} STATIC ${SOURCES})