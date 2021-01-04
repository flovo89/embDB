# Common sources
set(APP_SOURCES
  ${PROJECT_SOURCE_DIR}/embdb-layout/cpp/embdb-layout.pb.cc
  database/db-element/DbElement.cpp
  database/IDataBase.cpp
  database/db-layout/DbLayout.cpp
  file-io/FileReader.cpp
  file-io/FileWriter.cpp
  utilities/IHasher.cpp
  utilities/ITimestamper.cpp
  utilities/DefaultHasher.cpp
  utilities/DefaultTimestamper.cpp
  main.cpp
)

# External libraries
pkg_search_module(PROTO_BUF protobuf>=3.14.0)

# Compile protocol buffer .proto file
set(PROTOBUF_COMPILER "protoc" CACHE STRING "Path to protocol buffer compiler")
add_custom_command(
  OUTPUT ${PROJECT_SOURCE_DIR}/embdb-layout/cpp/embdb-layout.pb.cc
  COMMAND make PROTOC=${PROTOBUF_COMPILER} cpp
  DEPENDS ../embdb-layout/embdb-layout.proto
  WORKING_DIRECTORY ${EMBDB_LAYOUT_DIR}
)

include_directories(
  ${EMBDB_LAYOUT_DIR}/cpp
  ${PROTO_BUF_INCLUDE_DIRS}
)

link_directories(
  ${PROTO_BUF_LIBRARY_DIRS}
)

add_library(${APP_LIB_NAME} ${APP_SOURCES})

# Compile the application
add_executable(${APP_NAME} main.cpp)
target_link_libraries(${APP_NAME}
  ${APP_LIB_NAME}
  ${PROTO_BUF_LIBRARIES}
)
install(TARGETS ${APP_NAME} DESTINATION bin/)
