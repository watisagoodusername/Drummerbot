
#include <EEPROM.h>

bool getbit(uint8_t b, int bitnum) {
  return 1 & (b >> bitnum);
}

struct beat {
  uint8_t beatcount, sn, kk, ht;

  beat (uint8_t b1, uint8_t b2) {
    beatcount = 0b1111111 & (b1 >> 1); //beatcount is first 7 bits of first byte
    sn = ((1 & b1) * 4) + (0b11 & (b2 >> 6));//snare is last bit of byte 1 in 4s place and first 2 bits of byte 2
    kk = (0b111 & (b2 >> 3)); // kick is 3rd to 5th bit byte 2
    ht = (0b111 & b2); // hat is 6th to 8th bit of byte 2
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

}

void loop() {
  if (Serial.available() > 0) {
      int incomingbyte = Serial.read();// read the incoming byte 
      incomingbyte -= 48; // char to int

      Serial.print(count);
              
      if (incomingbyte >= 0 and incomingbyte < 10) {
        if (floor(count/leng) == 0){
          snhit[count] = incomingbyte;
          count ++;
          
        } else if (floor(count/leng) == 1){
          kkhit[count - leng] = incomingbyte;
          count ++;
          
        } else if (floor(count/leng) == 2){
          hthit[count - 2 * leng] = incomingbyte;
          count ++;
        }
    }
    if (count = 3 * leng) {
      count ++;
      for(int i = 0; i < leng; i++){
        Serial.print(snhit[i]);
      } Serial.println();

      for(int i = 0; i < leng; i++){
        Serial.print(kkhit[i]);
      } Serial.println();

      for(int i = 0; i < leng; i++){
        Serial.print(hthit[i]);
      } Serial.println();
    }
  }
}
