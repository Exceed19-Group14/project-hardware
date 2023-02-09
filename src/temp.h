#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define KY001_Signal_PIN 22

// Libraries are configured
OneWire oneWire(KY001_Signal_PIN);          
DallasTemperature sensors(&oneWire);    


void setup() {

    // Initialize serial output
    Serial.begin(115200);
    Serial.println("KY-001 temperature measurement");

    // Sensor is initialized
    sensors.begin();  
}

//main program loop
void loop()
{
    // Temperature measurement is started...
    sensors.requestTemperatures();
    // ... and output measured temperature
    Serial.print("Temperature: ");
    Serial.print(sensors.getTempCByIndex(0));
    Serial.println(" °C");

    delay(500); // 1s pause until next measurement
}