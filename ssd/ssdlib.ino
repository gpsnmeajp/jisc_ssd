const int PIN_IO1 = 0;
const int PIN_IO2 = 1;
const int PIN_IO3 = 2;
const int PIN_IO4 = 3;
const int PIN_IO5 = 4;
const int PIN_IO6 = 5;
const int PIN_IO7 = 6;
const int PIN_IO8 = 7;
const int PIN_CEB0 = 8; //OUTPUT       Flash0 !ChipEnable (HIGH=無効. LOW=有効)
const int PIN_CEB1 = 9; //OUTPUT       Flash1 !ChipEnable (Not implemented)
const int PIN_CLE = 10; //OUTPUT       Command Latch Enable (HIGH=コマンドレジスタへの書き込み(WE立ち上がり時), LOW:無効)
const int PIN_ALE = 11; //OUTPUT       Address Latch Enable (HIGH=アドレスレジスタへの書き込み(WE立ち上がり時), LOW:無効)
const int PIN_WPB = 12; //OUTPUT       !Write Protect (HIGH=書込許可, LOW=書込禁止)
const int PIN_WEB = 13; //OUTPUT       !Write Enable (HIGH=待機, LOW=書込中, LOW→HIGH=データ確定)
const int PIN_REB = 14; //OUTPUT       !Read Enable (HIGH=待機, LOW=読込中, HIGH→LOW=データ確定)
const int PIN_RBB = 15; //INPUT_PULLUP Ready/!Busy (HIGH=Ready, LOW=Busy)

bool ssd_debug_log = false;

void io_set_output() {
  pinMode(PIN_IO1, OUTPUT);
  pinMode(PIN_IO2, OUTPUT);
  pinMode(PIN_IO3, OUTPUT);
  pinMode(PIN_IO4, OUTPUT);
  pinMode(PIN_IO5, OUTPUT);
  pinMode(PIN_IO6, OUTPUT);
  pinMode(PIN_IO7, OUTPUT);
  pinMode(PIN_IO8, OUTPUT);
}

void io_set_input() {
  pinMode(PIN_IO1, INPUT_PULLUP);
  pinMode(PIN_IO2, INPUT_PULLUP);
  pinMode(PIN_IO3, INPUT_PULLUP);
  pinMode(PIN_IO4, INPUT_PULLUP);
  pinMode(PIN_IO5, INPUT_PULLUP);
  pinMode(PIN_IO6, INPUT_PULLUP);
  pinMode(PIN_IO7, INPUT_PULLUP);
  pinMode(PIN_IO8, INPUT_PULLUP);
}

void io_write(byte d) {
  if (d & 1) {
    digitalWrite(PIN_IO1, HIGH);
  } else {
    digitalWrite(PIN_IO1, LOW);
  }
  if (d & 2) {
    digitalWrite(PIN_IO2, HIGH);
  } else {
    digitalWrite(PIN_IO2, LOW);
  }
  if (d & 4) {
    digitalWrite(PIN_IO3, HIGH);
  } else {
    digitalWrite(PIN_IO3, LOW);
  }
  if (d & 8) {
    digitalWrite(PIN_IO4, HIGH);
  } else {
    digitalWrite(PIN_IO4, LOW);
  }
  if (d & 16) {
    digitalWrite(PIN_IO5, HIGH);
  } else {
    digitalWrite(PIN_IO5, LOW);
  }
  if (d & 32) {
    digitalWrite(PIN_IO6, HIGH);
  } else {
    digitalWrite(PIN_IO6, LOW);
  }
  if (d & 64) {
    digitalWrite(PIN_IO7, HIGH);
  } else {
    digitalWrite(PIN_IO7, LOW);
  }
  if (d & 128) {
    digitalWrite(PIN_IO8, HIGH);
  } else {
    digitalWrite(PIN_IO8, LOW);
  }
}

byte io_read() {
  byte d = 0;
  if (digitalRead(PIN_IO1)) {
    d |= 1;
  }
  if (digitalRead(PIN_IO2)) {
    d |= 2;
  }
  if (digitalRead(PIN_IO3)) {
    d |= 4;
  }
  if (digitalRead(PIN_IO4)) {
    d |= 8;
  }
  if (digitalRead(PIN_IO5)) {
    d |= 16;
  }
  if (digitalRead(PIN_IO6)) {
    d |= 32;
  }
  if (digitalRead(PIN_IO7)) {
    d |= 64;
  }
  if (digitalRead(PIN_IO8)) {
    d |= 128;
  }
  return d;
}

void io_wait_busy(){
  while(!digitalRead(PIN_RBB)){
    if(ssd_debug_log){
      Serial.println("BUSY!");
    }
    delayMicroseconds(10);
  }
}

