#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <OneWire.h>
#include <BH1750.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include "Temp-Web.h"

#define ONE_WIRE_BUS 2  
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds(&oneWire);
float dsCurrTemp = 0;

const char* ssid = "TemperatureSensor";
const char* pass = "kamehameha";

BH1750 lightMeter;
float bhCurrLux = 0;
bool sensor_detected = false;

AsyncWebServer server(80);

void setup(void) {
  Serial.begin(115200);
  ds.begin();
  bh_Init();
  server_init();
}

void loop(void) {
  ds_GetTemp();
  bh_read();
}

void ds_GetTemp() {
  ds.requestTemperatures();
  dsCurrTemp = ds.getTempCByIndex(0);
}

void server_init() {
  WiFi.softAP(ssid, pass);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  // Serial.println(WiFi.localIP());


  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", webpage);
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("Received: HTTP_GET /temperature");
    String response = "";
    if (dsCurrTemp != DEVICE_DISCONNECTED_C) {
      response = "Detected-" + String(dsCurrTemp);
    } else {
      response = "Not_Detected-0.0";
    }
    response += "-" + String(sensor_detected) + "-" + String(bhCurrLux);
    Serial.println("Responded with " + response);
    request->send(200, "text/plain", response);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/css", style_css);
  });

  // Start the server
  server.begin();
}


void bh_Init(){
  if(!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)){
    Serial.println(F("Sensor not detected"));
    sensor_detected = false;
  }else{
    Serial.println(F("Sensor initialized"));
    sensor_detected = true;
  }
  
}


void bh_read(){
  if(!sensor_detected){
    bhCurrLux = 0.0;
    return;
  }

  unsigned long start_time = millis();

  while (!lightMeter.measurementReady(true)) {
    if(millis() - start_time == 2000){
      sensor_detected = false;
      bhCurrLux = 0.0;
      return;
    }
  }
  float lux = lightMeter.readLightLevel();
  // Serial.print("Light: ");
  // Serial.print(lux);
  // Serial.println(" lx");
  // lightMeter.configure(BH1750::ONE_TIME_HIGH_RES_MODE);

  bhCurrLux = lux;
}