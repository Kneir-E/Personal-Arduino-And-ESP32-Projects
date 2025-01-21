#ifndef AP_CONFIG_FUNCTIONS_H
#define AP_CONFIG_FUNCTIONS_H

#include "Config_WebFiles.h"
#include "Config_StringHandling.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#ifndef CONFIG_INT_PIN
#define CONFIG_INT_PIN 19
#endif

AsyncWebServer __conf_server(80);
unsigned long __conf_SrvStartTime;    // The time that the __conf_server has been started
bool    __conf_enabled = false;          // Config Server current status
bool    __conf_trig_flag = false;        // true if Config Server button has been pressed
bool    __conf_successful_input = false;   // true if user successfully inputted data, false if user exitted
String  __conf_scanned_networks = "";
int     __conf_scan_network_iter = 0;

typedef void (*CallbackFunction)();
CallbackFunction __conf_Call_On_Trigger = nullptr;
CallbackFunction __conf_Call_On_Submit = nullptr;

extern String STA_SSID;
extern String STA_PASS;
extern String STA_HOST;
extern String STA_PORT;
extern String AP_SSID;
extern String AP_PASS;
extern String PKT_HEADER;
extern String PKT_TRAILER;

void __conf_AP_Init();
void __conf_Mode_Enable();
void __conf_Mode_Disable();
void __conf_Server_Init();
void __conf_Scan_Networks();
void config_mode_loop(int expire_time_seconds);
void __conf_ISR();
void conf_Init(CallbackFunction callBeforeTrigger, CallbackFunction callOnSubmit);
bool __config_process_paste(String data);


void __conf_AP_Init(){
  WiFi.disconnect(false, true);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void __conf_Mode_Enable(){
  // disconnect Client
  // disconnect STA

  __conf_AP_Init();
  __conf_Server_Init();
  __conf_server.begin();
  __conf_SrvStartTime = millis();
  __conf_enabled = true;
}

void __conf_Mode_Disable(){
  WiFi.disconnect(true, true);
  __conf_server.reset();
  __conf_enabled = false;
  Serial.println("Server Ended");
  if (__conf_Call_On_Submit != nullptr) {
    __conf_Call_On_Submit();
  }
}

void __conf_Server_Init(){
  __conf_server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->redirect("/main-menu");
  });

  __conf_server.on("/main-menu", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", main_menu_html);
  });

  __conf_server.on("/edit-wifi", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", edit_wifi_html);
  });

  __conf_server.on("/edit-all", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", edit_all_html);
  });

  __conf_server.on("/paste", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", paste_html);
  });

  __conf_server.on("/reset-confirm", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", reset_confirm_html);
  });

  __conf_server.on("/exit", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("Exitted");
    __conf_Mode_Disable();
  });

  __conf_server.on("/style", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/css", style_css);
  });

  __conf_server.onNotFound([](AsyncWebServerRequest* request){
    request->send(200, "text/html", not_found_html);
  });

  __conf_server.on("/error", HTTP_GET, [](AsyncWebServerRequest* request){
    request->send(200, "text/html", error_html);
  });

  __conf_server.on("/ssid-scan", HTTP_GET, [](AsyncWebServerRequest* request){
    String response = __conf_scanned_networks;
    request->send(200, "text/plain", response);
  });

  __conf_server.on("/update-wifi", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("Received: HTTP_GET /update-wifi");

    String _sta_ssid  = "";
    String _sta_pass  = "";
    String _sta_host  = "";
    String _sta_port  = "";

    _sta_ssid = request->getParam("STA_SSID")->value();
    _sta_pass = request->getParam("STA_PASS")->value();
    _sta_host = request->getParam("STA_HOST")->value();
    _sta_port = request->getParam("STA_PORT")->value();

    if(_sta_ssid != "")   STA_SSID = _sta_ssid;
    if(_sta_pass != "")   STA_PASS = _sta_pass;
    if(_sta_host != "")   STA_HOST = _sta_host;
    if(_sta_port != "")   STA_PORT = _sta_port;

    request->redirect("/exit");
  });

  __conf_server.on("/update-all", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("Received: HTTP_GET /update-all");

    String _ap_ssid   = "";
    String _ap_pass   = "";
    String _sta_ssid  = "";
    String _sta_pass  = "";
    String _sta_host  = "";
    String _sta_port  = "";
    String _header    = "";
    String _trailer   = "";

    _sta_ssid = request->getParam("STA_SSID")->value();
    _sta_pass = request->getParam("STA_PASS")->value();
    _sta_host = request->getParam("STA_HOST")->value();
    _sta_port = request->getParam("STA_PORT")->value();
    _ap_ssid  = request->getParam("AP_SSID")->value();
    _ap_pass  = request->getParam("AP_PASS")->value();
    _header   = request->getParam("PKT_Head")->value();
    _trailer  = request->getParam("PKT_Trail")->value();

    if(_sta_ssid != "")   STA_SSID = _sta_ssid;
    if(_sta_pass != "")   STA_PASS = _sta_pass;
    if(_sta_host != "")   STA_HOST = _sta_host;
    if(_sta_port != "")   STA_PORT = _sta_port;
    if(_ap_ssid != "")    AP_SSID  = _ap_ssid;
    if(_ap_pass != "")    AP_PASS  = _ap_pass;
    if(_header != "")     PKT_HEADER   = _header;
    if(_trailer != "")    PKT_TRAILER  = _trailer;

    request->redirect("/exit");
  });

  __conf_server.on("/update-paste", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("Received: HTTP_GET /update-paste");
    String paste = request->getParam("All_Config")->value();
    Serial.println(paste);
    bool success = __config_process_paste(paste);

    if(success){
      request->send(200, "text/html", success_html);
      request->redirect("/exit");
    }else{
      request->send(200, "text/html", error_html);
    }
  });

  __conf_server.on("/reset-confirmed", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("Received: HTTP_GET /reset-confirmed");
    request->redirect("/exit");
  });
}

