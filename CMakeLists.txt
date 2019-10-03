cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(ProducerConsumer)

set(CMAKE_CXX_STANDART 11)
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/sources)
set(HEADERS_DIR ${PROJECT_SOURCE_DIR}/headers)

include_directories(${HEADERS_DIR})

add_executable(ProducerConsumer ${SOURCES_DIR}/main.cpp
        ${HEADERS_DIR}/datamanager.h
        ${HEADERS_DIR}/producer.h
        ${HEADERS_DIR}/consumer.h)

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(ProducerConsumer PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(ProducerConsumer "${CMAKE_THREAD_LIBS_INIT}")
endif()
