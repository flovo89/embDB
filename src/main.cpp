/*
 * embDB
 * Copyright (C) 2021 flovo89
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <getopt.h>
#include <google/protobuf/stubs/common.h>
#include <signal.h>
#include <sys/stat.h>

#include <iostream>
#include <sstream>

#include "database/db-guard/DbGuard.hpp"
#include "database/db-layout/DbLayout.hpp"
#include "eventloop/EventLoopBreakerOnSignal.hpp"
#include "eventloop/EventLoopFactory.hpp"
#include "file-io/FileReader.hpp"
#include "file-io/FileWriter.hpp"
#include "logging/Loggers.hpp"
#include "process/posix/DaemonizerPosix.hpp"
#include "protocol/ProtocolArbiter.hpp"
#include "protocol/ProtocolProcessor.hpp"
#include "protocol/json/JsonProtocol.hpp"
#include "server/sockets/SocketClient.hpp"
#include "server/sockets/SocketClientFactory.hpp"
#include "server/sockets/SocketServer.hpp"
#include "socket/posix/SocketServer.hpp"
#include "utilities/DefaultHasher.hpp"
#include "utilities/DefaultMutex.hpp"
#include "utilities/DefaultTimestamper.hpp"
#include "version.hpp"

/* Constants */
static const std::string DEFAULT_LOG_FILE = "/etc/embdb/logging.ini";
static const std::string DEFAULT_PID_FILE = "/var/run/embdb.pid";
static const std::string DEFAULT_SOCKET_PATH = "/var/run/embdb.sock";
static const std::string DATABASE_FILE_PATH = "/var/data/sd/database/embdb";
static const int DEFAULT_TCP_PORT = 8085;

/* Locals */
std::string logConfig = DEFAULT_LOG_FILE;
std::string pidFile = DEFAULT_PID_FILE;
std::string socketPath = DEFAULT_SOCKET_PATH;
std::string databasePath = DATABASE_FILE_PATH;
bool isDaemonize = false;
int tcpport = DEFAULT_TCP_PORT;

std::list<std::shared_ptr<embDB_protocol::IProtocol>> protocols;

//--------------------------------------------------------------------------------------------
void usage(const char* progname) {
  std::cout << progname << ":" << std::endl;
  std::cout << " -h | --help         Show this help" << std::endl
            << " -v | --version      Print the version" << std::endl
            << " -l | --logging      Logging file (" << DEFAULT_LOG_FILE << ")"
            << std::endl
            << " -d | --daemonize    Run as daemon" << std::endl
            << " -p | --pidfile      PID file (" << DEFAULT_PID_FILE << ")"
            << std::endl
            << " -s | --socket       Path to socket (" << DEFAULT_SOCKET_PATH
            << ")" << std::endl
            << " -f | --file         Path to database file ("
            << DATABASE_FILE_PATH << ")" << std::endl
            << " -t | --tcpport      Tcp socket port (" << DEFAULT_TCP_PORT
            << ")" << std::endl;
}

//--------------------------------------------------------------------------------------------
std::string buildStringVersion() {
  std::stringstream ss;

  ss << embDB::VERSION_MAJOR << "." << embDB::VERSION_MINOR << "."
     << embDB::VERSION_BUGFIX << " (" << embDB::GIT_HASH << " / "
     << embDB::VERSION_BUILD_TIME << ")";

  return ss.str();
}

//--------------------------------------------------------------------------------------------
void printVersion() {
  std::cout << "(C) 2021 flovo89" << std::endl << std::endl;
  std::cout << "embdb : " << buildStringVersion() << std::endl;
}

//--------------------------------------------------------------------------------------------
bool parseArgs(int argc, char* argv[], int& retCode) {
  static struct option long_options[] = {
      {"help", no_argument, NULL, 'h'},
      {"version", no_argument, NULL, 'v'},
      {"logging", required_argument, NULL, 'l'},
      {"daemonize", no_argument, NULL, 'd'},
      {"pidfile", required_argument, NULL, 'p'},
      {"socket", required_argument, NULL, 's'},
      {"file", required_argument, NULL, 'f'},
      {"tcpport", required_argument, NULL, 't'},
      {0, 0, 0, 0}};
  int option_index = 0;

  while (1) {
    int c =
        getopt_long(argc, argv, "hvl:dp:s:f:t:", long_options, &option_index);

    if (c == -1) {
      break;
    }

    switch (c) {
      case 'h':
        usage(argv[0]);
        retCode = 0;
        return false;
      case 'v':
        printVersion();
        retCode = 0;
        return false;
      case 'l':
        logConfig = optarg;
        break;
      case 'd':
        isDaemonize = true;
        break;
      case 'p':
        pidFile = optarg;
        break;
      case 's':
        socketPath = optarg;
        break;
      case 'f':
        databasePath = optarg;
        break;
      case 't':
        tcpport = atoi(optarg);
        break;
      default:
        usage(argv[0]);
        retCode = -1;
        return false;
    }
  }

  return true;
}

//--------------------------------------------------------------------------------------------
void buildDatabaseGuard(std::unique_ptr<embDB_database::DbGuard>& guard) {
  std::unique_ptr<embDB_fileio::FileReader> filereader(
      new embDB_fileio::FileReader(databasePath));
  std::unique_ptr<embDB_fileio::FileWriter> filewriter(
      new embDB_fileio::FileWriter(databasePath));
  std::unique_ptr<embDB_utilities::IHasher> hasher(
      new embDB_utilities::DefaultHasher());
  std::unique_ptr<embDB_utilities::ITimestamper> timestamper(
      new embDB_utilities::DefaultTimestamper());

  std::unique_ptr<embDB_database::IDataBase> layout(
      new embDB_database::DbLayout(std::move(filereader), std::move(filewriter),
                                   std::move(hasher), std::move(timestamper)));

  std::unique_ptr<embDB_utilities::IMutex> mutex(
      new embDB_utilities::DefaultMutex());
  guard.reset(new embDB_database::DbGuard(std::move(layout), std::move(mutex)));
}

