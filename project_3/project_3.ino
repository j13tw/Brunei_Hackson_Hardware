#include <ArduinoJson.h>

const int controlPin0 = 12;
const int controlPin1 = 11;
int controlStatus = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(controlPin0, OUTPUT);
  pinMode(controlPin1, OUTPUT);
  digitalWrite(controlPin0, 1);
  digitalWrite(controlPin1, 1);
}

void loop() {
  String data;
  int count = 0;

  StaticJsonDocument<200> sendJson;

  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    data = Serial.readString();
    count = data.length();
    char json[count];
    data.toCharArray(json, count);
//    Serial.println(json);
    StaticJsonDocument<200> jsonBuffer;
    //  char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
    DeserializationError error = deserializeJson(jsonBuffer, json);

    // Test if parsing succeeds.
    if (error) {
//      Serial.print(F("deserializeJson() failed: "));
//      Serial.println(error.c_str());
      return;
    }
    if (jsonBuffer["fan"].is<int>() and (jsonBuffer["fan"] == -1 or jsonBuffer["fan"] == 0 or jsonBuffer["fan"] == 1)) {
      int receiveStatus = jsonBuffer["fan"];
//      Serial.print("receiveStatus:");
//      Serial.println(receiveStatus);
      if (receiveStatus != controlStatus) {
//        Serial.println("change status");
        if (controlStatus > 0) {
//          Serial.println("正轉關閉");
          digitalWrite(controlPin0, 1);
        }
        else if (controlStatus < 0) {
//          Serial.println("反轉關閉");
          digitalWrite(controlPin1, 1);
        }
        if (controlStatus != 0 and receiveStatus != 0){
//          Serial.println("正反狀態修改延遲");
          delay(10000); 
        }
        if (receiveStatus > 0) {
//          Serial.println("正轉開啟");
          digitalWrite(controlPin0, 0);
        }
        else if (receiveStatus < 0)  {
//          Serial.println("反轉開啟");
          digitalWrite(controlPin1, 0);
        }
        controlStatus = receiveStatus;
      }
    }
  }
//  sendJson["fan"] = controlStatus;
//  serializeJson(sendJson, Serial);
//  Serial.println();
}
