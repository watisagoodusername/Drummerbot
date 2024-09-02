#include <Servo.h>

const int button = 2;

Servo snare;
Servo kick;
Servo hat;

void hit(Servo drum, int velocity) {
  drum.write(90);
  delay(250);
  drum.write(0);
}

void setup() {
  snare.attach(3);
  kick.attach(5);
  hat.attach(6);

  pinMode(button, INPUT);

  Serial.begin(9600);
}

void loop() {
  int pressed = 0;
  pressed = digitalRead(button);
  if (pressed == HIGH){
    Serial.println("AAAAA");
    hit(snare,0);
    delay(1000);
  } else {
    Serial.println("BBBBBB");
  }
}
