#include <Arduino.h>
#include "sona_rs485.h"
sona_rs485::sona_rs485(int _cs_pin){
   cs_pin = _cs_pin;
}

void sona_rs485::init(){
   Serial2.begin(9600);
   while (!Serial2); // teensy硬體問題，需等待他開啟Serial功能
   pinMode(cs_pin,OUTPUT);
   command_data[0] = head1;   // 標頭1
   command_data[1] = head2;   // 標頭2
   command_data[2] = address; // 命令第三個位置放address
}

int sona_rs485::checksum(uint8_t *buf,int len){
   uint8_t sum = 0;
   
   for(int i=0;i<len-1;i++){
      sum += *buf++;
   }

   return sum;
}

void sona_rs485::send_command(int command){
   digitalWrite(cs_pin,HIGH);
   command_data[3] = command; // 命令第四個位置放命令的編號
   command_data[4] = checksum(command_data,sizeof(command_data)/sizeof(command_data[0]));

   for(int i=0;i<5;i++){
      Serial2.write(command_data[i]);
   }
}

void sona_rs485::get_data(unsigned char* sona_data_buffer){
   digitalWrite(cs_pin,LOW);
   int sona_data_flag = 0;
   while(Serial2.available()){
      Serial.println("have read");
      sona_data[sona_data_flag] = Serial2.read();
      // 標頭確認
      if(sona_data_flag == 1 && sona_data[sona_data_flag - 1] != head1 && sona_data[sona_data_flag] != head2)
      {
         sona_data_flag = 0;
         break;
      }
      // 確認接完所有資料
      else if(sona_data_flag == 12)
      {
         // checksum確認
          if(sona_data[sona_data_flag] != checksum(sona_data,sizeof(sona_data)))
          {
             sona_data_flag = 0;
             break;
          }
          else
          {
             for(int i =0;i<sizeof(sona_data);i++)
             {
                sona_data_buffer[i] = sona_data[i];
             }
             break;
          }
      }
      sona_data_flag++;
   }
}