void ssd_init()
{
  if (ssd_debug_log) {
    Serial.println("$ssd_init");
  }

  io_set_input();
  pinMode(PIN_CEB0, OUTPUT);
  pinMode(PIN_CEB1, OUTPUT);
  pinMode(PIN_CLE, OUTPUT);
  pinMode(PIN_ALE, OUTPUT);
  pinMode(PIN_WPB, OUTPUT);
  pinMode(PIN_WEB, OUTPUT);
  pinMode(PIN_REB, OUTPUT);
  pinMode(PIN_RBB, INPUT_PULLUP);

  digitalWrite(PIN_CEB0, LOW);
  digitalWrite(PIN_CEB1, HIGH);
  digitalWrite(PIN_CLE, LOW);
  digitalWrite(PIN_ALE, LOW);
  digitalWrite(PIN_WPB, HIGH);
  digitalWrite(PIN_WEB, HIGH);
  digitalWrite(PIN_REB, HIGH);
}

void ssd_reset()
{
  if (ssd_debug_log) {
    Serial.println("$ssd_reset");
  }

  digitalWrite(PIN_CLE, HIGH);
  digitalWrite(PIN_ALE, LOW);

  digitalWrite(PIN_WEB, LOW);

  io_set_output();
  io_write(0xFF);

  digitalWrite(PIN_WEB, HIGH);
  digitalWrite(PIN_CLE, LOW);

  io_wait_busy();
}

void ssd_command_input(byte d)
{
  if (ssd_debug_log) {
    Serial.print("$ssd_command_input ");
    Serial.print(d, HEX);
    Serial.println();
  }

  digitalWrite(PIN_CLE, HIGH);
  digitalWrite(PIN_ALE, LOW);

  digitalWrite(PIN_WEB, LOW);

  io_set_output();
  io_write(d);

  digitalWrite(PIN_WEB, HIGH);
  digitalWrite(PIN_CLE, LOW);

  io_set_input();
  io_wait_busy();
}

void ssd_data_input(byte d)
{
  if (ssd_debug_log) {
    Serial.print("$ssd_data_input ");
    Serial.print(d, HEX);
    Serial.println();
  }

  digitalWrite(PIN_CLE, LOW);
  digitalWrite(PIN_ALE, LOW);

  digitalWrite(PIN_WEB, LOW);

  io_set_output();
  io_write(d);

  digitalWrite(PIN_WEB, HIGH);

  io_set_input();
  io_wait_busy();
}

void ssd_address4_input(byte ca0, byte ca8, byte pa0, byte pa8)
{
  if (ssd_debug_log) {
    Serial.print("$ssd_addresses_input ");
    Serial.print(ca0, HEX);
    Serial.print(" ");
    Serial.print(ca8, HEX);
    Serial.print(" ");
    Serial.print(pa0, HEX);
    Serial.print(" ");
    Serial.print(pa8, HEX);
    Serial.println();
  }
  digitalWrite(PIN_CLE, LOW);
  digitalWrite(PIN_ALE, HIGH);

  digitalWrite(PIN_WEB, LOW);
  io_set_output();
  io_write(ca0);
  digitalWrite(PIN_WEB, HIGH);

  digitalWrite(PIN_WEB, LOW);
  io_write(ca8);
  digitalWrite(PIN_WEB, HIGH);

  digitalWrite(PIN_WEB, LOW);
  io_write(pa0);
  digitalWrite(PIN_WEB, HIGH);

  digitalWrite(PIN_WEB, LOW);
  io_write(pa8);
  digitalWrite(PIN_WEB, HIGH);

  digitalWrite(PIN_ALE, LOW);

  io_set_input();
  io_wait_busy();
}

void ssd_address2_input(byte ca0, byte ca8)
{
  if (ssd_debug_log) {
    Serial.print("$ssd_addresses_input ");
    Serial.print(ca0, HEX);
    Serial.print(" ");
    Serial.print(ca8, HEX);
    Serial.println();
  }
  digitalWrite(PIN_CLE, LOW);
  digitalWrite(PIN_ALE, HIGH);

  digitalWrite(PIN_WEB, LOW);
  io_set_output();
  io_write(ca0);
  digitalWrite(PIN_WEB, HIGH);

  digitalWrite(PIN_WEB, LOW);
  io_write(ca8);
  digitalWrite(PIN_WEB, HIGH);

  digitalWrite(PIN_ALE, LOW);

  io_set_input();
  io_wait_busy();
}

void ssd_address_input(byte a)
{
  if (ssd_debug_log) {
    Serial.print("$ssd_address_input ");
    Serial.print(a, HEX);
    Serial.println();
  }
  digitalWrite(PIN_CLE, LOW);
  digitalWrite(PIN_ALE, HIGH);

  digitalWrite(PIN_WEB, LOW);
  io_set_output();
  io_write(a);
  digitalWrite(PIN_WEB, HIGH);
  digitalWrite(PIN_ALE, LOW);

  io_set_input();
  io_wait_busy();
}

byte ssd_serial_data_output()
{
  if (ssd_debug_log) {
    Serial.print("$ssd_serial_data_output ");
  }
  digitalWrite(PIN_CLE, LOW);
  digitalWrite(PIN_ALE, LOW);

  digitalWrite(PIN_REB, HIGH);
  io_set_input();
  digitalWrite(PIN_REB, LOW);
  byte d = io_read();
  digitalWrite(PIN_REB, HIGH);

  if (ssd_debug_log) {
    Serial.print(d, HEX);
    Serial.println();
  }
  io_wait_busy();
  return d;
}
