#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

const char *SSID = "";
const char *PASSWORD = "";
const int WIFI_TIME_OUT = 2000;

AsyncWebServer server(80);

String processor(const String& var){
  Serial.println(var);

  return String();
}

void setup() {
  Serial.begin(9600);

  if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
  }

  Serial.println("Connecting to WiFi");
  Serial.println(SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  unsigned long startAttemp = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemp < WIFI_TIME_OUT) {
    Serial.print(".");
    delay(100);
  }

  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Failed to connect to WiFi!");
  } else {
    Serial.println();
    Serial.println("Connected!");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest  *request) {
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });

    server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) { 
      request->send(SPIFFS, "/styles.css", "text/css"); 
    });

    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) { 
      request->send(SPIFFS, "/script.js", "application/javascript"); 
    });

    server.begin();
  }
}

void loop() {

}