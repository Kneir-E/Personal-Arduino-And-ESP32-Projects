const int out0 = 2;
const int out1 = 3;
const int out2 = 4;
const int out3 = 5;
const int ena0 = 6;
const int ena1 = 7;
int data = 0;

void setup() {
  pinMode(out0, OUTPUT);
  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
  pinMode(out3, OUTPUT);
  pinMode(ena0, OUTPUT);
  pinMode(ena1, OUTPUT);
} 

void loop() {
  if(data==100)  data = 0;
  int i = 0;
  for(i=0;i<10;i++){
    setNum(data%10, 0);
    delay(20);
    setNum(data/10, 1);
    delay(20);
  }
  data++;
}

void setNum(int num, int en){
  if(!en){          digitalWrite(ena0, HIGH);
                    digitalWrite(ena1, LOW);}
  else{             digitalWrite(ena0, LOW);
                    digitalWrite(ena1, HIGH);}
  if(num%2)         digitalWrite(out0, HIGH);   else  digitalWrite(out0, LOW);
  if(num/2%2)       digitalWrite(out1, HIGH);   else  digitalWrite(out1, LOW);
  if(num/2/2%2)     digitalWrite(out2, HIGH);   else  digitalWrite(out2, LOW);
  if(num/2/2/2%2)   digitalWrite(out3, HIGH);   else  digitalWrite(out3, LOW);
}

