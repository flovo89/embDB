import json
import datetime


class CommandInterpreter:
    def createrow(self, name, type, itemscount):
        dict = {'command': 'createCircularRow', 'name': name,
                'type': type, 'itemsCount': itemscount}
        return json.dumps(dict)

    def printcreaterowanswer(self, obj):
        dict = json.loads(obj)
        print("================================")
        print("createCircularRow")
        if(dict["error"] == 0):
            print("name: " + dict["name"])
        print("error: " + str(dict["error"]))
        print("================================")

    def clearall(self):
        dict = {'command': 'clearAllCircular'}
        return json.dumps(dict)

    def printclearallanswer(self, obj):
        dict = json.loads(obj)
        print("================================")
        print("clearAllCircular")
        print("error: " + str(dict["error"]))
        print("================================")

    def readrow(self, name, type):
        dict = {'command': 'readCircularRow', 'name': name, 'type': type}
        return json.dumps(dict)

    def printreadrowanswer(self, obj):
        dict = json.loads(obj)
        print("================================")
        print("readCircularRow")
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
        dict = {'command': 'readCircularRowTimestamped', 'name': name,
                'type': type, 'startTime': start, 'endTime': end}
        return json.dumps(dict)

    def deleterow(self, name):
        dict = {'command': 'deleteCircularRow', 'name': name}
        return json.dumps(dict)

    def printdeleterowanswer(self, obj):
        dict = json.loads(obj)
        print("================================")
        print("deleteCircularRow")
        if(dict["error"] == 0):
            print("name: " + dict["name"])
        print("error: " + str(dict["error"]))
        print("================================")

    def checkrow(self, name):
        dict = {'command': 'circularRowExists', 'name': name}
        return json.dumps(dict)

    def getrowcount(self):
        dict = {'command': 'circularRowCount'}
        return json.dumps(dict)

    def printrowcountanswer(self, obj):
        dict = json.loads(obj)
        print("================================")
        print("circularRowCount")
        if(dict["error"] == 0):
            print("count: " + str(dict["circularRowCount"]))
        print("error: " + str(dict["error"]))
        print("================================")

    def additem(self, name, type, data):
        dict = {'command': 'writeItemCircular',
                'name': name, 'type': type, 'data': data}
        return json.dumps(dict)

    def printadditemanswer(self, obj):
        dict = json.loads(obj)
        print("================================")
        print("writeItemCircular")
        if(dict["error"] == 0):
            print("name: " + dict["name"])
        print("error: " + str(dict["error"]))
        print("================================")
