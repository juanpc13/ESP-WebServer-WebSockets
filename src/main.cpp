#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char *ssid = "TURBONETT_1DFD27";
const char *password = "57E04D255E";

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){

  if(type == WS_EVT_CONNECT){
    Serial.println("Websocket client connection received");
  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("Client disconnected");
  } else if(type == WS_EVT_DATA){
    Serial.print("Data received: ");
    for(int i=0; i < len; i++) {
      Serial.print(char(data[i]));      
    }
    Serial.println();
  }
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
  String text = "";
  StaticJsonDocument<256> doc;
  doc["millis"] = millis();  
  serializeJson(doc, text);
  ws.textAll(text.c_str());
  delay(250);
}

