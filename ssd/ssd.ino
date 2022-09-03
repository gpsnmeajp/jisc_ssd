const int PAGE_SIZE = 2176;
byte buff[PAGE_SIZE] = {0};

//1page = 2176byte
//1block = 128k+8k bytes (64pages)
//Capacity = 2176 * 64 * 1024 byte = 136MB

void read_id() {
  Serial.println(" --- read_id ---");
  ssd_command_input(0x90);
  ssd_address_input(0x00);

  int x1 = ssd_serial_data_output();
  int x2 = ssd_serial_data_output();
  int x3 = ssd_serial_data_output();
  int x4 = ssd_serial_data_output();
  int x5 = ssd_serial_data_output();

  Serial.print("Maker Code: ");
  Serial.print(x1, HEX);
  Serial.println();

  Serial.print("Device Code: ");
  Serial.print(x2, HEX);
  Serial.println();
  Serial.print("Chip Number, Cell Type: ");
  Serial.print(x3, HEX);
  Serial.println();
  Serial.print(" Chip Number: ");
  Serial.print(1 << (x3 & 3));
  Serial.println();
  Serial.print(" Cell Type: ");
  Serial.print(2 << ((x3 >> 2) & 3));
  Serial.println();

  Serial.print("Page Size, Block Size: ");
  Serial.print(x4, HEX);
  Serial.println();
  Serial.print(" Page Size: ");
  Serial.print(1 << (x4 & 3));
  Serial.println();
  Serial.print(" Block Size: ");
  Serial.print(64 << ((x4 >> 4) & 3));
  Serial.println();
  Serial.print(" I/O Width: ");
  Serial.print(8 << ((x4 >> 6) & 1));
  Serial.println();

  Serial.print("District Number: ");
  Serial.print(x5, HEX);
  Serial.println();
  Serial.print(" District Number: ");
  Serial.print(1 << ((x5 >> 2) & 3));
  Serial.println();
}

void read_status() {
  Serial.println(" --- read_status ---");
  ssd_command_input(0x70);

  int x1 = ssd_serial_data_output();

  Serial.print("Status :");
  Serial.print(x1, HEX);
  Serial.println();

  if ((x1 & 1) == 0) {
    Serial.println(" Chip Status1: Pass");
  } else {
    Serial.println(" Chip Status1: Fail");
  }
  if ((x1 & 2) == 0) {
    Serial.println(" Chip Status2: Pass");
  } else {
    Serial.println(" Chip Status2: Fail");
  }
  if (x1 & 32) {
    Serial.println(" Page Buffer Ready/Budy: Ready");
  } else {
    Serial.println(" Page Buffer Ready/Budy: Busy");
  }
  if (x1 & 64) {
    Serial.println(" Data Cache Ready/Budy: Ready");
  } else {
    Serial.println(" Data Cache Ready/Budy: Busy");
  }
  if (x1 & 128) {
    Serial.println(" Write Protect: Not Protected");
  } else {
    Serial.println(" Write Protect: Protected");
  }
}

void read_page(int page)
{
  Serial.println(" --- read_page ---");
  ssd_command_input(0x00);

  //カラムアドレスはページ内オフセットと思えば良い
  ssd_address4_input(0x00, 0x00, page & 0xFF, ((page >> 8) & 0xFF));
  ssd_command_input(0x30);

  for (int i = 0; i < PAGE_SIZE; i++) {
    buff[i] = ssd_serial_data_output();
  }
  for (int i = 0; i < PAGE_SIZE; i++) {
    Serial.print(buff[i], HEX);
    Serial.print(" ");
  }
  Serial.println(" ");
}

void write_page(int page)
{
  Serial.println(" --- write_page ---");
  ssd_command_input(0x80);
  ssd_address4_input(0x00, 0x00, page & 0xFF, ((page >> 8) & 0xFF));

  for (int i = 0; i < PAGE_SIZE; i++) {
    //ssd_data_input(page[i]);
    ssd_data_input((byte)i); //お試し連番
  }
  ssd_command_input(0x10);
  ssd_command_input(0x10);
}

void erase_page(int page)
{
  Serial.println(" --- erase_page ---");
  ssd_command_input(0x60);
  ssd_address2_input(page & 0xFF, ((page >> 8) & 0xFF)); //下位6bitに意味はない？
  ssd_command_input(0xD0);
}

void erase_all()
{
  Serial.println(" --- erase_all ---");
  for (int block = 0; block < 1024; block++) {
    int page = block * 64;
    ssd_command_input(0x60);
    ssd_address2_input(page & 0xFF, ((page >> 8) & 0xFF)); //下位6bitに意味はない？
    ssd_command_input(0xD0);
  }
}

//初期状態 or 全erase済みのときだけ有効
void bad_block_test() {
  for (int block = 0; block < 1024; block++) {
    bool bad = false;
    int page = block * 64;
    ssd_command_input(0x00);
    ssd_address4_input(0x00, 0x00, page & 0xFF, ((page >> 8) & 0xFF));
    ssd_command_input(0x30);
    if (ssd_serial_data_output() == 0) {
      bad = true;
    }
    if (bad) {
      Serial.print("BAD BLOCK: ");
      Serial.print(page);
      Serial.print(" - ");
      Serial.print(page + 63);
      Serial.print(" (");
      Serial.print(block);
      Serial.print(")");
      Serial.println();
    }
  }
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  ssd_init();
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
  ssd_reset();
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
