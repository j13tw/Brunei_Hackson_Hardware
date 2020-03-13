import configparser
import requests
import time
import serial
import json

# Load data from config.ini file
config = configparser.ConfigParser()
config.read('config.ini')

registJson = {
    "ip": config["LOCAL_DEVICE"]["IP"],
    "mac": config["LOCAL_DEVICE"]["MAC"]
}

controlJson = {"fan": 0}
preStatus = {"fan": 0}

gcpRegist = 0
referenceData = 0

while(gcpRegist == 0):
    try:
        # print(config["GCP"]["SERVER_PROTOCOL"] + "://" + config["GCP"]["SERVER_IP"] + ":" + config["GCP"]["SERVER_PORT"] + "/devices")
        r = requests.post(config["GCP"]["SERVER_PROTOCOL"] + "://" + config["GCP"]["SERVER_IP"] + ":" + config["GCP"]["SERVER_PORT"] + "/devices", json=registJson)
        print("GCP Device Regist Status:", r.json()["status"])
        gcpRegist = 1
    except:
        print("GCP Device Regist Error")
    time.sleep(5)

while(True):
    print(config["GCP"]["SERVER_PROTOCOL"] + "://" + config["GCP"]["SERVER_IP"] + ":" + config["GCP"]["SERVER_PORT"] + "/query/" + config["CLOUD_DEVICE"]["MAC"])
    r = requests.get(config["GCP"]["SERVER_PROTOCOL"] + "://" + config["GCP"]["SERVER_IP"] + ":" + config["GCP"]["SERVER_PORT"] + "/query/" + config["CLOUD_DEVICE"]["MAC"])
    print(r.text)
    for x in range(0, len(r.json())):
        if (r.json()[x].get('name') == "light"):
            referenceData = int(r.json()[x].get('value'))
    print(referenceData)
    
    if (referenceData > 550): 
        controlJson["fan"] = 1
    elif (referenceData < 100): 
        controlJson["fan"] = 0
    else: 
        controlJson["fan"] = -1
    
    print(str(json.dumps(controlJson)))
    
    portOpen = 0
    
    print("controlJson", controlJson)
    if (preStatus["fan"] != controlJson["fan"]):
        try:
            print("serial open")
            arduino = serial.Serial(config["LOCAL_DEVICE"]["SERIAL"])
            time.sleep(3)
            arduino.write(str(json.dumps(controlJson) + "\\n").encode('utf-8'))
            arduino.close()
            preStatus = {}
            preStatus["fan"] = controlJson["fan"]
            time.sleep(10)
        except:
            pass
    try:
        sendStatus = {}
        sendStatus["mac"] = config["LOCAL_DEVICE"]["MAC"]
        sendStatus["sensorData"] = preStatus
        print(json.dumps(sendStatus))
        r = requests.post(config["GCP"]["SERVER_PROTOCOL"] + "://" + config["GCP"]["SERVER_IP"] + ":" + config["GCP"]["SERVER_PORT"] + "/insert", json=sendStatus)
        print(r.text)
    except:
        pass
        
    time.sleep(0.5)