#include <Arduino.h>
#define LDR 32
void setup() {
    Serial.begin(115200);
    Serial.println("LDR");
}

void loop() {
    Serial.println(analogRead(LDR));
    delay(500);
}