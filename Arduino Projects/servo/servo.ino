#include <Servo.h>

Servo servo_1; // servo controller (multiple can exist)

int servo_pin = 3;
int pos = 0;    // servo starting position
int sig = 0;

void setup() {
  // put your setup code here, to run once:
  servo_1.attach(servo_pin); // start servo control
  Serial.begin(9600); // start serial monitor
  servo_1.write(pos); // move servo to 0 degrees
  Serial.println("Positioned at 0 Degrees");
  Serial.println("Input Desired Angle and Press Enter");
}

void setSig(){
  sig=!sig;
}

void loop() {
  attachInterrupt(digitalPinToInterrupt(2), setSig, FALLING);
  if(sig){
    servo_1.write(180); // convert angle and write servo
    _delay_ms(500); // delay for maximum speed
  }
  if(sig){
    servo_1.write(90); // convert angle and write servo
    _delay_ms(500);
  }
  if(sig){
    servo_1.write(0); // convert angle and write servo
    _delay_ms(500);
  }
  if(sig){
    servo_1.write(180); // convert angle and write servo
    _delay_ms(500);
  }
  
  // while (Serial.available()){
  //   String in_char = Serial.readStringUntil('\n'); // read until the newline
  //   Serial.print("Moving to: ");
  //   Serial.print(in_char);
  //   Serial.println(" Degrees");
  //   servo_1.write(in_char.toInt()); // convert angle and write servo
  //   delay(in_char.toFloat()*(10.0/6.0)); // delay for maximum speed
  // }
}
