import json
import datetime


class CommandInterpreter:
    def createrow(self, name, type, itemscount):
        dict = {'command': 'createRow', 'name': name,
                'type': type, 'itemsCount': itemscount}
        return json.dumps(dict)

    def printcreaterowanswer(self, obj):
        dict = json.loads(obj)
        print("================================")
        print("createRow")
        if(dict["error"] == 0):
            print("name: " + dict["name"])
        print("error: " + str(dict["error"]))
        print("================================")

    def clearall(self):
        dict = {'command': 'clearAll'}
        return json.dumps(dict)

    def printclearallanswer(self, obj):
        dict = json.loads(obj)
        print("================================")
        print("clearAll")
        print("error: " + str(dict["error"]))
        print("================================")

    def readrow(self, name, type):
        dict = {'command': 'readRow', 'name': name, 'type': type}
        return json.dumps(dict)

    def printreadrowanswer(self, obj):
        dict = json.loads(obj)
        print("================================")
        print("readRow")
        if(dict["error"] == 0):
            print("name: " + dict["name"])
            print("type: " + dict["type"])
            i = 0
            while(i < len(dict["values"])):
                print("index " + str(i) + ": " + str(dict["values"][i]["value"]) + " || " + str(
                    datetime.datetime.fromtimestamp(dict["values"][i]["timestamp"]/1000.0)))
                i = i + 1
        print("error: " + str(dict["error"]))
        print("================================")

    def readrowtimestamped(self, name, type, start, end):
        dict = {'command': 'readRowTimestamped', 'name': name,
                'type': type, 'startTime': start, 'endTime': end}
        return json.dumps(dict)

    def deleterow(self, name):
        dict = {'command': 'deleteRow', 'name': name}
        return json.dumps(dict)

    def printdeleterowanswer(self, obj):
        dict = json.loads(obj)
        print("================================")
        print("deleteRow")
        if(dict["error"] == 0):
            print("name: " + dict["name"])
        print("error: " + str(dict["error"]))
        print("================================")

    def checkrow(self, name):
        dict = {'command': 'rowExists', 'name': name}
        return json.dumps(dict)

    def getrowcount(self):
        dict = {'command': 'rowCount'}
        return json.dumps(dict)

    def printrowcountanswer(self, obj):
        dict = json.loads(obj)
        print("================================")
        print("rowCount")
        if(dict["error"] == 0):
            print("count: " + str(dict["rowCount"]))
        print("error: " + str(dict["error"]))
        print("================================")

    def additem(self, name, type, data):
        dict = {'command': 'writeItem',
                'name': name, 'type': type, 'data': data}
        return json.dumps(dict)

    def printadditemanswer(self, obj):
        dict = json.loads(obj)
        print("================================")
        print("writeItem")
        if(dict["error"] == 0):
            print("name: " + dict["name"])
        print("error: " + str(dict["error"]))
        print("================================")
