#ifndef __HTTP_LIGHT_SWITCH_SERVER_H
#define __HTTP_LIGHT_SWITCH_SERVER_H

class ESP8266WebServer;

class HTTPLightSwitchServer
{
  private:
    bool _serial_output;
    ESP8266WebServer *_server;
    static const char *_serverIndex;
    int _switch_state;
    int _switch_pin;
  public:
    HTTPLightSwitchServer(bool serial_debug=false, int switch_pin=5);
    void setup(ESP8266WebServer *server=NULL);
};


#endif
