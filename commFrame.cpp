#include "commFrame.h"
#include <Arduino.h>

commFrame::commFrame(const uint8_t framestarter){
    _framestarter = framestarter;
}

void commFrame::setStarter(const uint8_t framestarter){
    _framestarter = framestarter;
}

void commFrame::setId(const uint8_t id){
    _id = id;
}

void commFrame::setValues(const float value2, const float value1, const float value0){
    _value2 = value2;
    _value1 = value1;
    _value0 = value0;
}

uint8_t commFrame::getStarter(){
    return _framestarter;
}

uint8_t commFrame::getId(){
    return _id;
}

void commFrame::getValues(float &value2, float &value1, float &value0){
    value2 = _value2;
    value1 = _value1;
    value0 = _value0;
}


void divideFloat(const float value, uint8_t &byte3, uint8_t &byte2, uint8_t &byte1, uint8_t &byte0){
    byte3 = ( (uint32_t)value >> 24);
    byte2 = ( ((uint32_t)value) & (0x00FFFFFF) ) >>16;
    byte1 = ( ((uint32_t)value) & (0x0000FFFF) ) >> 8;
    byte0 = ( ((uint32_t)value) & (0x000000FF) ); 
}

void formFloat(float &value, const uint8_t &byte3, const uint8_t &byte2, const uint8_t &byte1, const uint8_t &byte0){
    value = byte3 << 24 | byte2 << 16 | byte1 << 8 | byte0;
}

void sendFrame(commFrame frame){
  /* Extraction des valeurs */
  float value2,value1,value0,checksum;
  uint8_t byteList[4][4];

  frame.getValues(value2,value1,value0);
  checksum = frame.getChecksum();
  /* Conversion des float en 4 octets */
  
  divideFloat(value2,byteList[2][3],byteList[2][2],byteList[2][1],byteList[2][0]); //Decoupage value2
  divideFloat(value1,byteList[1][3],byteList[1][2],byteList[1][1],byteList[1][0]); //Decoupage value1
  divideFloat(value0,byteList[0][3],byteList[0][2],byteList[0][1],byteList[0][0]); //Decoupage value0
  divideFloat(checksum,byteList[3][3],byteList[3][2],byteList[3][1],byteList[3][0]); //Decoupage checksum
  //Début de l'envoi
  Serial2.write(frame.getStarter());
  delay(10);
  Serial2.write(frame.getId());
  //Envoi value 2, value 1 et value 0
        //de l'octet de poids fort à l'octet de poids faible
  for(int i = 2; i>=0; i--){
    for(int j = 3; j>=0; j--){
      delay(10);
      Serial2.write(byteList[i][j]);
    }
  }
  //Envoi du checksum, de l'octet de poids fort à l'octet de poids faible
  for(int i = 3;i>=0;i--){
      delay(10);
      Serial2.write(byteList[3][i]);
  }
}

void readFrame(uint8_t &ID, float &value2, float &value1, float &value0){
    ID = Serial2.read();
    delay(60);
    formFloat(value2,Serial2.read(),Serial2.read(),Serial2.read(),Serial2.read());
    delay(60);
    formFloat(value1,Serial2.read(),Serial2.read(),Serial2.read(),Serial2.read());
    delay(60);
    formFloat(value0,Serial2.read(),Serial2.read(),Serial2.read(),Serial2.read());
}

void readFrame(commFrame &frame){
    uint8_t ID = Serial2.read();
    float value2,value1,value0;
    float checksum;
    delay(60);
    formFloat(value2,Serial2.read(),Serial2.read(),Serial2.read(),Serial2.read());
    delay(60);
    formFloat(value1,Serial2.read(),Serial2.read(),Serial2.read(),Serial2.read());
    delay(60);
    formFloat(value0,Serial2.read(),Serial2.read(),Serial2.read(),Serial2.read());
    delay(60);
    formFloat(checksum,Serial2.read(),Serial2.read(),Serial2.read(),Serial2.read());
    frame.setId(ID);
    frame.setValues(value2,value1,value0);
    frame.setChecksum(checksum);
}

void commFrame::setChecksum(const float checksum){
    _checksum = checksum;
}

float calculateChecksum(commFrame &frame){
    uint8_t ID = frame.getId();
    float value2,value1,value0;
    frame.getValues(value2,value1,value0);

    return ID + value0 + value1 + value2;
}

float commFrame::getChecksum(){
    return _checksum;
}

void affichageFrame(commFrame &frame){
    float value2,value1,value0;
    frame.getValues(value2,value1,value0);
    float checksum = frame.getChecksum();
    Serial.println("Affichage trame recue:");
    Serial.println(frame.getId());
    Serial.print(value2,HEX);
    Serial.print("|");
    Serial.print(value1,HEX);
    Serial.print("|");
    Serial.println(value0,HEX);
    Serial.print("Checksum inclue: ");
    Serial.println(checksum);
    Serial.print("Checksum calculee: ");
    Serial.println(calculateChecksum(frame));
}