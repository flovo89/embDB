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
int main(int argc, char* argv[]) {
  int retCode;
  // comet_lib::DaemonizerPosix daemonizer;

  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // Parse args
  if (!parseArgs(argc, argv, retCode)) return retCode;

  return retCode;
}
