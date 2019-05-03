#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "CSS.h"
#include "HTML.h"
#include "JavaScript.h"

ESP8266WebServer server(80);
const char* ssid = "TURBONETT_1DFD27";
const char* password = "";

void setup() {
  //Puerto Serial
  Serial.begin(115200);
  Serial.println();
  //Conectar Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  //Mostrar IP
  Serial.print("IP : ");Serial.println(WiFi.localIP());
  //Eventos
  server.on("/", handleRoot);
  server.on("/js", handleJS);
  server.on("/css", handleCSS);
  server.on("/data", handleJsonData);
  server.begin();

}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String s = MAIN_HTML; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

void handleJS() {
  String s = MAIN_JS; //Read JS contents
  server.send(200, "text/javascript", s); //Send page
}

void handleCSS() {
  String s = MAIN_CSS; //Read JS contents
  server.send(200, "text/css", s); //Send page
}

void handleJsonData() {
  String s = "";
  DynamicJsonDocument doc(1024);
  JsonObject root = doc.to<JsonObject>();

  root["time"] = millis();

  serializeJson(doc, s);

  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", s); //Send json page
}

