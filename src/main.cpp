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
#define red_LED 25
#define green_LED 26

TaskHandle_t TaskA = NULL;
TaskHandle_t TaskB = NULL;

OneWire oneWire(TEMP);
DallasTemperature sensors(&oneWire);

int StopSong = 0;
int ClickToStart = 0;

const String baseUrl = "http://group14.exceed19.online/";

const char *ssid = "group15";
const char *password = "thisisapassword";

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

int On_Off_Auto = 0;
long Dulation = 1000;

void GET_post(void *param){
    while(true){
    DynamicJsonDocument doc(2048);
    const String url = baseUrl + "plant/1/water";
    HTTPClient http;
    http.begin(url);
    int httpResponseCode = http.GET();
    if (httpResponseCode >= 200 && httpResponseCode < 400) {
        Serial.print("GET!!");
        String payload = http.getString();
        deserializeJson(doc, payload);
        ClickToStart = doc["water_status"].as<int>();
        Dulation = doc["duration"].as<int>();
        Serial.printf("Status : %d, Dulation : %d\n", ClickToStart, Dulation);
        if (ClickToStart == 1){
            vTaskDelay(Dulation/portTICK_PERIOD_MS);
        }
        Serial.println("=======================");
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    vTaskDelay(5000/portTICK_PERIOD_MS);
    }
}

int light = 0;
int moisture = 0;
float temp = 0.0;

void PATCH_data(void *param){
    while(true){
    vTaskDelay(20000/portTICK_PERIOD_MS);
    String json;
    DynamicJsonDocument doc(2048);
    doc["light"] = light;
    doc["moisture"] = moisture;
    doc["temperature"] = temp;
    serializeJson(doc,json);

    const String url = baseUrl + "plant/1";
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type","application/json");

    int httpResponseCode = http.PATCH(json);
    if (httpResponseCode >= 200 && httpResponseCode < 300) {
        Serial.print("PATCH COMPLETE !!");
        Serial.printf("Light : %d , Moiture : %d , Temp : %.2f°C\n", light, moisture, temp);
        Serial.println("=======================");
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
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

    if (moisture < 300){
        ledcWrite(0, 200);
        ledcWrite(1, 0);
    }
    if (moisture >= 300){
        ledcWrite(0, 0);
        ledcWrite(1, 70);
    }
    delay(2000);
}

void Start(void *param){
    while(true){
    Measure();
    if (On_Off_Auto == 1 && moisture < 300){
        Serial.println("Initiated!! Auto Mode.");
        Serial.printf("Dulation = %d Secound.\n", Dulation/1000);
        PlaySong();
        Serial.println("Done.");
        Serial.println("=======================");
    }
    if (On_Off_Auto == 0 && ClickToStart == 1){
        Serial.println("Initiated!! Start AquaBot!!");
        Serial.printf("Dulation = %d Secound.\n", Dulation/1000);
        PlaySong();
        Serial.println("Done.");
        Serial.println("=======================");
        ClickToStart = 0;
    }
    }
}

void setup() {
    Serial.begin(115200);
    sensors.begin(); 
    pinMode(BUZZER, OUTPUT);
    ledcSetup(0, 5000, 8);
    ledcAttachPin(red_LED, 0);
    ledcSetup(1, 5000, 8);
    ledcAttachPin(green_LED, 1);

    Connect_Wifi();
    
    xTaskCreatePinnedToCore(Start, "StartProgram", 10000, NULL, 1, &TaskA, 0);
    xTaskCreatePinnedToCore(GET_post, "GET_post", 10000, NULL, 1, &TaskA, 1);
    xTaskCreatePinnedToCore(PATCH_data, "PATCH_data", 10000, NULL, 1, &TaskA, 1);
}

void loop() {

}