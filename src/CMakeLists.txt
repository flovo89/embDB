# Common sources
set(APP_SOURCES
  ${PROJECT_SOURCE_DIR}/embdb-layout/cpp/embdb-layout.pb.cc
  database/db-element/DbElement.cpp
  database/IDataBase.cpp
  database/db-layout/DbLayout.cpp
  database/db-guard/DbGuard.cpp
  file-io/FileReader.cpp
  file-io/FileWriter.cpp
  utilities/IHasher.cpp
  utilities/ITimestamper.cpp
  utilities/IMutex.cpp
  utilities/DefaultHasher.cpp
  utilities/DefaultTimestamper.cpp
  utilities/DefaultMutex.cpp
  utilities/NullMutex.cpp
  exception/Exception.cpp
  eventloop/IEventLoop.cpp
  eventloop/IHandledIo.cpp
  eventloop/IHandledSignal.cpp
  eventloop/EventLoopFactory.cpp
  eventloop/EventLoopBreakerOnSignal.cpp
  eventloop/libevent/EventBaseLibevent.cpp
  eventloop/libevent/EventLoopLibevent.cpp
  eventloop/libevent/HandledByLibevent.cpp
  eventloop/libevent/HandledIoLibevent.cpp
  eventloop/libevent/HandledSignalLibevent.cpp
  logging/ILogger.cpp
  logging/ILogSink.cpp
  logging/LogFormatter.cpp
  logging/Logger.cpp
  logging/LoggerFactory.cpp
  logging/Loggers.cpp
  logging/console/LoggerConsole.cpp
  logging/null/LoggerNull.cpp
  process/IDaemonizer.cpp
  process/posix/DaemonizerPosix.cpp
  socket/ISocketServer.cpp
  socket/ISocketClient.cpp
  socket/posix/SocketClient.cpp
  socket/posix/SocketServer.cpp
  server/ISocketServer.cpp
  server/ISocketClient.cpp
  server/buffer/ClientBuffer.cpp
  server/sockets/ISocketClientFactory.cpp
  server/sockets/SocketClientFactory.cpp
  server/sockets/SocketClient.cpp
  server/sockets/SocketServer.cpp
  protocol/IProtocol.cpp
  protocol/IProtocolArbiter.cpp
  protocol/DataObject.cpp
  protocol/ProtocolProcessor.cpp
  protocol/json/JsonProtocol.cpp
  main.cpp
)

# Compile protocol buffer .proto file
set(PROTOBUF_COMPILER "protoc" CACHE STRING "Path to protocol buffer compiler")
add_custom_command(
  OUTPUT ${PROJECT_SOURCE_DIR}/embdb-layout/cpp/embdb-layout.pb.cc
  COMMAND make PROTOC=${PROTOBUF_COMPILER} cpp
  DEPENDS ../embdb-layout/embdb-layout.proto
  WORKING_DIRECTORY ${EMBDB_LAYOUT_DIR}
)

set(LINK_LIBRARIES
  ${APP_LIB_NAME}
  ${PROTO_BUF_LIBRARIES}
  ${Libevent_LIBRARIES}
)

# Log4cplus
configure_file (
  "${PROJECT_SOURCE_DIR}/src/logging/LoggerConfig.hpp.in"
  "${PROJECT_SOURCE_DIR}/src/logging/LoggerConfig.hpp"
)
if(LOG4CPLUS_FOUND)
  set(APP_SOURCES
    ${APP_SOURCES}
    logging/log4cplus/CustomLog4CplusLogsLevels.cpp
    logging/log4cplus/LoggerLog4Cplus.cpp
  )

  include_directories(${LOG4CPLUS_INCLUDE_DIRS})
  set(LINK_LIBRARIES
    ${LINK_LIBRARIES}
    ${LOG4CPLUS_LIBRARIES}
  )
endif(LOG4CPLUS_FOUND)

include_directories(
  ${EMBDB_LAYOUT_DIR}/cpp
  ${PROTO_BUF_INCLUDE_DIRS}
  ${Libevent_INCLUDE_DIRS}
)

add_library(${APP_LIB_NAME} ${APP_SOURCES})

# Compile the application
add_executable(${APP_NAME} main.cpp)
target_link_libraries(${APP_NAME}
  ${LINK_LIBRARIES}
)
install(TARGETS ${APP_NAME} DESTINATION bin/)
