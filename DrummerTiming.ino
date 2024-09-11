#include <Servo.h>
#include <EEPROM.h>

class drum {
  Servo motor;

  public:

  drum(Servo &servo){
    motor = servo;
  }

  void hitDown() {
    motor.write(0);
  }

  void reset(float velocity) {
    motor.write(90 * velocity);
  }

};

Servo snareServo;
Servo kickServo;
Servo hatServo;

drum snare(snareServo);
drum kick(kickServo);
drum hat(hatServo);

unsigned long time;
int bpm;
int beattime;
int beat;

int hits[2][8] = { {1, 0, 0, 0, 1, 0, 0, 0}, {1, 0, 1, 0, 1, 0, 1, 0} };

void setup() {
  snareServo.attach(3);
  kickServo.attach(5);
  hatServo.attach(6);

  Serial.begin(9600);

  bpm = 100;
  beattime = 60000/bpm;
  beat = 0;
}

void loop() {
  time = millis();

  if (!(time%beattime)){
    //on hit
    //if (hits[0][beat] == 1){
      //kick.hitDown();
    //}
    //if (hits[1][beat] == 1){
      snare.hitDown();
    //}
    beat++;
  } 
  else if(!((time+180)%beattime)){
    //prepare for next hit
    kick.reset(1);
    kick.reset(1);
  }
}