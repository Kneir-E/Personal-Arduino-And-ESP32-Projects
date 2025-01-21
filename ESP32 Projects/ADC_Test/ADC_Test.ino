
#define INVOL 3.3
#define ADCRES 4095

int PIN = A0;  // Analog input pin that the potentiometer is attached to
int ADCVAL = 0;  // ADC value obtained
float VOL = 0;  // Voltage value converted from ADC

int index=0;
String ADC_Channel[] = {
  "ADC1_0", "ADC1_1", "ADC1_2", "ADC1_3", "ADC1_4", "ADC1_5", "ADC1_6", "ADC1_7", 
  "", "",
  "ADC2_0", "ADC2_1", "ADC2_2", "ADC2_3", "ADC2_4", "ADC2_5", "ADC2_6", "ADC2_7"
};

void setup() {
  Serial.begin(115200);
}

void loop() {
  
  checkSerial();

  ADCVAL = analogRead(PIN);
  VOL = INVOL * (float) ADCVAL / ADCRES;

  Serial.println(ADC_Channel[index] + "\t-\tADC: " + ADCVAL + "\t-\tVOL: " + VOL);
  delay(500);
}

void checkSerial(){
  if (Serial.available() > 0) {
    String inStr = Serial.readStringUntil('\n');
    inStr.trim();
    int inNum = inStr.toInt();
    if (String(inNum) == inStr) {
      Serial.println("Error: Input is not integer");
      return;
    }
    bool success = true;
    switch(inNum){
      case 0:
        Serial.println("Measuring: ADC1_0");
        PIN = 32;
        break;
      case 1:
        Serial.println("Measuring: ADC1_1");
        PIN = 33;
        break;
      case 2:
        Serial.println("Measuring: ADC1_2");
        PIN = 34;
        break;
      case 3:
        Serial.println("Measuring: ADC1_3");
        PIN = 35;
        break;
      case 4:
        Serial.println("Measuring: ADC1_4");
        PIN = 36;
        break;
      case 5:
        Serial.println("Measuring: ADC1_5");
        PIN = A0;
        break;
      case 6:
        Serial.println("Measuring: ADC1_6");
        PIN = A0;
        break;
      case 7:
        Serial.println("Measuring: ADC1_7");
        PIN = A0;
        break;
      case 10:
        Serial.println("Measuring: ADC2_0");
        PIN = A0;
        break;
      case 11:
        Serial.println("Measuring: ADC2_1");
        PIN = A0;
        break;
      case 12:
        Serial.println("Measuring: ADC2_2");
        PIN = A0;
        break;
      case 13:
        Serial.println("Measuring: ADC2_3");
        PIN = A0;
        break;
      case 14:
        Serial.println("Measuring: ADC2_4");
        PIN = A0;
        break;
      case 15:
        Serial.println("Measuring: ADC2_5");
        PIN = A0;
        break;
      case 16:
        Serial.println("Measuring: ADC2_6");
        PIN = A0;
        break;
      case 17:
        Serial.println("Measuring: ADC2_7");
        PIN = A0;
        break;
      default:
        Serial.println("Pin not found");
        success = false;
        break;
    }
    if(success)index=inNum;
  }
  return;
}
