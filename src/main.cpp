#include <Arduino.h>
#include <OneWire.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <DallasTemperature.h>
#include "melody.h"

#define LDR 32
#define SOIL 33
#define TEMP 13
#define BUZZER 12

OneWire oneWire(TEMP);
DallasTemperature sensors(&oneWire);

int StopSong = 0;

const String baseUrl = "WEB";

const char *ssid = "<Wifi name>";
const char *password = "<Wifi password>";

void Connect_Wifi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        }
    Serial.print("OK! IP=");
    Serial.println(WiFi.localIP());
}

int On_Off = 0;

void GET_post(){
    DynamicJsonDocument doc(2048);
    const String url = baseUrl + "...";
    HTTPClient http;
    http.begin(url);
    int httpResponseCode = http.GET();
    if (httpResponseCode >= 200 && httpResponseCode < 400) {
        Serial.print("HTTP ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        deserializeJson(doc, payload);
        On_Off = doc["id"].as<int>();
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
}

int light = 0;
int moisture = 0;
float temp = 0.0;
int id = 0;

void PUT_data(){
    String json;
    DynamicJsonDocument doc(2048);
    doc["Id"] = id;
    doc["light"] = light;
    doc["moisture"] = moisture;
    doc["temp"] = temp;
    serializeJson(doc,json);

    const String url = baseUrl + "...";
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type","application/json");

    int httpResponseCode = http.PUT(json);
    if (httpResponseCode >= 200 && httpResponseCode < 300) {
        Serial.print("HTTP ");
        Serial.println(httpResponseCode);
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
}

void HarrySong(){
    int size = sizeof(durations) / sizeof(int);

    for (int note = 0; note < size; note++) {
        int duration = 1000 / durations[note];
        tone(BUZZER, melody[note], duration);

        int pauseBetweenNotes = duration * 1.30;
        delay(pauseBetweenNotes);
    
        noTone(BUZZER);
        
        if (StopSong == 1)
        {
            break;
        }
     }
}

long Dulation = 1000;
void PlaySong(){
    long StartTime = millis();
    long EndTime = StartTime + Dulation;
    while(millis() < EndTime)
    {
		digitalWrite(BUZZER, HIGH);
        delay(1);
        digitalWrite(BUZZER, LOW);
        delay(1);
	}
    
}

void Measure(){
    sensors.requestTemperatures();
    light = analogRead(LDR);
    moisture = analogRead(SOIL);
    temp = sensors.getTempCByIndex(0);

    Serial.printf("Light : %d , Moiture : %d , Temp : %.2f °C\n", light, moisture, temp);
    delay(2000);
}

void Start(){
    if (On_Off == 1){
        Serial.println("=======================");
        Serial.println("Initiated!!");
        Serial.printf("Dulation = %d Secound.\n", Dulation/1000);
        Serial.println("=======================");
        PlaySong();
    }
}

void setup() {
    Serial.begin(115200);
    sensors.begin(); 
    pinMode(BUZZER, OUTPUT);

    On_Off = 1;
    Start();
}

void loop() {
    Measure();
}