import json

class CommandInterpreter:
    def createrow(self, name, type, itemscount):
        dict = {'command' : 'createRow', 'name' : name, 'type' : type, 'itemsCount' : itemscount}
        return json.dumps(dict)

    def clearall(self):
        dict = {'command' : 'clearAll'}
        return json.dumps(dict)

    def readrow(self, name, type):
        dict = {'command' : 'readRow', 'name' : name, 'type' : type}
        return json.dumps(dict)

    def readrowtimestamped(self, name, type, start, end):
        dict = {'command' : 'readRowTimestamped', 'name' : name, 'type' : type, 'startTime' : start, 'endTime' : end}
        return json.dumps(dict)

    def deleterow(self, name):
        dict = {'command' : 'deleteRow', 'name' : name}
        return json.dumps(dict)

    def checkrow(self, name):
        dict = {'command' : 'rowExists', 'name' : name}
        return json.dumps(dict)

    def getrowcount(self):
        dict = {'command' : 'rowCount'}
        return json.dumps(dict)

    def additem(self, name, type, data):
        dict = {'command' : 'writeItem', 'name' : name, 'type' : type, 'data' : data}
        return json.dumps(dict)


    