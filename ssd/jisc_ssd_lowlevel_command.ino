#include "jisc_ssd.h"

void jisc_ssd_lowlevel_command_init()
{
  if (ssd_debug_log) {
    Serial.println("$jisc_ssd_lowlevel_command_init");
  }

  jisc_ssd_lowlevel_io_init();
  delay(10);
}

void jisc_ssd_lowlevel_command_reset()
{
  if (ssd_debug_log) {
    Serial.println("$jisc_ssd_lowlevel_command_reset");
  }

  jisc_ssd_lowlevel_command_command_input(0xFF);
  delay(10);
}

void jisc_ssd_lowlevel_command_command_input(byte d)
{
  if (ssd_debug_log) {
    Serial.print("$jisc_ssd_lowlevel_command_input ");
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

  //後始末
  jisc_ssd_lowlevel_io_set_input();
  jisc_ssd_lowlevel_io_set_latch_command_address(0,0);

  //ビジー待機
  jisc_ssd_lowlevel_io_wait_busy();
}

void jisc_ssd_lowlevel_command_data_input(byte d)
{
  if (ssd_debug_log) {
    Serial.print("$jisc_ssd_lowlevel_command_data_input ");
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

  //後始末
  jisc_ssd_lowlevel_io_set_input();
  jisc_ssd_lowlevel_io_set_latch_command_address(0,0);

  //ビジー待機
  jisc_ssd_lowlevel_io_wait_busy();
}

void jisc_ssd_lowlevel_command_address4_input(byte ca0, byte ca8, byte pa0, byte pa8)
{
  if (ssd_debug_log) {
    Serial.print("$jisc_ssd_lowlevel_command_address4_input ");
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

  //データを出力
  jisc_ssd_lowlevel_io_write(ca8);
  jisc_ssd_lowlevel_io_set_write_enable(0);
  jisc_ssd_lowlevel_io_set_write_enable(1);

  //データを出力
  jisc_ssd_lowlevel_io_write(pa0);
  jisc_ssd_lowlevel_io_set_write_enable(0);
  jisc_ssd_lowlevel_io_set_write_enable(1);

  //データを出力
  jisc_ssd_lowlevel_io_write(pa8);
  jisc_ssd_lowlevel_io_set_write_enable(0);
  jisc_ssd_lowlevel_io_set_write_enable(1);

  //後始末
  jisc_ssd_lowlevel_io_set_input();
  jisc_ssd_lowlevel_io_set_latch_command_address(0,0);

  //ビジー待機
  jisc_ssd_lowlevel_io_wait_busy();
}

void jisc_ssd_lowlevel_command_address2_input(byte pa0, byte pa8)
{
  if (ssd_debug_log) {
    Serial.print("$jisc_ssd_lowlevel_command_address2_input ");
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

  //データを出力
  jisc_ssd_lowlevel_io_write(pa8);
  jisc_ssd_lowlevel_io_set_write_enable(0);
  jisc_ssd_lowlevel_io_set_write_enable(1);

  //後始末
  jisc_ssd_lowlevel_io_set_input();
  jisc_ssd_lowlevel_io_set_latch_command_address(0,0);

  //ビジー待機
  jisc_ssd_lowlevel_io_wait_busy();
}

void jisc_ssd_lowlevel_command_address_input(byte a0)
{
  if (ssd_debug_log) {
    Serial.print("$jisc_ssd_lowlevel_command_address_input ");
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

  //後始末
  jisc_ssd_lowlevel_io_set_input();
  jisc_ssd_lowlevel_io_set_latch_command_address(0,0);

  //ビジー待機
  jisc_ssd_lowlevel_io_wait_busy();
}

byte jisc_ssd_lowlevel_command_serial_data_output()
{
  if (ssd_debug_log) {
    Serial.print("$jisc_ssd_lowlevel_command_serial_data_output ");
  }

  //準備
  jisc_ssd_lowlevel_io_set_latch_command_address(0,0);
  jisc_ssd_lowlevel_io_set_write_enable(1);
  jisc_ssd_lowlevel_io_set_input();

  //データを入力
  jisc_ssd_lowlevel_io_set_read_enable(1);
  jisc_ssd_lowlevel_io_set_read_enable(0);
  byte d = jisc_ssd_lowlevel_io_read();

  if (ssd_debug_log) {
    Serial.print(d, HEX);
    Serial.println();
  }
  //後始末
  jisc_ssd_lowlevel_io_set_read_enable(1);
  jisc_ssd_lowlevel_io_set_latch_command_address(0,0);

  //ビジー待機
  jisc_ssd_lowlevel_io_wait_busy();
  return d;
}
