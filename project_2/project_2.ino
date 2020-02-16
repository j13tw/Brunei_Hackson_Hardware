
int led_status=0;
char led = false;
bool sound = false;

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A0, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  digitalWrite(2, LOW);
}

void loop() {
  int lm35_temp_raw = analogRead(A0);
  int sound_raw = analogRead(A1);
  int button_raw = digitalRead(3);
  float lm35_temp = (lm35_temp_raw*0.48828125);
  if (sound_raw > 512) {
    led_status = !led_status;
    digitalWrite(2, led_status);
    sound = true;
  }
  else sound = false;
  Serial.print("{\"lm35_temp\":");
  Serial.print(lm35_temp);
  Serial.print(",\"sound\":");
  if (sound == true) Serial.print("true");
  else Serial.print("false");
  Serial.print(",\"led\":");
  if (led_status == 1) Serial.print("true");
  else Serial.print("false");
  Serial.print(",\"button\":");
  if (button_raw == 0) Serial.print("true");
  else Serial.print("false");
  Serial.println("}");
  delay(1000);
}
