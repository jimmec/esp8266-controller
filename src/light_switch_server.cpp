#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "light_switch_server.h"


const char* HTTPLightSwitchServer::_serverIndex =
R"(<html><body><h1>Jimmys Light Switch v0.1</h1>
Hello! I'm a light switch:
<form method='POST' action='/switch1'>
                  <input name='toggle' value='1'>
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
      _server->sendHeader("Connection", "close");
      _server->sendHeader("Access-Control-Allow-Origin", "*");
      _server->send(200, "text/html", _serverIndex);
    });

    // handler for the /update form POST (once file upload finishes)
    _server->on("/switch1", HTTP_POST, [&](){
      _server->sendHeader("Connection", "close");
      _server->sendHeader("Access-Control-Allow-Origin", "*");
      _server->send(200, "text/html", "<META http-equiv=\"refresh\" content=\"15;URL=/switch1\">OK");
      ESP.restart();
    },[&](){
      // handler for a toggling command
      if (_serial_output) Serial.println("toggled!");
      digitalWrite(_switch_pin, 1 - _switch_state);
      delay(100);
    });
}
