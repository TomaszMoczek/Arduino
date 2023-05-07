int trig = 13;
int echo = 12;
int czas;
int odleglosc;

int enablePin1 = 11;
int input1A = 10;
int input2A = 7;

int enablePin2 = 6;
int input1B = 4;
int input2B = 5;

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);

  pinMode(enablePin1, OUTPUT);
  pinMode(input1A, OUTPUT);
  pinMode(input2A, OUTPUT);
  digitalWrite(enablePin1, LOW);
  
  pinMode(enablePin2, OUTPUT);
  pinMode(input1B, OUTPUT);
  pinMode(input2B, OUTPUT);
  digitalWrite(enablePin2, LOW);
}

void loop() {
  digitalWrite(trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);
  czas = pulseIn(echo, HIGH, 500000);
  odleglosc = (czas / 2) / 29.1;
  Serial.print(odleglosc);
  Serial.print(" cm");
  Serial.println();
  delay(20);

  if (odleglosc > 30) {
    digitalWrite(input1A, HIGH);
    digitalWrite(input2A, LOW);
    analogWrite(enablePin1, 255);
    
    digitalWrite(input1B, HIGH);
    digitalWrite(input2B, LOW);
    analogWrite(enablePin2, 255);
  }
  else {
    digitalWrite(input1A, LOW);
    digitalWrite(input2A, HIGH);
    analogWrite(enablePin1, 255);
    
    digitalWrite(input1B, HIGH);
    digitalWrite(input2B, LOW);
    analogWrite(enablePin2, 255);
  }
}
