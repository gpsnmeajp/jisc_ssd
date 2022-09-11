#include "jisc_ssd.h"
//https://raspberrypi.github.io/pico-sdk-doxygen/group__hardware__gpio.html

void jisc_ssd_lowlevel_io_init() {
  gpio_init_mask(0xFFFF); //0-15pin
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
  
}

/*
Flash0 !ChipEnable (1=無効. 0=有効)
Flash1 !ChipEnable (Not implemented)
*/
inline void jisc_ssd_lowlevel_io_set_ce(bool ce0, bool ce1)
{
  gpio_put(PIN_CEB0, ce0);
  gpio_put(PIN_CEB1, ce1);
}

/*
Command Latch Enable (1=コマンドレジスタへの書き込み(WE立ち上がり時), 0:無効)
Address Latch Enable (1=アドレスレジスタへの書き込み(WE立ち上がり時), 0:無効)
*/
inline void jisc_ssd_lowlevel_io_set_latch_command_address(bool cle, bool ale)
{
  gpio_put(PIN_CLE, cle);
  gpio_put(PIN_ALE, ale);  
}

/*
!Write Protect (1=書込許可, 0=書込禁止)
*/
inline void jisc_ssd_lowlevel_io_set_write_protect(bool wp)
{
  gpio_put(PIN_WPB, wp);
}

/*
!Write Enable (1=待機, 0=書込中, 0→1=データ確定)
*/
inline void jisc_ssd_lowlevel_io_set_write_enable(bool we)
{
  gpio_put(PIN_WEB, we);
}

/*
!Read Enable (1=待機, 0=読込中, 1→0=データ確定)
*/
inline void jisc_ssd_lowlevel_io_set_read_enable(bool re)
{
  gpio_put(PIN_REB, re);
}


/*
INPUT_PULLUP Ready/!Busy (1=Ready, 0=Busy)
*/
inline bool jisc_ssd_lowlevel_io_get_busy()
{
    return gpio_get(PIN_RBB);
}

//データ線を出力モードにする
inline void jisc_ssd_lowlevel_io_set_output() {
  gpio_set_dir_out_masked(0x00FF);
}

//データ線を入力モードにする
inline void jisc_ssd_lowlevel_io_set_input() {
  gpio_set_dir_in_masked(0x00FF);
}

//データ線に8bit出力する
inline void jisc_ssd_lowlevel_io_write(byte d) {
  gpio_put_masked(0xFF, d);
}

//データ線から8bit入力する
inline byte jisc_ssd_lowlevel_io_read() {
  return (byte)(gpio_get_all() & 0xFF);
}

//ビジーを待つ
inline void jisc_ssd_lowlevel_io_wait_busy(){
  while(!jisc_ssd_lowlevel_io_get_busy()){
    //Serial.print("B");
    jisc_ssd_lowlevel_io_delay_nano100();
  }
}

//100ナノ秒待つ
inline void jisc_ssd_lowlevel_io_delay_nano100(){
    busy_wait_at_least_cycles(101);
    /*
    //だいたい104nsくらいになるはず(テスターの周波数カウンタで計測)
    asm volatile("nop\n");
    asm volatile("nop\n");
    asm volatile("nop\n");
    asm volatile("nop\n");
    asm volatile("nop\n");
    asm volatile("nop\n");
    asm volatile("nop\n");
    asm volatile("nop\n");
    asm volatile("nop\n");
    asm volatile("nop\n");
    asm volatile("nop\n");
    */
}

//30ナノ秒待つ
inline void jisc_ssd_lowlevel_io_delay_nano30(){
    busy_wait_at_least_cycles(30);
}
