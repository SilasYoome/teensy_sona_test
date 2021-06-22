#pragma once
#define head1 0x55
#define head2 0xAA
#define address 0x01

class sona_rs485{
    public:
        sona_rs485(int _cs_pin);
        void init();
        void send_command(int command);
        void get_data(uint8_t* sona_data_buffer);
        int checksum(uint8_t *buf,int len);

    private:
        int cs_pin;
        uint8_t command_data[5];  //{head1(0x55),head2(0xAA),address,command,check}
        uint8_t sona_data[13];  //{head1,head2,address,command,D1H,D1L,D2H,D2L,D3H,D3L,D4H,D4L,check}
      
};