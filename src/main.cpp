#include <Arduino.h>

#define analogPin 33

int val = 0;
void setup() {
Serial.begin(115299);
}
 
void loop() {
val = analogRead(analogPin);
Serial.print("val = "); 
Serial.println(val); 
delay(500);
}