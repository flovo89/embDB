import tcpsocket
import commands

from time import sleep


HOST = 'localhost'
PORT = 5678

tcpsock = tcpsocket.TcpSocket()
tcpsock.connect(HOST, PORT)
interpreter = commands.CommandInterpreter()

ROWNAME_BASE = 'testrow'

################ Helper functions #####################
def createTestrows(count, type, maxitems):
    i = 0
    while i < count:
        tcpsock.send(interpreter.createrow(ROWNAME_BASE + str(i), type, maxitems))
        print(tcpsock.receive())
        i = i + 1

def deleteRowsRange(startindex, count):
    i = startindex
    while i < startindex + count:
        tcpsock.send(interpreter.deleterow(ROWNAME_BASE + str(i)))
        print(tcpsock.receive())
        i = i + 1

def getRowCount():
    tcpsock.send(interpreter.getrowcount())
    print(tcpsock.receive())

################### Your tests ########################

createTestrows(1000, 'string', 1000)
getRowCount()
deleteRowsRange(0, 1000)
