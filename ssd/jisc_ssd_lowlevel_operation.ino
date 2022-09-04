#include "jisc_ssd.h"

void jisc_ssd_lowlevel_operation_init()
{
  if (jisc_ssd_debug_log) {
    Serial.println("$jisc_ssd_lowlevel_operation_init");
  }

  jisc_ssd_lowlevel_io_init();
}

void jisc_ssd_lowlevel_operation_reset()
{
  if (jisc_ssd_debug_log) {
    Serial.println("$jisc_ssd_lowlevel_operation_reset");
  }

  jisc_ssd_lowlevel_operation_command_input(0xFF);
}

void jisc_ssd_lowlevel_operation_command_input(byte d)
{
  if (jisc_ssd_debug_log) {
    Serial.print("$jisc_ssd_lowlevel_operation_input ");
    Serial.print(d, HEX);
    Serial.println();
  }

  //準備
  jisc_ssd_lowlevel_io_set_latch_command_address(1,0);
  jisc_ssd_lowlevel_io_set_read_enable(1);

  //データを出力
  jisc_ssd_lowlevel_io_set_output();
  jisc_ssd_lowlevel_io_write(d);

  //データ確定
  jisc_ssd_lowlevel_io_set_write_enable(0);
  jisc_ssd_lowlevel_io_set_write_enable(1);
  jisc_ssd_lowlevel_io_delay_nano100();  

  //後始末
  jisc_ssd_lowlevel_io_set_input();
  jisc_ssd_lowlevel_io_set_latch_command_address(0,0);

  //ビジー待機
  jisc_ssd_lowlevel_io_wait_busy();
}

void jisc_ssd_lowlevel_operation_data_input(byte d)
{
  if (jisc_ssd_debug_log) {
    Serial.print("$jisc_ssd_lowlevel_operation_data_input ");
    Serial.print(d, HEX);
    Serial.println();
  }

  //準備
  jisc_ssd_lowlevel_io_set_latch_command_address(0,0);
  jisc_ssd_lowlevel_io_set_read_enable(1);

  //データを出力
  jisc_ssd_lowlevel_io_set_output();
  jisc_ssd_lowlevel_io_write(d);

  //データ確定
  jisc_ssd_lowlevel_io_set_write_enable(0);
  jisc_ssd_lowlevel_io_set_write_enable(1);
  jisc_ssd_lowlevel_io_delay_nano100();  

  //後始末
  jisc_ssd_lowlevel_io_set_input();

  //ビジー待機
  jisc_ssd_lowlevel_io_wait_busy();
}

//1ページ分まるごと送りつける
void jisc_ssd_lowlevel_operation_data_input_page_from_block_buffer(const byte *page_buffer)
{
  if (jisc_ssd_debug_log) {
    Serial.print("$jisc_ssd_lowlevel_operation_data_input_page_from_block_buffer");
    Serial.println();
  }

  //準備
  jisc_ssd_lowlevel_io_set_latch_command_address(0,0);
  jisc_ssd_lowlevel_io_set_read_enable(1);

  for(int i=0;i<jisc_ssd_page_size;i++)
  {
    //データを出力
    jisc_ssd_lowlevel_io_set_output();
    jisc_ssd_lowlevel_io_write(page_buffer[i]);

    //データ確定
    jisc_ssd_lowlevel_io_set_write_enable(0);
    jisc_ssd_lowlevel_io_set_write_enable(1);
    jisc_ssd_lowlevel_io_delay_nano100();  
  }
  jisc_ssd_lowlevel_io_delay_nano100();  

  //後始末
  jisc_ssd_lowlevel_io_set_input();

  //ビジー待機
  jisc_ssd_lowlevel_io_wait_busy();
}


void jisc_ssd_lowlevel_operation_address4_input(byte ca0, byte ca8, byte pa0, byte pa8)
{
  if (jisc_ssd_debug_log) {
    Serial.print("$jisc_ssd_lowlevel_operation_address4_input ");
    Serial.print(ca0, HEX);
    Serial.print(" ");
    Serial.print(ca8, HEX);
    Serial.print(" ");
    Serial.print(pa0, HEX);
    Serial.print(" ");
    Serial.print(pa8, HEX);
    Serial.println();
  }

  //準備
  jisc_ssd_lowlevel_io_set_latch_command_address(0,1);
  jisc_ssd_lowlevel_io_set_read_enable(1);
  jisc_ssd_lowlevel_io_set_output();

  //データを出力
  jisc_ssd_lowlevel_io_write(ca0);
  jisc_ssd_lowlevel_io_set_write_enable(0);
  jisc_ssd_lowlevel_io_set_write_enable(1);
  jisc_ssd_lowlevel_io_delay_nano100();  

  //データを出力
  jisc_ssd_lowlevel_io_write(ca8);
  jisc_ssd_lowlevel_io_set_write_enable(0);
  jisc_ssd_lowlevel_io_set_write_enable(1);
  jisc_ssd_lowlevel_io_delay_nano100();  

  //データを出力
  jisc_ssd_lowlevel_io_write(pa0);
  jisc_ssd_lowlevel_io_set_write_enable(0);
  jisc_ssd_lowlevel_io_set_write_enable(1);
  jisc_ssd_lowlevel_io_delay_nano100();  

  //データを出力
  jisc_ssd_lowlevel_io_write(pa8);
  jisc_ssd_lowlevel_io_set_write_enable(0);
  jisc_ssd_lowlevel_io_set_write_enable(1);
  jisc_ssd_lowlevel_io_delay_nano100();  

  //後始末
  jisc_ssd_lowlevel_io_set_input();
  jisc_ssd_lowlevel_io_set_latch_command_address(0,0);

  //ビジー待機
  jisc_ssd_lowlevel_io_wait_busy();
}

