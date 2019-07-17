#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <Relay.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

Relay relay = {false, 2};

const char *ssid = "TURBONETT_1DFD27";
const char *password = "57E04D255E";

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){

  String text = "";
  StaticJsonDocument<256> doc;

  if(type == WS_EVT_CONNECT){
    Serial.println("Client connected");    
    JsonObject root = doc.to<JsonObject>();
    root["relay"] = relay.status;
    serializeJson(doc, text);
    client->text(text.c_str());    
  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("Client disconnected");
  } else if(type == WS_EVT_DATA){
    Serial.println("Client Recive Data");    
    deserializeJson(doc, data);
    JsonVariant varRelay = doc["relay"];
    if (!varRelay.isNull()) {
      relay.status = varRelay.as<bool>();
      digitalWrite(relay.pinRelay, relay.status);
    }
    serializeJson(doc, text);
    ws.textAll(text.c_str());
  }

  Serial.println(text);
}


void setup() {
  //Puerto Serial
  Serial.begin(115200);
  Serial.println();
  //Memoria SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    while (1);
  }
  //Relay
  pinMode(relay.pinRelay, OUTPUT);
  digitalWrite(relay.pinRelay, relay.status);
  //Conectar Wifi
  //WiFi.softAP("ESP32", "87654321");
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

  server.onNotFound([](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/notFound.html", "text/html");
    //request->redirect("/");
  });

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  //DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();
}

void loop() {
  
}

