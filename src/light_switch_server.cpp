#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "light_switch_server.h"


const char* HTTPLightSwitchServer::_serverIndex =
R"(<html><body><h1>Jimmys Light Switch v0.1</h1>
Hello! I'm a light switch, current state = %d!
<form method='POST' action='/switch1'>
                  <button>toggle me</button>
               </form>
         </body></html>)";

HTTPLightSwitchServer::HTTPLightSwitchServer(bool serial_debug, int switch_pin)
{
  _serial_output = serial_debug;
  _server = NULL;
  _switch_state = 0;
  _switch_pin = switch_pin;
}

void HTTPLightSwitchServer::setup(ESP8266WebServer *server)
{
    _server = server;

    // handler for the /switch1 form page
    _server->on("/switch1", HTTP_GET, [&](){
        showIndex();
    });

    // handler for the /update form POST (once file upload finishes)
    _server->on("/switch1", HTTP_POST, [&](){
      // handler for a toggling command
      toggleSwitch();

      showIndex();
    },[&](){
      // do nothing to handle file uploads
    });
}

void HTTPLightSwitchServer::toggleSwitch() {
  // toggle switch state
  _switch_state = 1 - _switch_state;

  digitalWrite(_switch_pin, _switch_state);
  if (_serial_output) Serial.println("toggled!");

  delay(100);
}

void HTTPLightSwitchServer::showIndex() {
  char resp[200];
  sprintf(resp, _serverIndex, _switch_state);
  _server->sendHeader("Connection", "close");
  _server->sendHeader("Access-Control-Allow-Origin", "*");
  _server->send(200, "text/html", resp);
}
