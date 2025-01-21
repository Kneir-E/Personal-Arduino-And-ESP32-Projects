#include "ConfigManager.h"

String  STA_SSID="PLDTHOMEFIBR4fc38",
        STA_PASS="PLDTWIF!11b9b", 
        STA_HOST="", 
        STA_PORT="", 
        AP_SSID="TEMP_SSID", 
        AP_PASS="TEMP_PASS", 
        PKT_HEADER="", 
        PKT_TRAILER="";

void STA_Init(){
  WiFi.softAPdisconnect();
  WiFi.disconnect(false, true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STA_SSID, STA_PASS);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("...");
  }
  Serial.println("WiFi_STA Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void conf_callback_trig(){
  Serial.println("Callback on trigger");
  // Disconnect client
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  STA_Init();
  conf_Init(conf_callback_trig, STA_Init);
}

void loop() {
  // put your main code here, to run repeatedly:
  config_mode_loop();
}