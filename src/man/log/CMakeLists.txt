cmake_minimum_required( VERSION 2.8.3 )

#add pthreads
find_package( Threads )
add_definitions( ${CMAKE_THREAD_LIBS_INIT} )

set(LOG_SRCS LogModule.cpp)
set(STREAM_SRCS StreamBroker)

add_library(
  log
  STATIC
  ${LOG_SRCS} )

add_library(
  stream
  STATIC
  ${STREAM_SRCS} )

target_link_libraries(
  log
  portals
  messages
)

target_link_libraries(
  stream
  portals
  messages
  pthread
)

add_executable(
  stream_test
  tests/StreamBrokerTest
)

target_link_libraries(
  stream_test
  stream
  ${GTEST_LIBRARY}
)

add_test(
  stream_test
  stream_test
)

add_custom_target(check
                  DEPENDS stream_test)
