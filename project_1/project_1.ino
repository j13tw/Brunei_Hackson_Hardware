#include <DHT.h>

#define DHTPIN 2     
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(A0, INPUT);
}

void loop() {
  float light = analogRead(A0);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT11 sensor!"));
    return;
  }
  Serial.print("{\"humidity\": ");
  Serial.print(h);
  Serial.print(", \"temperature\": ");
  Serial.print(t);
  Serial.print(", \"light\": ");
  Serial.print(light);
  Serial.println("}");
  delay(1000);
}
