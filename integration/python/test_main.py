import tcpsocket
import commands
import errorcheck

from time import sleep


HOST = 'localhost'
PORT = 5678

tcpsock = tcpsocket.TcpSocket()
tcpsock.connect(HOST, PORT)
interpreter = commands.CommandInterpreter()
errorcheck = errorcheck.ErrorCheck()

ROWNAME_BASE = 'testrow'

################ Helper functions #####################


def createTestrows(count, type, maxitems, experror):
    i = 0
    while i < count:
        tcpsock.send(interpreter.createrow(
            ROWNAME_BASE + str(i), type, maxitems))
        obj = tcpsock.receive()
        interpreter.printcreaterowanswer(obj)
        errorcheck.checkerrorcode(experror, obj)
        i = i + 1


def deleteRowsRange(startindex, count, experror):
    i = startindex
    while i < startindex + count:
        tcpsock.send(interpreter.deleterow(ROWNAME_BASE + str(i)))
        obj = tcpsock.receive()
        interpreter.printdeleterowanswer(obj)
        errorcheck.checkerrorcode(experror, obj)
        i = i + 1


def getRowCount(experror, expcount):
    tcpsock.send(interpreter.getrowcount())
    obj = tcpsock.receive()
    interpreter.printrowcountanswer(obj)
    errorcheck.checkerrorcode(experror, obj)
    errorcheck.checkrowcount(expcount, obj)


def clearAll(experror):
    tcpsock.send(interpreter.clearall())
    obj = tcpsock.receive()
    interpreter.printclearallanswer(obj)
    errorcheck.checkerrorcode(experror, obj)


def readRow(index, type, experror, expstart, expincrement):
    tcpsock.send(interpreter.readrow(ROWNAME_BASE + str(index), type))
    obj = tcpsock.receive()
    interpreter.printreadrowanswer(obj)
    errorcheck.checkerrorcode(experror, obj)
    errorcheck.checknumbers(expstart, expincrement, obj)


def addItemsNumberIncreasing(index, type, increment, start, count, experror):
    i = 0
    while i < count:
        tcpsock.send(interpreter.additem(ROWNAME_BASE +
                                         str(index), type, start+(i*increment)))
        obj = tcpsock.receive()
        interpreter.printadditemanswer(obj)
        errorcheck.checkerrorcode(experror, obj)
        i = i + 1

################### Your tests ########################


# Start with empty database
clearAll(0)

# Create and delete rows
createTestrows(1000, 'string', 1000, 0)
createTestrows(1000, 'uint64', 1000, 14)
getRowCount(0, 1000)
deleteRowsRange(0, 1000, 0)
createTestrows(5, 'bool', 50000, 0)
getRowCount(0, 5)
clearAll(0)
getRowCount(0, 0)

# Add elements uint32
createTestrows(100, 'uint32', 20, 0)
addItemsNumberIncreasing(2, 'uint32', 1, 0, 30, 0)
addItemsNumberIncreasing(20, 'uint32', 1, 1200, 10, 0)
addItemsNumberIncreasing(99, 'uint32', 1, 500, 100, 0)
addItemsNumberIncreasing(100, 'uint32', 1, 0, 100, 10)
readRow(0, 'uint32', 0, 0, 0)
readRow(2, 'uint32', 0, 10, 1)
readRow(20, 'uint32', 0, 1200, 1)
readRow(99, 'uint32', 0, 580, 1)
readRow(100, 'uint32', 10, 0, 0)
clearAll(0)

# Add elements int32
createTestrows(100, 'int32', 20, 0)
addItemsNumberIncreasing(3, 'int32', 1, -30, 30, 0)
addItemsNumberIncreasing(21, 'int32', 1, -10, 10, 0)
addItemsNumberIncreasing(99, 'int32', 1, -50, 100, 0)
addItemsNumberIncreasing(101, 'int32', 1, 0, 100, 10)
readRow(0, 'int32', 0, 0, 0)
readRow(3, 'int32', 0, -20, 1)
readRow(21, 'int32', 0, -10, 1)
readRow(99, 'int32', 0, 30, 1)
readRow(101, 'int32', 10, 0, 0)
clearAll(0)

# Add elements uint64
createTestrows(100, 'uint64', 20, 0)
addItemsNumberIncreasing(2, 'uint64', 1, 0, 30, 0)
addItemsNumberIncreasing(20, 'uint64', 2, 1200, 10, 0)
addItemsNumberIncreasing(99, 'uint64', 1, 500, 100, 0)
addItemsNumberIncreasing(100, 'uint64', 1, 0, 100, 10)
readRow(0, 'uint64', 0, 0, 0)
readRow(2, 'uint64', 0, 10, 1)
readRow(20, 'uint64', 0, 1200, 2)
readRow(99, 'uint64', 0, 580, 1)
readRow(100, 'uint64', 10, 0, 0)
clearAll(0)

# Add elements int64
createTestrows(100, 'int64', 20, 0)
addItemsNumberIncreasing(3, 'int64', 1, -30, 30, 0)
addItemsNumberIncreasing(21, 'int64', 1, -10, 10, 0)
addItemsNumberIncreasing(99, 'int64', -10, 0, 100, 0)
addItemsNumberIncreasing(101, 'int64', 1, 0, 100, 10)
readRow(0, 'int64', 0, 0, 0)
readRow(3, 'int64', 0, -20, 1)
readRow(21, 'int64', 0, -10, 1)
readRow(99, 'int64', 0, -800, -10)
readRow(101, 'int64', 10, 0, 0)
clearAll(0)

# Add elements float
createTestrows(100, 'float', 20, 0)
addItemsNumberIncreasing(3, 'float', 0.1, -10, 30, 0)
addItemsNumberIncreasing(21, 'float', 1.0, -10.0, 10, 0)
addItemsNumberIncreasing(99, 'float', -10, 0.0, 100, 0)
addItemsNumberIncreasing(101, 'float', 1, 0.0, 100, 10)
readRow(0, 'float', 0, 0, 0)
readRow(3, 'float', 0, -9.0, 0.1)
readRow(21, 'float', 0, -10, 1.0)
readRow(99, 'float', 0, -800, -10)
readRow(101, 'float', 10, 0, 0)
clearAll(0)

# Add elements double
createTestrows(100, 'double', 20, 0)
addItemsNumberIncreasing(3, 'double', 0.1, -10, 30, 0)
addItemsNumberIncreasing(21, 'double', 1.0, -10.0, 10, 0)
addItemsNumberIncreasing(99, 'double', -10, 0.0, 100, 0)
addItemsNumberIncreasing(101, 'double', 1, 0.0, 100, 10)
readRow(0, 'double', 0, 0, 0)
readRow(3, 'double', 0, -9.0, 0.1)
readRow(21, 'double', 0, -10, 1.0)
readRow(99, 'double', 0, -800, -10)
readRow(101, 'double', 10, 0, 0)
clearAll(0)
