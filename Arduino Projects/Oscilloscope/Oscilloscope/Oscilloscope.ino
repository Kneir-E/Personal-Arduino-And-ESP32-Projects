#define D_PRES 10  // 0.5s
#define D_OPEN 15  // 1.0s
#define D_CLOS 20  // 1.5s
#define D_LOCK 25  // 2.0s
#define SIG_LEEWAY 4 

const int inPin = A1;
int count = 0;
float total = 0;
float ave = 0;

void setup() {
  // put your setup code here, to run once:
  //attachInterrupt(digitalPinToInterrupt(intpin), countUp, FALLING);
  Serial.begin(115200);
  Serial.println(" ~ Serial started ~ ");
}

void loop() {
  for(count=0;count<=100;count++){
    total+=5.0*(float)analogRead(inPin)/1024.0;
  }
  Serial.println(total/100.0);
  total=0;
}