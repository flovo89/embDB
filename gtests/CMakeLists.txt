# Find gtest & gmock pkg-config
pkg_search_module(GTEST REQUIRED gtest_main>=0.1.7)
pkg_search_module(GMOCK REQUIRED gmock>=0.1.7)

set(
  TEST_APP_NAME 
  ${APP_NAME}_gtests
  )

file(
  GLOB_RECURSE 
  TEST_SOURCES 
  LIST_DIRECTORIES 
  false 
  *.hpp 
  *.cpp
  )

include_directories(
  ../src
  ${PROTO_BUF_INCLUDE_DIRS}
  )

link_directories(
  ${PROTO_BUF_LIBRARY_DIRS}
)

set(
  SOURCES 
  ${TEST_SOURCES}
  )

add_executable(
  ${TEST_APP_NAME} 
  ${TEST_SOURCES}
  )

target_link_libraries(
  ${TEST_APP_NAME} 
  PUBLIC 
  ${APP_LIB_NAME} 
  ${GMOCK_LDFLAGS}
  ${GTEST_LDFLAGS}
  ${PROTO_BUF_LIBRARIES}
  )

target_compile_options(
  ${TEST_APP_NAME} 
  PUBLIC 
  ${GMOCK_CFLAGS}
  ${GTEST_CFLAGS}
  )

# Execute the tests if necessary
option(RUN_GTESTS "Run the tests" ON)
if(RUN_GTESTS)
  add_custom_command(TARGET ${TEST_APP_NAME} 
  COMMAND 
  ./${TEST_APP_NAME} 
  POST_BUILD)
else()
  message(STATUS "Unit gtests won't be executed")
endif(RUN_GTESTS)