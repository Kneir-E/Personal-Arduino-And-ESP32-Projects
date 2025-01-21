// MOTOR CONTROL
// L298 controls 2 motors:
//    Motor A
//        In1 - In2 - Direction
//         0     0     OFF
//         1     0     Forward   (Out1+ , Out2-)
//         0     1     Backward  (Out1- , Out2+)
//         1     1     OFF
//        EnA - PWM
//
//    Motor B
//        In3, In4 - Direction (same)
//        EnB - PWM

// PINS
#define p_MotIn3 4
#define p_MotIn4 5
#define p_MotEnB 6

void setup() {
  // Set all the motor control pins to outputs
	pinMode(p_MotIn3, OUTPUT);
	pinMode(p_MotIn4, OUTPUT);
	pinMode(p_MotEnB, OUTPUT);
	
	// Initialize Motor Direction : FORWARD
	digitalWrite(p_MotIn3, HIGH);
	digitalWrite(p_MotIn4, LOW);
  Serial.begin(9600);
}

void loop() {
  // Serial.println("Loop Start");
  // int i=0;
  // for(i=0;i<25;i++){
  //   Serial.println( (i*10) );
  //   analogWrite(p_MotEnB, (i*10) );
  //   delay(1000);
  // }

  Serial.println("100");
  analogWrite(p_MotEnB, 100);
  delay(5000);
  Serial.println("200");
  analogWrite(p_MotEnB, 200);
  delay(5000);
  Serial.println("250");
  analogWrite(p_MotEnB, 250);
  delay(5000);
  
}
