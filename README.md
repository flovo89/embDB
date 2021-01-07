# embDB
Simple Time Series Database 

Due to its simplicity the main focus is on embedded linux devices. But it is not exclusively developed for embedded devices. The database scales perfectly and can be used on full-blown linux distros as well.

# Facts

## Database structure
The database has the following structure

| Hash X | Ringbuffer info | Type | Item A | Item B | ... | Item N |
|--------|-----------------|------|--------|--------|-----|--------|
|    :   |         :       |   :  |     :   |     :   |  :  |    :    |
| Hash Z | Ringbuffer info | Type | Item a | Item b | ... | Item n |

**Hash**: Built from the name-string

**Ringbuffer info**: Index, maxItems, overflow, curItem. 

**Type**: Datatype of items in row

**Item x**: One item, containing timestamp & value

**Each row has its own itemscount and overflows automatically if more than itemscount items are added.**

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
Each entry in the database has its timestamp which has millisecond resolution since epoch and is packed in an signed long

## Limitations
The entire database is stored using a protobuf serialization. When the embDB service is running, the entire database is kept in RAM. When the service is stopped, the latest db version from RAM is serialized and stored to the persistent location. Therefore a proper stop of the service is necessary. Proper in this case means by SIGTERM or SIGINT.

# Interface

## Access
The database can be accessed by multible clients over a Unix-Domain-Socket or a TCP-Socket. More infos how to set the path or port are given in the help of the embDB service when calling 
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
Currently only a JSON protocol is implemented. Other protocols can be added using the implemented interfaces.
### JSON
#### CreateRow
Creates a new row in the database. A row with an unique name must be created to add items to it. The "itemsCount" field defines how many items are stored in this row. If as in the example below 1000 items are stored and the 1001 is added, the first added will be overwritten (ringbuffer like).
#### Request:
```JSON
{ 
    "command": "createRow", 
    "name": "yourRowName", 
    "type": "yourRowType", 
    "itemsCount": 1000 
}
```
#### Result:
```JSON
{ 
    "command": "createRow", 
    "name": "yourRowName", 
    "error": 0
}
```

### ClearAll
Clears all rows with their items.
#### Request:
```JSON
{ 
    "command": "clearAll" 
}
```
#### Result:
```JSON
{ 
    "command": "clearAll", 
    "error": 0
}
```
### DeleteRow
Deletes the row with the specified name.
#### Request:
```JSON
{ 
    "command": "deleteRow", 
    "name": "yourRowName" 
}
```
#### Result:
```JSON
{ 
    "command": "deleteRow", 
    "name": "yourRowName", 
    "error": 0 
}
```
### RowExists
Returns info if the row exists.
##### Request:
```JSON
{ 
    "command": "rowExists", 
    "name": "yourRowName" 
}
```
#### Result:
```JSON
{ 
    "command": "rowExists", 
    "name": "yourRowName", 
    "error": 0, 
    "rowExists": true 
    }
```
### RowCount
Number of all rows in database.
#### Request:
```JSON
{ 
    "command": "rowCount" 
}
```
#### Result:
```JSON
{ 
    "command": "rowCount", 
    "error": 0, 
    "rowCount": 2 
}
```
### ReadRow
Read all items in a row. 
#### Request:
```JSON
{ 
    "command": "readRow", 
    "name": "yourRowName", 
    "type": "yourRowType" 
}
```
#### Result:
```JSON
{ 
    "command": "readRow", 
    "error": 0, 
    "type": "yourRowType", 
    "values": [{"value": 89, "timestamp": 1234567891},{}] 
}
```
### ReadRowTimestamped
Returns all items between the given start & end time.
#### Request:
```JSON
{ 
    "command": "readRowTimestamped", 
    "name": "yourRowName", 
    "type": "yourRowType", 
    "startTime": 1234567890, 
    "endTime": 1234567899 
}
```
#### Result:
```JSON
{ 
    "command": "readRowTimestamped", 
    "error": 0, 
    "rowCount": 10, 
    "startTime": 1234567890, 
    "endTime": 1234567899, 
    "type": "yourRowType", 
    "values": [{"value": 1, "timestamp":1234567897},{}] 
}
```
### AddItem
Adds an item to the given row in the appropriate type.
#### Request:
"data" in appropriate type
```JSON
{ 
    "command": "writeItem", 
    "name": "yourRowName", 
    "type": "yourRowType", 
    "data": 1234
}
```
#### Result:
```JSON
{ 
    "command": "writeItem", 
    "name": "yourRowName", 
    "error": 0
}
```

## Errorcodes
```
- 0: SUCCESS

// Database errors
- 10: NOTFOUND              --> Requested row not found
- 11: INTERNAL              --> Internal error
- 12: HASHNAMEMISMATCH      --> Rowname hash mismatch
- 13: ITEMSCOUNTMISMATCH    --> Already existing row with other itemscount
- 14: TYPEMISMATCH          --> Type does not match for this row

// Protocol errors
- 50: NO_COMMAND            --> No command given
- 51: UNKNOWN_COMMAND       --> Command not known
- 52: NO_NAME               --> No rowname given
- 53: NO_TYPE               --> No type given
- 54: UNKNOWN_TYPE          --> Type not known
- 55: NO_STARTTIME          --> No starttime given
- 56: NO_ENDTIME            --> No endtime given
- 57: NO_ITEMSCOUNT         --> No itemscount given
- 58: NO_DATA               --> No data given
- 59: INVALID_DATA          --> Invalid data given
```

# Setup
..Not complete yet
## Protobuf
### Library
[Github link](https://github.com/protocolbuffers/protobuf/blob/master/src/README.md) Installed version 3.14.0

### Compiler
[Github link](http://google.github.io/proto-lens/installing-protoc.html)
Installed version 3.14.0

### After installation
```bash
sudo chown -R $(whoami).$(whoami) /usr/local/include/google
```

## Compile
```bash
mkdir build
cd build
cmake ..
make
sudo make install
```
Without automatic unit-test execution:
```bash
mkdir build
cd build
cmake .. _DCOMPILE_GTESTS=OFF
make
sudo make install
```
