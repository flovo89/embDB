import json


class ErrorCheck:
    def checkerrorcode(self, code, obj):
        dict = json.loads(obj)
        if(dict["error"] != code):
            print(str(obj))
            print("\033[91m" + "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
            print("\033[91m" + "Command " + dict["command"])
            print("\033[91m" + "Expected error: " + str(code) +
                  " Received error: " + str(dict["error"]))
            print("\033[91m" + "Answer: " + str(obj))
            print("\033[91m" + "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
            exit()

    def checkrowcount(self, count, obj):
        dict = json.loads(obj)
        if(dict["rowCount"] != count):
            print("\033[91m" + "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
            print("\033[91m" + "Command " + dict["command"])
            print("\033[91m" + "Expected rowcount: " + str(count) +
                  " Received rowcount: " + str(dict["rowCount"]))
            print("\033[91m" + "Answer: " + str(obj))
            print("\033[91m" + "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
            exit()

    def checknumbers(self, start, increment, obj):
        dict = json.loads(obj)
        i = 0
        v = start
        if(dict["error"] == 0):
            while(i < len(dict["values"])):
                if(round(dict["values"][i]["value"], 2) != round(v, 2)):
                    print("\033[91m" + "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
                    print("\033[91m" + "Command " + dict["command"])
                    print("\033[91m" + "Expected value at index " + str(i) + ": " +
                          str(v) + " Received: " + str(dict["values"][i]["value"]))
                    print("\033[91m" + "Answer: " + str(obj))
                    print("\033[91m" + "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
                    exit()
                i = i + 1
                v = v + increment

    def checkbooltoggle(self, start, obj):
        dict = json.loads(obj)
        i = 0
        v = start
        if(dict["error"] == 0):
            while(i < len(dict["values"])):
                if(dict["values"][i]["value"] != v):
                    print("\033[91m" + "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
                    print("\033[91m" + "Command " + dict["command"])
                    print("\033[91m" + "Expected value at index " + str(i) + ": " +
                          str(v) + " Received: " + str(dict["values"][i]["value"]))
                    print("\033[91m" + "Answer: " + str(obj))
                    print("\033[91m" + "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
                    exit()
                i = i + 1
                v = not v


    def checkstrings(self, strval, start, increment, obj):
        dict = json.loads(obj)
        i = 0
        v = strval + str(start)
        if(dict["error"] == 0):
            while(i < len(dict["values"])):
                if(dict["values"][i]["value"] != v):
                    print("\033[91m" + "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
                    print("\033[91m" + "Command " + dict["command"])
                    print("\033[91m" + "Expected value at index " + str(i) + ": " +
                          v + " Received: " + dict["values"][i]["value"])
                    print("\033[91m" + "Answer: " + str(obj))
                    print("\033[91m" + "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
                    exit()
                i = i + 1
                v = strval + str(start+i*increment)

    def checksbytes(self, expstart, explength, obj):
        dict = json.loads(obj)
        i = 0
        v = [None] * explength
        while i < explength:
            v[i] = (expstart + i) % 256
            i = i + 1
        i = 0
        if(dict["error"] == 0):
            while(i < len(dict["values"])):
                if(dict["values"][i]["value"] != v):
                    print("\033[91m" + "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
                    print("\033[91m" + "Command " + dict["command"])
                    print("\033[91m" + "Expected value at index " + str(i) + ": " +
                          v + " Received: " + dict["values"][i]["value"])
                    print("\033[91m" + "Answer: " + str(obj))
                    print("\033[91m" + "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
                    exit()
                i = i + 1