void __conf_Scan_Networks(){
  int n = WiFi.scanNetworks();
  int i = 0;
  __conf_scanned_networks = "";
  for(i=0;i<n;++i){
    __conf_scanned_networks += String(WiFi.SSID(i).c_str()) + ":" + String(WiFi.RSSI(i)) + ";";
  }
  WiFi.scanDelete();
}

void config_mode_loop(int expire_time_seconds=120){

  if(__conf_trig_flag==true){ 

    // Conduct network scan
    __conf_Scan_Networks();

    // Enable configure mode
    __conf_Mode_Enable();   

    // Trigger callback function  
    if(__conf_Call_On_Trigger!=nullptr){
      __conf_Call_On_Trigger();
    }

    //Consume trigger flag
    __conf_trig_flag=false;

  }else if(__conf_enabled){
    __conf_trig_flag=false;
  }

  // Check the config mode timeout
  if(__conf_enabled && (millis() - __conf_SrvStartTime >= expire_time_seconds*1000) ){
    __conf_Mode_Disable();
  }
}

void __conf_ISR(){
  if(!__conf_enabled){
    __conf_trig_flag = true;
  }
}

void conf_Init(CallbackFunction callBeforeTrigger = nullptr, CallbackFunction callOnSubmit = nullptr){
  // Initialize pin for wifi manager setup
  pinMode(CONFIG_INT_PIN, INPUT_PULLUP);
  attachInterrupt(
    digitalPinToInterrupt(CONFIG_INT_PIN),
    __conf_ISR,
    FALLING
  );

  // Initialize callbacks
  __conf_Call_On_Trigger = callBeforeTrigger;     // Set function to call when 
  __conf_Call_On_Submit = callOnSubmit;
}

bool __config_process_paste(String data){
  data = removeWhitespace(data);

  bool success = true;

  String keyValuePairs[8];  // Array to store the key-value pairs
  int tokenCount = splitString(data, ';', keyValuePairs, 8);

  // Check if the number of variables is correct
  if (tokenCount < 8) {
      Serial.println("Not enough variables");
      return false;
  } else if (tokenCount > 8) {
      Serial.println("Too many variables");
      return false;
  }

  String _ap_ssid   = "";
  String _ap_pass   = "";
  String _sta_ssid  = "";
  String _sta_pass  = "";
  String _sta_host  = "";
  String _sta_port  = "";
  String _header    = "";
  String _trailer   = "";

  // Process each key-value pair
  for (int i = 0; i < tokenCount; i++) {
    int colonIndex = keyValuePairs[i].indexOf(':');
    if (colonIndex != -1) {
      String variableName = keyValuePairs[i].substring(0, colonIndex);
      String value = keyValuePairs[i].substring(colonIndex + 1);

      Serial.println(variableName + " " + value);

      if(value=="") continue;
      
      // Store the values in the corresponding variables
      if      (variableName == "STA_SSID")  _sta_ssid = value;
      else if (variableName == "STA_PASS")  _sta_pass = value;
      else if (variableName == "STA_HOST")  _sta_host = value;
      else if (variableName == "STA_PORT")  _sta_port = value;
      else if (variableName == "AP_SSID")   _ap_ssid = value;
      else if (variableName == "AP_PASS")   _ap_pass = value;
      else if (variableName == "PKT_Head")  _header = value;
      else if (variableName == "PKT_Trail") _trailer = value;
      else{
        Serial.println("fail at " + variableName + " " + value);
        success=false;
        break;
      }
    }
  }

  if(success){
    if(_sta_ssid != "")   STA_SSID    = _sta_ssid;
    if(_sta_pass != "")   STA_PASS    = _sta_pass;
    if(_sta_host != "")   STA_HOST    = _sta_host;
    if(_sta_port != "")   STA_PORT    = _sta_port;
    if(_ap_ssid != "")    AP_SSID     = _ap_ssid;
    if(_ap_pass != "")    AP_PASS     = _ap_pass;
    if(_header != "")     PKT_HEADER  = _header;
    if(_trailer != "")    PKT_TRAILER = _trailer;
  }
  
  return success;
}


#endif