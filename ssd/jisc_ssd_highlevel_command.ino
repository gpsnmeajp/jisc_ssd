#include "jisc_ssd.h"

const int PAGE_SIZE = 2176;
byte buff[PAGE_SIZE] = {0};

//1page = 2176byte
//1block = 128k+8k bytes (64pages)
//Capacity = 2176 * 64 * 1024 byte = 136MB

void read_id() {
  Serial.println(" --- read_id ---");
  jisc_ssd_lowlevel_operation_command_input(0x90);
  jisc_ssd_lowlevel_operation_address_input(0x00);

  int x1 = jisc_ssd_lowlevel_operation_serial_data_output();
  int x2 = jisc_ssd_lowlevel_operation_serial_data_output();
  int x3 = jisc_ssd_lowlevel_operation_serial_data_output();
  int x4 = jisc_ssd_lowlevel_operation_serial_data_output();
  int x5 = jisc_ssd_lowlevel_operation_serial_data_output();

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
  jisc_ssd_lowlevel_operation_command_input(0x70);

  int x1 = jisc_ssd_lowlevel_operation_serial_data_output();

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
  //Serial.println(" --- read_page ---");
  jisc_ssd_lowlevel_operation_command_input(0x00);

  //カラムアドレスはページ内オフセットと思えば良い
  jisc_ssd_lowlevel_operation_address4_input(0x00, 0x00, page & 0xFF, ((page >> 8) & 0xFF));
  jisc_ssd_lowlevel_operation_command_input(0x30);
  jisc_ssd_lowlevel_operation_serial_data_output_page_to_block_buffer(jisc_ssd_block_buffer);

  Serial.print(page);
  Serial.print(":");
  Serial.println(jisc_ssd_block_buffer[0], HEX);

  for (int i = 0; i < jisc_ssd_page_size; i++) {
    Serial.print(jisc_ssd_block_buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println(" ");

}

void write_page(int page)
{
  Serial.println(" --- write_page ---");
  jisc_ssd_lowlevel_operation_command_input(0x80);
  jisc_ssd_lowlevel_operation_address4_input(0x00, 0x00, page & 0xFF, ((page >> 8) & 0xFF));

  jisc_ssd_lowlevel_operation_data_input_page_from_block_buffer(jisc_ssd_block_buffer);
/*
  for (int i = 0; i < PAGE_SIZE; i++) {
    //jisc_ssd_lowlevel_data_input(page[i]);
    jisc_ssd_lowlevel_operation_data_input((byte)i); //お試し連番
  }
*/
  jisc_ssd_lowlevel_operation_command_input(0x10);
  jisc_ssd_lowlevel_operation_command_input(0x10);
}

void erase_page(int page)
{
  Serial.println(" --- erase_page ---");
  jisc_ssd_lowlevel_operation_command_input(0x60);
  jisc_ssd_lowlevel_operation_address2_input(page & 0xFF, ((page >> 8) & 0xFF)); //下位6bitに意味はない？
  jisc_ssd_lowlevel_operation_command_input(0xD0);
}

void erase_all()
{
  Serial.println(" --- erase_all ---");
  for (int block = 0; block < 1024; block++) {
    int page = block * 64;
    jisc_ssd_lowlevel_operation_command_input(0x60);
    jisc_ssd_lowlevel_operation_address2_input(page & 0xFF, ((page >> 8) & 0xFF)); //下位6bitに意味はない？
    jisc_ssd_lowlevel_operation_command_input(0xD0);
  }
}

//初期状態 or 全erase済みのときだけ有効
void bad_block_test() {
  for (int block = 0; block < 1024; block++) {
    bool bad = false;
    int page = block * 64;
    jisc_ssd_lowlevel_operation_command_input(0x00);
    jisc_ssd_lowlevel_operation_address4_input(0x00, 0x00, page & 0xFF, ((page >> 8) & 0xFF));
    jisc_ssd_lowlevel_operation_command_input(0x30);
    if (jisc_ssd_lowlevel_operation_serial_data_output() == 0) {
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
