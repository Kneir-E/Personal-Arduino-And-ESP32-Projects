// C++ code
//
const int pin_Buzzer = 13;  //buzzer output pin
const int freq[9] = {261, 293, 329, 349, 391, 440, 493, 523, 587};
#define do1 freq[0]
#define re1 freq[1]
#define mi1 freq[2]
#define fa1 freq[3]
#define so1 freq[4]
#define la1 freq[5]
#define ti1 freq[6]
#define do2 freq[7]
#define re2 freq[8]

#define MSCDEL 1000
//const int freq[9] = {349, 391, 440, 493, 523, 587, 659, 698, 783};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Serial Functions Begun");
  pinMode(pin_Buzzer, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);

  play(do1);
  delay(MSCDEL*0.7);
  noTone(pin_Buzzer);
  delay(MSCDEL*0.3);

  play(do1);
  delay(MSCDEL*0.3);
  play(re1);
  delay(MSCDEL*0.3);
  play(la1);
  delay(MSCDEL*0.5);
  play(so1);
  delay(MSCDEL*0.3);

  noTone(pin_Buzzer);
  delay(MSCDEL*0.5);
  
  play(do1);
  delay(MSCDEL*0.3);
  play(re1);
  delay(MSCDEL*0.3);
  play(la1);
  delay(MSCDEL*0.5);
  play(so1);
  delay(MSCDEL*0.3);

  noTone(pin_Buzzer);
  delay(MSCDEL*0.5);

  play(do1);
  delay(MSCDEL*0.5);

  noTone(pin_Buzzer);
  delay(MSCDEL*0.1);

  play(do2);
  delay(MSCDEL*0.5);
  noTone(pin_Buzzer);
  while(1){}
}

void play(int note){
  tone(pin_Buzzer, note);
}


// Formula: out * size_rc_in + in