//--------------------------------------------------------------------------------------------
int deserializeDatabase(std::unique_ptr<embDB_database::DbGuard>& guard) {
  if (guard->deserialize() != embDB_database::DbErrorCode::SUCCESS) {
    LOG_WA() << "Database could not be deserialized, so clear it all!";
    if (guard->clearAll() != embDB_database::DbErrorCode::SUCCESS) {
      LOG_ER() << "Database could not be cleared, FATAL";
      return -1;
    }
  }

  return 0;
}

//--------------------------------------------------------------------------------------------
void buildProtocolProcessor(
    embDB_database::DbGuard& guard,
    std::unique_ptr<embDB_protocol::ProtocolProcessor>& processor) {
  std::unique_ptr<embDB_protocol::IProtocol> jsonprotocol(
      new embDB_protocol::JsonProtocol());

  protocols.push_back(std::move(jsonprotocol));
  std::unique_ptr<embDB_protocol::IProtocolArbiter> arbiter(
      new embDB_protocol::ProtocolArbiter(protocols));

  processor.reset(
      new embDB_protocol::ProtocolProcessor(std::move(arbiter), guard));
}

//--------------------------------------------------------------------------------------------
void buildUnixSocketServer(
    embDB_eventloop::IEventLoop& eventloop,
    embDB_server::IClientReception& receiver,
    std::unique_ptr<embDB_server::ISocketServer>& server) {
  std::unique_ptr<embDB_socket::ISocketServer> socketServerUnix(
      new embDB_socket::SocketServer(socketPath));
  socketServerUnix->setAccessMode(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
  std::unique_ptr<embDB_socket::ISocketServer> socketServer(
      std::move(socketServerUnix));
  std::unique_ptr<embDB_server::ISocketClientFactory> clientFactory(
      new embDB_server::SocketClientFactory());
  server.reset(new embDB_server::SocketServer(
      eventloop, std::move(socketServer), std::move(clientFactory), receiver));
}

//--------------------------------------------------------------------------------------------
void buildTcpSocketServer(
    embDB_eventloop::IEventLoop& eventloop,
    embDB_server::IClientReception& receiver,
    std::unique_ptr<embDB_server::ISocketServer>& server) {
  std::unique_ptr<embDB_socket::ISocketServer> socketServerTcp(
      new embDB_socket::SocketServer(tcpport));
  std::unique_ptr<embDB_socket::ISocketServer> socketServer(
      std::move(socketServerTcp));
  std::unique_ptr<embDB_server::ISocketClientFactory> clientFactory(
      new embDB_server::SocketClientFactory());
  server.reset(new embDB_server::SocketServer(
      eventloop, std::move(socketServer), std::move(clientFactory), receiver));
}

//--------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
  int retCode;
  embDB_process::DaemonizerPosix daemonizer;

  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // Parse args
  if (!parseArgs(argc, argv, retCode)) return retCode;

  // Initialize logging
  if (embDB_logging::Loggers::getInstance().initDefault(logConfig)) return -1;

  // Initialize the event loop
  std::unique_ptr<embDB_eventloop::IEventLoop> eventLoop;
  if (embDB_eventloop::EventLoopFactory::createDefaultEventLoop(eventLoop))
    return -1;
  if (eventLoop->init()) return -1;

  // Build database
  std::unique_ptr<embDB_database::DbGuard> guard;
  buildDatabaseGuard(guard);

  // Init database
  if (deserializeDatabase(guard)) return -1;

  // Build protocolprocessor
  std::unique_ptr<embDB_protocol::ProtocolProcessor> processor;
  buildProtocolProcessor(*guard, processor);

  // Build socket servers
  std::unique_ptr<embDB_server::ISocketServer> unixsocketserver;
  buildUnixSocketServer(*eventLoop, *processor, unixsocketserver);
  std::unique_ptr<embDB_server::ISocketServer> tcpsocketserver;
  buildTcpSocketServer(*eventLoop, *processor, tcpsocketserver);

  // Init socket server
  if (unixsocketserver->init()) return -1;
  if (tcpsocketserver->init()) return -1;

  // Quit the app gracefully on signal
  embDB_eventloop::EventLoopBreakerOnSignal elBreaker(*eventLoop);
  eventLoop->registerHandledSignal(elBreaker, SIGTERM);
  eventLoop->registerHandledSignal(elBreaker, SIGINT);

  if (isDaemonize) {
    int daemonizeStatus;
    daemonizer.setPidFile(pidFile);
    daemonizeStatus = daemonizer.daemonize();

    if (daemonizeStatus < 0)
      return -1;  // Error
    else if (daemonizeStatus > 0)
      return 0;  // Parent

    if (eventLoop->reInit()) {
      LOG_ER() << "Cannot reinit event loop!";
      return -1;
    }
  }

  LOG_IN() << "Starting embDB main loop";
  retCode = eventLoop->run();

  // Cleanup
  unixsocketserver->close();
  tcpsocketserver->close();
  guard->serialize();
  google::protobuf::ShutdownProtobufLibrary();
  if (isDaemonize) daemonizer.removePidFile();

  LOG_IN() << "Exiting embDB with return code " << retCode;

  return retCode;
}
