// Pins used
#define p_EncA 2
#define p_EncB 3

#define p_MotIn3 4
#define p_MotIn4 5
#define p_MotEnB 6

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

// Constants
const float con_RotTrig = 2400;    // How many edges to measure - used to calculate RPM 
const float con_RotCPR = 2400;     // Rotary Encoder's Counts Per Revolution
const float con_gearRatio = 3;

// Variables
volatile long cur_RotTrig = 0;  // Edge counter
volatile int  lastEncoded = 0;  // previous value
int rpm = 0;
unsigned long lastTime = 0;

unsigned long pwmTime = millis();
unsigned char pwmVal = 0;

void setup() {
  initRotEncoder();
  initSingleMotor();
  Serial.begin(9600);
  Serial.print("\n~~~Serial Started~~~");
}

void loop() {
  unsigned long cur_time = millis();

  // SET PWM values from 0 to 250
  if(millis() - pwmTime >= 2000){
    Serial.print( "\nPWM: " );
    Serial.print( (pwmVal*10) );
    analogWrite(p_MotEnB, (pwmVal*10) );
    if(pwmVal<25) pwmVal++; else pwmVal=0;
    pwmTime = millis();
  }

  // CHECK if ROTATION threshold has been reached
  if (cur_RotTrig == 0){      // if position at 0, reset timer
    rpm = 0;
    lastTime = cur_time;
  } else if (cur_RotTrig >= con_RotTrig) {  // if current rotary triggers reaches count threshold (positive)
    rpm = calcRPM( float( cur_time - lastTime ) );
    cur_RotTrig = 0;
  } else if (cur_RotTrig <= -con_RotTrig) { // if current rotary triggers reaches count threshold (negative)
    rpm = - calcRPM( float( cur_time - lastTime ) );
    cur_RotTrig = 0;
  }
}

int calcRPM(float time){
  float data = ( con_RotTrig * 1 * 1000 * 60 / ( time * con_RotCPR * 1 * 1) ) / con_GearRatio;
  Serial.print("\nRPM: ");
  Serial.print(data);
  printRPM(data);
  return data;
}

void updateEncoder() {
  int MSB = digitalRead(p_EncA); // MSB = most significant bit
  int LSB = digitalRead(p_EncB); // LSB = least significant bit

  int encoded = (MSB << 1) | LSB; // Converting the 2 pin values to single integer
  int sum = (lastEncoded << 2) | encoded; // Shifting last state and adding current state

  // Incrementing or decrementing encoderTicks based on the encoder state transition table
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) cur_RotTrig++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) cur_RotTrig--;

  lastEncoded = encoded; // Store current state as the last state
}

void initRotEncoder(){
  pinMode(p_EncA, INPUT_PULLUP);
  pinMode(p_EncB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(p_EncA), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(p_EncB), updateEncoder, CHANGE);
}

void initSingleMotor(){
  // Set all the motor control pins to outputs
	pinMode(p_MotIn3, OUTPUT);
	pinMode(p_MotIn4, OUTPUT);
	pinMode(p_MotEnB, OUTPUT);
	
	// Initialize Motor Direction : FORWARD
	digitalWrite(p_MotIn3, HIGH);
	digitalWrite(p_MotIn4, LOW);
}

void initLCD(){
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("S T A R T U P");
  lcd.clear();
  lcd.home();
  lcd.print("RPM: ");
  lcd.setCursor(1,0);
}

void printRPM(float data){
  lcd.setCursor(0, 5);
  lcd.print(data);
}