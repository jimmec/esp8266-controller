/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "light_switch_server.h"

const char* host = "jc-lights";
const char* ssid = "Free Wifi2";
const char* password = "gimm3internetz";
const int switchPin = 5;

ESP8266WebServer httpServer(80);
HTTPLightSwitchServer httpSwitchServer;

void setup(void){

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");

  pinMode(switchPin, OUTPUT);
  digitalWrite(switchPin, LOW);
  delay(100);

  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  while(WiFi.waitForConnectResult() != WL_CONNECTED){
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
    Serial.println(WiFi.status());
  }

  MDNS.begin(host);

  httpSwitchServer.setup(&httpServer);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPLightSwitchServer ready! Open http://%s.local/switch1 in your browser\n", host);
}

void loop(void){
  httpServer.handleClient();
  delay(1);
}
