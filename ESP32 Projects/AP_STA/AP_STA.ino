#include <WiFi.h>

const char *ssid = "ESPNetwork";
const char *password = "password";
const char *APssid = "ESP32-WiFi";
const char *APpassword = "password";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Serial Started");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  WiFi.softAP(APssid, APpassword);
  
  WiFi.begin();

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
