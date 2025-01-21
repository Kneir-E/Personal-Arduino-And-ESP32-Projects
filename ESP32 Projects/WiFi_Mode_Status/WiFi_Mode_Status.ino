/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <WiFi.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2  // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED
#endif

// Set these to your desired credentials.
const char *apssid = "yourAP";
const char *appassword = "yourPassword";

const char *ssid = "PLDTHOMEFIBR4fc38";
const char *password = "PLDTWIF!11b9b";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  delay(5000);

  Serial.println("Startup - " + String(WiFi.getMode()) + " - " + String(WiFi.status()) );
  
  Serial.println("Configuring access point...");
  if (!WiFi.softAP(apssid, appassword)) {
    Serial.println("Soft AP creation failed.");
    while (1);
  }

  Serial.println("SoftAP mode - " + String(WiFi.getMode()) + " - " + String(WiFi.status()) );

  WiFi.disconnect(true);

  Serial.println("Disconnect - " + String(WiFi.getMode()) + " - " + String(WiFi.status()) );

  int n = WiFi.scanNetworks();
  Serial.println("Scan Networks - " + String(WiFi.getMode()) + " - " + String(WiFi.status()) );
  int i = 0;
  for(i=0;i<n;++i){
    Serial.println( String(WiFi.SSID(i).c_str()) + ":" + String(WiFi.RSSI(i)) );
  }
  WiFi.scanDelete();
  Serial.println("Scan Delete - " + String(WiFi.getMode()) + " - " + String(WiFi.status()) );


  WiFi.begin(ssid, password);
  Serial.println("WiFi begin - " + String(WiFi.getMode()) + " - " + String(WiFi.status()) );
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting - " + String(WiFi.getMode()) + " - " + String(WiFi.status()) );
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected - " + String(WiFi.getMode()) + " - " + String(WiFi.status()) );

  WiFi.disconnect(true);
  Serial.println("Disconnect from STA - " + String(WiFi.getMode()) + " - " + String(WiFi.status()) );
}

void loop() {

}
