#include <Servo.h>
#include <EEPROM.h>

bool getbit(uint8_t b, int bitnum) {
  return 1 & (b >> bitnum);
}

struct beat {
  uint8_t byte1, byte2, beatstonext, sn, kk, ht;

  beat (uint8_t b1, uint8_t b2) {
    byte1 = b1;
    byte2 = b2;

    beatstonext = 0b1111111 & (b1 >> 1); //beatstonext is first 7 bits of first byte
    sn = ((1 & b1) * 4) + (0b11 & (b2 >> 6));//snare is last bit of byte 1 in 4s place and first 2 bits of byte 2
    kk = (0b111 & (b2 >> 3)); // kick is 3rd to 5th bit byte 2
    ht = (0b111 & b2); // hat is 6th to 8th bit of byte 2
  }

  beat (uint8_t _beatstonext, uint8_t _sn, uint8_t _kk, uint8_t _ht) {
    beatstonext = _beatstonext;
    sn = _sn;
    kk = _kk;
    ht = _ht;

    byte1 = (beatstonext << 1) + 1 && (sn >> 2);
    byte2 = (sn << 6) + (kk << 3) + ht;
  }
};

class drum {
  Servo motor;

  public:

  drum(Servo &servo){
    motor = servo;
  }

  void hitDown(int velocity) {
    if (velocity > 0){
      motor.write(0);
    }
  }

  void reset(int velocity) {
    if (velocity > 0){
      motor.write(12.8 * velocity);
    }
  }

};

Servo snareServo;
Servo kickServo;
Servo hatServo;

drum snare(snareServo);
drum kick(kickServo);
drum hat(hatServo);

unsigned long time;
unsigned long prevtime;
int bpm;
int beattime;
int count = 0;
int readloc = 0;
beat currenthit(1, 0, 0, 0);
beat prevhit(1, 0, 0, 0);

void setup() {
  Serial.begin(9600);
  delay(200);

  snareServo.attach(3);
  kickServo.attach(5);
  hatServo.attach(6);

  bpm = 200;
  beattime = 60000/bpm;
}

void loop() {
  time = millis();
  // prevhit and currenthit
  // hit down
  // currenthit -> prevhit
  // read currenthit
  // wait a little
  // reset motors to currenthit
  // wait prevhit time

  if(!(time%beattime) && time != prevtime){
    prevtime = time;

    if (count == prevhit.beatstonext){
      snare.hitDown(currenthit.sn);
      kick.hitDown(currenthit.kk);
      hat.hitDown(currenthit.ht);

      Serial.print(currenthit.sn); Serial.print(" ");
      Serial.print(currenthit.kk); Serial.print(" ");
      Serial.print(currenthit.ht); Serial.println(" ");

      prevhit = currenthit;
      currenthit = beat(EEPROM.read(readloc),EEPROM.read(readloc + 1));
      
      readloc += 2;
      
      delay(180);

      snare.reset(currenthit.sn);
      kick.reset(currenthit.kk);
      hat.reset(currenthit.ht);
      count = 0;
    } else {
      Serial.println("0 0 0");
    }
    count ++;
  }
  /*else if (!((time+180)%beattime)){
    if (count == prevhit.beatstonext){
      kick.reset(1);
      snare.reset(1);
    }*/
}