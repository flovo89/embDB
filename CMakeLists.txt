cmake_minimum_required (VERSION 3.2)
project (embdb)

set(APP_NAME embdb)
set(APP_LIB_NAME ${APP_NAME}_lib)
set(EMBDB_LAYOUT_DIR ${PROJECT_SOURCE_DIR}/embdb-layout)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Werror -Wextra -fconcepts)

# App version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_BUGFIX 0)

# Compilation date
EXECUTE_PROCESS(COMMAND "date" "-u" "+%F %R" OUTPUT_VARIABLE COMPILE_DATE)
string(REGEX REPLACE "\n$" "" COMPILE_DATE "${COMPILE_DATE}")

# Git version
EXECUTE_PROCESS(COMMAND
  "git" "describe" "--always" "--tags" "--dirty" "--abbrev=5"
  OUTPUT_VARIABLE GIT_HASH
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
string(REGEX REPLACE "\n$" "" GIT_HASH "${GIT_HASH}")

configure_file (
  "${PROJECT_SOURCE_DIR}/src/version.hpp.in"
  "${PROJECT_SOURCE_DIR}/src/version.hpp"
)

# Externals librarires
find_package(PkgConfig REQUIRED)
pkg_search_module(PROTO_BUF protobuf>=3.14.0)
pkg_search_module(LOG4CPLUS log4cplus>=1.2.0)
pkg_search_module(LIBEVENT REQUIRED libevent>=2.0)

# Compile the app
add_subdirectory(src)

# Compile the unit tests
option(COMPILE_GTESTS "Compile the tests" ON)
if(COMPILE_GTESTS)
  add_subdirectory(gtests)
endif(COMPILE_GTESTS)