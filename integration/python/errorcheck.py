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
