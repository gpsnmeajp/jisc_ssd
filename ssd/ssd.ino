#include "jisc_ssd.h"

void setup() {
  pinMode(PIN_LED, OUTPUT);
  jisc_ssd_lowlevel_command_init();
  Serial.begin(115200);

  //Serial表示待ち
  while (!Serial) {
    digitalWrite(PIN_LED, LOW);
    delay(100);
    digitalWrite(PIN_LED, HIGH);
    delay(100);
  }

  Serial.println("Ready");
  int page = 1;
  jisc_ssd_lowlevel_command_reset();
  read_id();
  read_status();

  read_page(page);
  read_status();
  //erase_all();
  bad_block_test();
  /*
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