void jisc_ssd_lowlevel_operation_address2_input(byte pa0, byte pa8)
{
  if (jisc_ssd_debug_log) {
    Serial.print("$jisc_ssd_lowlevel_operation_address2_input ");
    Serial.print(pa0, HEX);
    Serial.print(" ");
    Serial.print(pa8, HEX);
    Serial.println();
  }
  //準備
  jisc_ssd_lowlevel_io_set_latch_command_address(0,1);
  jisc_ssd_lowlevel_io_set_read_enable(1);
  jisc_ssd_lowlevel_io_set_output();

  //データを出力
  jisc_ssd_lowlevel_io_write(pa0);
  jisc_ssd_lowlevel_io_set_write_enable(0);
  jisc_ssd_lowlevel_io_set_write_enable(1);
  jisc_ssd_lowlevel_io_delay_nano100();  

  //データを出力
  jisc_ssd_lowlevel_io_write(pa8);
  jisc_ssd_lowlevel_io_set_write_enable(0);
  jisc_ssd_lowlevel_io_set_write_enable(1);
  jisc_ssd_lowlevel_io_delay_nano100();  

  //後始末
  jisc_ssd_lowlevel_io_set_input();
  jisc_ssd_lowlevel_io_set_latch_command_address(0,0);

  //ビジー待機
  jisc_ssd_lowlevel_io_wait_busy();
}

void jisc_ssd_lowlevel_operation_address_input(byte a0)
{
  if (jisc_ssd_debug_log) {
    Serial.print("$jisc_ssd_lowlevel_operation_address_input ");
    Serial.print(a0, HEX);
    Serial.println();
  }
  //準備
  jisc_ssd_lowlevel_io_set_latch_command_address(0,1);
  jisc_ssd_lowlevel_io_set_read_enable(1);
  jisc_ssd_lowlevel_io_set_output();

  //データを出力
  jisc_ssd_lowlevel_io_write(a0);
  jisc_ssd_lowlevel_io_set_write_enable(0);
  jisc_ssd_lowlevel_io_set_write_enable(1);
  jisc_ssd_lowlevel_io_delay_nano100();  

  //後始末
  jisc_ssd_lowlevel_io_set_input();
  jisc_ssd_lowlevel_io_set_latch_command_address(0,0);

  //ビジー待機
  jisc_ssd_lowlevel_io_wait_busy();
}

byte jisc_ssd_lowlevel_operation_serial_data_output()
{
  if (jisc_ssd_debug_log) {
    Serial.print("$jisc_ssd_lowlevel_operation_serial_data_output ");
  }

  //準備
  jisc_ssd_lowlevel_io_set_latch_command_address(0,0);
  jisc_ssd_lowlevel_io_set_write_enable(1);
  jisc_ssd_lowlevel_io_set_input();

  //データを入力
  jisc_ssd_lowlevel_io_set_read_enable(1);
  jisc_ssd_lowlevel_io_set_read_enable(0);
  jisc_ssd_lowlevel_io_delay_nano100();  
  byte d = jisc_ssd_lowlevel_io_read();

  if (jisc_ssd_debug_log) {
    Serial.print(d, HEX);
    Serial.println();
  }
  //後始末
  jisc_ssd_lowlevel_io_set_read_enable(1);

  //ビジー待機
  jisc_ssd_lowlevel_io_wait_busy();
  return d;
}

//1ページ分まるごと受け付ける
void jisc_ssd_lowlevel_operation_serial_data_output_page_to_block_buffer(byte *page_buffer)
{
  if (jisc_ssd_debug_log) {
    Serial.print("$jisc_ssd_lowlevel_operation_serial_data_output_page_to_block_buffer ");
  }

  //準備
  jisc_ssd_lowlevel_io_set_latch_command_address(0,0);
  jisc_ssd_lowlevel_io_set_write_enable(1);
  jisc_ssd_lowlevel_io_set_input();

  for(int i=0;i<jisc_ssd_page_size;i++)
  {
    //データを入力
    jisc_ssd_lowlevel_io_set_read_enable(1);
    jisc_ssd_lowlevel_io_set_read_enable(0);
    jisc_ssd_lowlevel_io_delay_nano100();  
    page_buffer[i] = jisc_ssd_lowlevel_io_read();
  }

  //後始末
  jisc_ssd_lowlevel_io_set_read_enable(1);

  //ビジー待機
  jisc_ssd_lowlevel_io_wait_busy();
  return;
}
