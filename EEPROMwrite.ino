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

    byte1 = (beatstonext << 1) | (1 & (sn >> 2));
    byte2 = (sn << 6) + (kk << 3) + ht;
  }
};

const int leng = 16;
uint8_t snhit[leng];
uint8_t kkhit[leng];
uint8_t hthit[leng];

int count = 0;

void setup() {
  Serial.begin(9600);
  delay(200);

  //EEPROM.write(0, 0b00001001); //4(0000100), 7(111), 5(101), 0(000)
  //EEPROM.write(1, 0b11101000);
  //EEPROM.write(2, 0b11111111);

  //b1 - 7 bits for time between hits, 1 bit joins with other byte to store velocities
  //0000000 000 000 000
  // delay  sn  kk  ht

  /*beat todrum = beat(0b00001001, 0b11101000);

  Serial.print(todrum.beatstonext);
  Serial.print(" ");
  Serial.print(todrum.sn);
  Serial.print(" ");
  Serial.print(todrum.kk);
  Serial.print(" ");
  Serial.print(todrum.ht);
  Serial.println();

  beat tobyte = beat(todrum.beatstonext, todrum.sn, todrum.kk, todrum.ht);

  for (int i = 7; i >= 0; i--) {
    Serial.print(getbit(tobyte.byte1, i));
  } Serial.print(" ");
  for (int i = 7; i >= 0; i--) {
    Serial.print(getbit(tobyte.byte2, i));
  } Serial.println(" ");*/

}

void loop() {
  if (Serial.available() > 0) {
      int incomingbyte = Serial.read();// read the incoming byte 
      incomingbyte -= 48; // char to int, since 0 is 48 and 9 is 57
              
      if (incomingbyte >= 0 and incomingbyte < 8) {// if incoming byte is a valid uint8

        //write to lists
        if (floor(count/leng) == 0){// for the first leng incoming bytes it writes the value to the snhit list
          snhit[count] = incomingbyte;
          count ++;
          
        } else if (floor(count/leng) == 1){// for the second leng it writes to kkhit
          kkhit[count - leng] = incomingbyte;
          count ++;
          
        } else if (floor(count/leng) == 2){// for the third it writes to hthit
          hthit[count - 2 * leng] = incomingbyte;
          count ++;
        }

        Serial.print(count);
    }
    if (count == 3 * leng) {//once it has recieved every track do calculations on them
      count ++;// so this only runs once

      //print lists
      Serial.println();
      for(int i = 0; i < leng; i++){
        Serial.print(snhit[i]);
      } Serial.println();

      for(int i = 0; i < leng; i++){
        Serial.print(kkhit[i]);
      } Serial.println();

      for(int i = 0; i < leng; i++){
        Serial.print(hthit[i]);
      } Serial.println();

      //convert to bytes for the eeprom 

      int beattimes[leng] = { 0 };// leng is the maximum amount of hits
      int beattimeslen = 0;

      int prev = 0;
      for (int i = 4; i < leng; i++) {// calculate beats in between hits and store to beattimes[]
        if (snhit[i] != 0 or kkhit[i] != 0 or hthit[i] != 0) {
          beattimeslen++;
          beattimes[beattimeslen - 1] = i - prev;

          prev = i;
        }
      } beattimeslen++;

      Serial.println();
      int b = 0;
      for (int i = 0; i < beattimeslen; i++)  {
        //Serial.print(beattimes[i]);
        beat currentbeat = beat(beattimes[i], snhit[b], kkhit[b], hthit[b]);
        b += beattimes[i];

        // dont run this one too often
        EEPROM.write(2*i, currentbeat.byte1);
        EEPROM.write(2*i + 1, currentbeat.byte2);

        Serial.print(currentbeat.beatstonext);
        Serial.print(" ");
        Serial.print(currentbeat.sn);
        Serial.print(" ");
        Serial.print(currentbeat.kk);
        Serial.print(" ");
        Serial.print(currentbeat.ht);
        Serial.println();

        for (int i = 7; i >= 0; i--) {
          Serial.print(getbit(currentbeat.byte1, i));
        } Serial.print(" ");
        for (int i = 7; i >= 0; i--) {
          Serial.print(getbit(currentbeat.byte2, i));
        } Serial.println(" ");
      }
      
    }
  }
}
