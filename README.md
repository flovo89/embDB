# embDB
Database for embedded devices

## Setup
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
