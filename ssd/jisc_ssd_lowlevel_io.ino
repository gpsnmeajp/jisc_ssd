#include "jisc_ssd.h"
//https://raspberrypi.github.io/pico-sdk-doxygen/group__hardware__gpio.html

void jisc_ssd_lowlevel_io_init() {
  gpio_init_mask(0xFF);
  jisc_ssd_lowlevel_io_set_input();
  gpio_set_dir(PIN_CEB0, true); //OUTPUT
  gpio_set_dir(PIN_CEB1, true);
  gpio_set_dir(PIN_CLE, true);
  gpio_set_dir(PIN_ALE, true);
  gpio_set_dir(PIN_WPB, true);
  gpio_set_dir(PIN_WEB, true);
  gpio_set_dir(PIN_REB, true);

  gpio_set_dir(PIN_RBB, false); //INPUT
  gpio_set_pulls(PIN_RBB, true, false);

  jisc_ssd_lowlevel_io_set_ce(0,1);
  jisc_ssd_lowlevel_io_set_latch_command_address(0,0);
  jisc_ssd_lowlevel_io_set_write_protect(1);
  jisc_ssd_lowlevel_io_set_write_enable(1);
  jisc_ssd_lowlevel_io_set_read_enable(1);
  delayMicroseconds(10);
}

/*
Flash0 !ChipEnable (1=無効. 0=有効)
Flash1 !ChipEnable (Not implemented)
*/
void jisc_ssd_lowlevel_io_set_ce(bool ce0, bool ce1)
{
  gpio_put(PIN_CEB0, ce0);
  gpio_put(PIN_CEB1, ce1);
  delayMicroseconds(10);
}

/*
Command Latch Enable (1=コマンドレジスタへの書き込み(WE立ち上がり時), 0:無効)
Address Latch Enable (1=アドレスレジスタへの書き込み(WE立ち上がり時), 0:無効)
*/
void jisc_ssd_lowlevel_io_set_latch_command_address(bool cle, bool ale)
{
  gpio_put(PIN_CLE, cle);
  gpio_put(PIN_ALE, ale);
  delayMicroseconds(10);
}

/*
!Write Protect (1=書込許可, 0=書込禁止)
*/
void jisc_ssd_lowlevel_io_set_write_protect(bool wp)
{
  gpio_put(PIN_WPB, wp);
  delayMicroseconds(10);
}

/*
!Write Enable (1=待機, 0=書込中, 0→1=データ確定)
*/
void jisc_ssd_lowlevel_io_set_write_enable(bool we)
{
  gpio_put(PIN_WEB, we);
  delayMicroseconds(10);
}

/*
!Read Enable (1=待機, 0=読込中, 1→0=データ確定)
*/
void jisc_ssd_lowlevel_io_set_read_enable(bool re)
{
  gpio_put(PIN_REB, re);
  delayMicroseconds(10);
}


/*
INPUT_PULLUP Ready/!Busy (1=Ready, 0=Busy)
*/
bool jisc_ssd_lowlevel_io_get_busy()
{
    return gpio_get(PIN_RBB);
}

//データ線を出力モードにする
void jisc_ssd_lowlevel_io_set_output() {
  gpio_set_dir_out_masked(0x00FF);
  delayMicroseconds(10);
}

//データ線を入力モードにする
void jisc_ssd_lowlevel_io_set_input() {
  gpio_set_dir_in_masked(0x00FF);
  delayMicroseconds(10);
}

//データ線に8bit出力する
void jisc_ssd_lowlevel_io_write(byte d) {
  gpio_put_masked(0xFF, d);
  delayMicroseconds(10);
}

//データ線から8bit入力する
byte jisc_ssd_lowlevel_io_read() {
  delayMicroseconds(10);
  return (byte)(gpio_get_all() & 0xFF);
}

//ビジーを待つ
void jisc_ssd_lowlevel_io_wait_busy(){
  delayMicroseconds(10);
  while(!gpio_get(PIN_RBB)){
    delayMicroseconds(10);
  }
}
