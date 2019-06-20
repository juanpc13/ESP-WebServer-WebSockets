#include <FS.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
const char *ssid = "TURBONETT_1DFD27";
const char *password = "57E04D255E";

bool relay = false;
#define pinRelay 2

void setup() {
  //Puerto Serial
  Serial.begin(115200);
  Serial.println();
  //Memoria SPIFFS
#ifdef ESP32
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    while (1);
  }
#elif defined(ESP8266)
  SPIFFS.begin();
#endif
  //Relay
  pinMode(pinRelay, OUTPUT);
  digitalWrite(pinRelay, LOW);
  //Conectar Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  //Mostrar IP
  Serial.print("IP : ");
  Serial.println(WiFi.localIP());
  //Server Config
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest * request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    response->addHeader("Connection", "close");
    response->addHeader("Access-Control-Allow-Origin", "*");
    StaticJsonDocument<256> doc;
    JsonObject root = doc.to<JsonObject>();
    root["relay"] = relay;
    root["millis"] = millis();
    serializeJson(doc, *response);
    request->send(response);
  });

  server.on("/system", HTTP_POST, [](AsyncWebServerRequest * request) {}, NULL, [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    StaticJsonDocument<256> doc;
    deserializeJson(doc, data);

    JsonVariant varRelay = doc["relay"];

    if (!varRelay.isNull()) {
      relay = varRelay.as<bool>();
    }

    request->send(200);
  });

  server.onNotFound([](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/notFound.html", "text/html");
    //request->redirect("/");
  });

  //DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();
}

void loop() {

  digitalWrite(pinRelay, relay);
  delay(100);
}
