#include <Arduino.h>
#include "sona_rs485.h"
sona_rs485 sona(5);
uint8_t data[13];
void setup() {
    sona.init();
}

void loop() {
  static unsigned long last_millis=0;
  static unsigned long command_time=0;
  static bool still_command=false;

  if(millis()-last_millis >= 250 && still_command == false){
    last_millis = millis();
    Serial.println("1");
    Serial.println(millis());
    sona.send_command(0x01);
    command_time = millis();
    still_command = true;
  }else if(millis()-command_time >= 50 && still_command == true){
    Serial.println("2");
    Serial.println(millis());
    sona.get_data(data);
    for(int i = 0 ;i<13;i++){
      Serial.print(data[i]);
      Serial.print(" ");
    }
    Serial.println();
    still_command=false;
  }
}