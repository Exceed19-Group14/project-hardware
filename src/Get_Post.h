#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

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