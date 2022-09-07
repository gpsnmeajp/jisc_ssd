#ifndef JISC_SSD
#define JISC_SSD
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

const bool jisc_ssd_debug_log = false;

const int jisc_ssd_blocks = 1024;
const int jisc_ssd_page_size = 2176;
const int jisc_ssd_pages_of_block = 64;
const int jisc_ssd_block_size = jisc_ssd_page_size*jisc_ssd_pages_of_block;
byte jisc_ssd_block_buffer[jisc_ssd_block_size] = {0};

#endif //JISC_SSD