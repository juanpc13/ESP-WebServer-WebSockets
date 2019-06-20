#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
const char *ssid = "TURBONETT_1DFD27";
const char *password = "57E04D255E";

int luz = 0;
int sensibilidad = 0;
bool relay = false;
#define pinRelay D5

void setup(){
  //Puerto Serial
  Serial.begin(115200);
  Serial.println();
  //Memoria SPIFFS
  SPIFFS.begin();
  //Relay
  pinMode(pinRelay, OUTPUT);
  digitalWrite(pinRelay, LOW);
  //Conectar Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  //Mostrar IP
  Serial.print("IP : ");
  Serial.println(WiFi.localIP());
  //Eventos
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/main.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/main.css", "text/css");
  });

  server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/main.js", "text/javascript");
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    response->addHeader("Connection", "close");
    response->addHeader("Access-Control-Allow-Origin", "*");
    StaticJsonDocument<256> doc;
    JsonObject root = doc.to<JsonObject>();
    root["luz"] = luz;
    root["relay"] = relay;
    root["sensibilidad"] = sensibilidad;
    serializeJson(doc, *response);
    request->send(response);
  });

  server.on("/system", HTTP_POST, [](AsyncWebServerRequest * request) {}, NULL, [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    StaticJsonDocument<256> doc;
    deserializeJson(doc, data);

    JsonVariant varRelay = doc["relay"];
    JsonVariant varSen = doc["sensibilidad"];    

    if (!varRelay.isNull()) {
      relay = varRelay.as<bool>();
    }

    if(!varSen.isNull()){
      sensibilidad = varSen.as<int>();
    }
    
    request->send(200);
  });

  server.onNotFound([](AsyncWebServerRequest *request) {    
    request->send(SPIFFS, "/notFound.html", "text/html");
    //request->redirect("/");
  });

  //DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();
}

void loop(){
  luz = analogRead(A0);
  bool condicion = (relay || sensibilidad > luz);
  digitalWrite(pinRelay, condicion);  
  delay(100);
}
