cmake_minimum_required (VERSION 2.6)
project (RingBuffer)

set (EXECUTABLE_OUTPUT_PATH ${RingBuffer_SOURCE_DIR}/exe)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g -Wall -Wextra -Werror -Wshadow -lcmockery")

set (SRC_DIR ${RingBuffer_SOURCE_DIR}/src)

set (RingBuffer_SOURCES
  ${SRC_DIR}/test.c
  ${SRC_DIR}/ring_buffer.c
  ${SRC_DIR}/ring_buffer_test.c
)

add_executable (ringbuffertest ${RingBuffer_SOURCES})
