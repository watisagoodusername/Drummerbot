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

void setup() {
  Serial.begin(9600);
  delay(200);

  bool lastbeat = false;
  int i = 0;
  while (!lastbeat) { //read all values
    beat currentbeat = beat(EEPROM.read(2*i), EEPROM.read(2*i+1));
    if (currentbeat.beatstonext == 0){
      lastbeat = true;
    }
    Serial.print(currentbeat.beatstonext); Serial.print(" ");
    Serial.print(currentbeat.sn); Serial.print(" ");
    Serial.print(currentbeat.kk); Serial.print(" ");
    Serial.print(currentbeat.ht); Serial.print(" ");
    Serial.println();

    i++;
  }
}

void loop() {

}
