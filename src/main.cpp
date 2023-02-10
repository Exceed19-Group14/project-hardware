#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define LDR 32
#define SOIL 33
#define TEMP 13
#define BUZZER 12

OneWire oneWire(TEMP);
DallasTemperature sensors(&oneWire);

void setup() {
    Serial.begin(115200);
    sensors.begin(); 
    pinMode(BUZZER, OUTPUT);
}

void loop() {
    sensors.requestTemperatures();
    Serial.printf("Light : %d , Moiture : %d , Temp : %.2f °C\n", analogRead(LDR), analogRead(SOIL), sensors.getTempCByIndex(0));
    delay(500);
}