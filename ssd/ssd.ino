#include "jisc_ssd.h"

void setup() {
  pinMode(PIN_LED, OUTPUT);
  jisc_ssd_lowlevel_operation_init();
  Serial.begin(115200);

  //Serial表示待ち
  while (!Serial) {
    digitalWrite(PIN_LED, LOW);
    delay(100);
    digitalWrite(PIN_LED, HIGH);
    delay(100);
  }


  Serial.println("Ready");
  int page = 5;
  jisc_ssd_lowlevel_operation_reset();
  read_id();
  read_status();
/*
  erase_all();
  int x=0xAA;
  for(int i=0;i<jisc_ssd_block_size;i++){
    jisc_ssd_block_buffer[i] = x;
    x ^= 0xFF;
  }
  for(int i=0;i<1024;i++){
    write_page(i);
    //read_status();
  }
*/
  //All pages
  for(int i=0;i<65536;i++){
    read_page(i);
    //read_status();
  }
/*
  read_status();
  //erase_all();
  //bad_block_test();
  
  erase_page(page);
  read_status();

  read_page(page);
  read_status();

  write_page(page);
  read_status();

  read_page(page);
  read_status();
*/
  Serial.println("Done");
}

void loop() {
}
