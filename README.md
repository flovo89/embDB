# embDB
Simple Time Series Database

# Facts

## Datatypes
Supported datatypes are:
- string
- unsigned integer
- signed integer
- unsinged long
- signed long
- float
- double
- bool
- bytes vector

## Timestamps
Each entry in the database has its timestamp which has millisecond resulution and is packet in an signed long

# Interface

## Access
The database can be accessed by multible clients over a Unix-Domain-Socket or a TCP-Socket. More infos are given in the help of the embDB service when calling 
```BASH
root@machine embdb -h
embdb:
 -h | --help         Show this help
 -v | --version      Print the version
 -l | --logging      Logging file (/etc/embdb/logging.ini)
 -d | --daemonize    Run as daemon
 -p | --pidfile      PID file (/var/run/embdb.pid)
 -s | --socket       Path to socket (/var/run/embdb.sock)
 -f | --file         Path to database file (/var/data/sd/database/embdb)
 -t | --tcpport      Tcp socket port (8085)

```

## Protocol
Currently only a JSON protocol is implemented. Other protocols can be implemented on request.
### JSON
#### CreateRow
Request:
```JSON
{ 
    "command": "createRow", 
    "name": "yourRowName", 
    "type": "yourRowType", 
    "itemsCount": maxItemsInRow 
}
```
Result:
```JSON
{ 
    "command": "createRow", 
    "name": "yourRowName", 
    "error": errorcode
}
```

### ClearAll
Request:
```JSON
{ 
    "command": "clearAll" 
}
```
Result:
```JSON
{ 
    "command": "clearAll", 
    "error": errorcode
}
```
### DeleteRow
Request:
```JSON
{ 
    "command": "deleteRow", 
    "name": "yourRowName" 
}
```
Result:
```JSON
{ 
    "command": "deleteRow", 
    "name": "yourRowName", 
    "error": errorcode 
}
```
### RowExists
Request:
```JSON
{ 
    "command": "rowExists", 
    "name": "yourRowName" 
}
```
Result:
```JSON
{ 
    "command": "rowExists", 
    "name": "yourRowName", 
    "error": errorcode, 
    "rowExists": bool 
    }
```
### RowCount
Request:
```JSON
{ 
    "command": "rowCount" 
}
```
Result:
```JSON
{ 
    "command": "rowCount", 
    "error": errorcode, 
    "rowCount": count 
}
```
### ReadRow
Request:
```JSON
{ 
    "command": "readRow", 
    "name": "yourRowName", 
    "type": "yourRowType" 
}
```
Result:
```JSON
{ 
    "command": "readRow", 
    "error": errorcode, 
    "rowCount": count, 
    "type": "yourRowType", 
    "values": [{"value": v, "timestamp":ts},{},...] 
}
```
### ReadRowTimestamped
Request:
```JSON
{ 
    "command": "readRowTimestamped", 
    "name": "yourRowName", 
    "type": "yourRowType", 
    "startTime": ms_start, 
    "endTime": ms_end 
}
```
Result:
```JSON
{ 
    "command": "readRowTimestamped", 
    "error": errorcode, 
    "rowCount": count, 
    "startTime": ms_start, 
    "endTime": ms_end, 
    "type": "yourRowType", 
    "values": [{"value": v, "timestamp":ts},{},...] 
}
```
### AddItem
Request:
```JSON
{ 
    "command": "writeItem", 
    "name": "yourRowName", 
    "type": "yourRowType", 
    "data": yourDataInAppropriateType
}
```
Result:
```JSON
{ 
    "command": "writeItem", 
    "name": "yourRowName", 
    "error": errorcode 
}
```

## Setup
..Not complete yet
### Protobuf
#### Library
[Github link](https://github.com/protocolbuffers/protobuf/blob/master/src/README.md) Installed version 3.14.0

#### compiler
[Github link](http://google.github.io/proto-lens/installing-protoc.html)
Installed version 3.14.0

#### After installation
```bash
sudo chown -R $(whoami).$(whoami) /usr/local/include/google
```

### Compile
```bash
mkdir build
cd build
cmake ..
make
```
Without automatic unit-test execution:
```bash
mkdir build
cd build
cmake .. _DCOMPILE_GTESTS=OFF
make
```
