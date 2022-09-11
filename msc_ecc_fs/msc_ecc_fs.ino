#include "Adafruit_TinyUSB.h"
#include "jisc_ssd.h"
#include "./src/pff3a/pff.h"

//USB StackをAdafruit_TinyUSBにしてください

Adafruit_USBD_MSC usb_msc;
int target_block = -1;
int target_block_fs = -1;
bool haveBufferData = false;

FATFS fatfs;

void accessLed()
{
  /*
  static bool led = 0;
  digitalWrite(PIN_LED, led);
  if(led){
    led = 0;
  }else{
    led = 1;
  }
  */
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  //Serial.begin(115200);

  jisc_ssd_lowlevel_operation_init();
  jisc_ssd_lowlevel_operation_reset();

  usb_msc.setID("Afafruit","External Flash", "1.0");
  usb_msc.setReadWriteCallback(msc_read, msc_write, msc_flush);
  usb_msc.setCapacity(1000*64*2,512);//24ブロックは予備
  usb_msc.setUnitReady(true);
  usb_msc.begin();

  //Serial接続待ち
  /*
  while (!Serial) {
    digitalWrite(PIN_LED, LOW);
    delay(100);
    digitalWrite(PIN_LED, HIGH);
    delay(100);
  }
  */
}

int32_t msc_read(uint32_t lba, void *buffer, uint32_t bufsize){
  accessLed();
  //LBA = Logical Block Address
  //ここではLBA = pageの1/2としている
  int page = bad_block_replace(lba/2);
  int in_page = page%jisc_ssd_pages_of_block;
  int block = page / jisc_ssd_pages_of_block;
/*
  Serial.print("lba:");
  Serial.print(lba);
  Serial.print(" bufsize:");
  Serial.print(bufsize);
  Serial.println();
  Serial.print("target_block:");
  Serial.print(target_block);
  Serial.print(" block:");
  Serial.print(block);
  Serial.print(" page:");
  Serial.print(page);
  Serial.print(" in_page:");
  Serial.print(in_page);
  Serial.println();
*/
  if(target_block != block){
    if(target_block >= 0 && haveBufferData){
      //現在保持しているブロックの内容を書き込み
      if(jisc_ssd_block_buffer[jisc_ssd_block_size-1] != 0xFF){
        eraseBlockOp(target_block);
      }
      jisc_ssd_block_buffer[jisc_ssd_block_size-1] = 0xAA; //使用済みブロックマーキング
      writeBlockOp(target_block);
    }

    target_block = block;
    haveBufferData = false;

    //クリア
    //memset(jisc_ssd_block_buffer,0xFF,jisc_ssd_block_size);

    //ブロック読み出し
    readBlockOp(block);
  }
  memcpy(buffer, &(jisc_ssd_block_buffer[(jisc_ssd_page_size * in_page) + (512*(lba%2))]), bufsize);
  return bufsize;
}

int32_t msc_write(uint32_t lba, uint8_t *buffer, uint32_t bufsize){
  accessLed();
  int page = bad_block_replace(lba/2);
  int in_page = page % jisc_ssd_pages_of_block;
  int block = page / jisc_ssd_pages_of_block;
/*
  Serial.print("lba:");
  Serial.print(lba);
  Serial.print(" bufsize:");
  Serial.print(bufsize);
  Serial.println();
  Serial.print("target_block:");
  Serial.print(target_block);
  Serial.print(" block:");
  Serial.print(block);
  Serial.print(" page:");
  Serial.print(page);
  Serial.print(" in_page:");
  Serial.print(in_page);
  Serial.println();
*/
  if(target_block != block){
    if(target_block >= 0 && haveBufferData){
      //現在保持しているブロックの内容を書き込み
      if(jisc_ssd_block_buffer[jisc_ssd_block_size-1] != 0xFF){
        eraseBlockOp(target_block);
      }
      jisc_ssd_block_buffer[jisc_ssd_block_size-1] = 0xAA; //使用済みブロックマーキング
      writeBlockOp(target_block);
    }

    target_block = block;
    haveBufferData = false;

    //クリア
    //memset(jisc_ssd_block_buffer,0xFF,jisc_ssd_block_size);

    //ブロック読み出し
    readBlockOp(block);
  }
  memcpy(&(jisc_ssd_block_buffer[(jisc_ssd_page_size * in_page) + (512*(lba%2))]), buffer, bufsize);
  haveBufferData = true;
  return bufsize;
}

void msc_flush()
{
    if(target_block >= 0){
      //現在保持しているブロックの内容を書き込み
      if(jisc_ssd_block_buffer[jisc_ssd_block_size-1] != 0xFF){
        eraseBlockOp(target_block);
      }
      jisc_ssd_block_buffer[jisc_ssd_block_size-1] = 0xAA; //使用済みブロックマーキング
      writeBlockOp(target_block);
    }

    target_block = -1;
}

int bad_block_replace(int page){
  int block = page / jisc_ssd_pages_of_block;
  switch(block){
    case 93:
      page = page + ((-93 + 1000) * jisc_ssd_pages_of_block);
      break;
    case 768:
      page = page + ((-768 + 1001) * jisc_ssd_pages_of_block);
      break;
    default:
      break;
  }
  return page;
}


void loop() {
  delay(500);
  if(pf_mount(&fatfs) == 0){
    if(pf_open("led.txt") == 0){
      pf_lseek(0);
      BYTE buff[16] = {0};
      UINT count;
      pf_read(buff,16,&count);
      digitalWrite(PIN_LED, buff[0] == '1');
    }
  }
}

bool checkErrorStatus(){
  jisc_ssd_lowlevel_operation_command_input(0x70);
  int x1 = jisc_ssd_lowlevel_operation_serial_data_output();
  if ((x1 & 1) != 0) {
    return true;
  }
  return false;
}

void writeBlockOp(int block){
  int offset = jisc_ssd_pages_of_block * block;
  for(int i=0;i<jisc_ssd_pages_of_block-1;i++)
  {
    int page_addr = i + offset;
    jisc_ssd_lowlevel_operation_command_input(0x80);
    jisc_ssd_lowlevel_operation_address4_input(0x00, 0x00, page_addr & 0xFF, ((page_addr >> 8) & 0xFF));

    jisc_ssd_lowlevel_operation_data_input_page_from_block_buffer(
      &(jisc_ssd_block_buffer[jisc_ssd_page_size * i])
    );
    jisc_ssd_lowlevel_operation_command_input(0x15);
  }

  {
    int i = jisc_ssd_pages_of_block-1;
    int page_addr = i + offset;
    jisc_ssd_lowlevel_operation_command_input(0x80);
    jisc_ssd_lowlevel_operation_address4_input(0x00, 0x00, page_addr & 0xFF, ((page_addr >> 8) & 0xFF));

    jisc_ssd_lowlevel_operation_data_input_page_from_block_buffer(
      &(jisc_ssd_block_buffer[jisc_ssd_page_size * i])
    );
    jisc_ssd_lowlevel_operation_command_input(0x10);
  }

  if(checkErrorStatus()){
    Serial.println("Error! ABORT");
    return;
  }
}

void readBlockOp(int block){
  int offset = jisc_ssd_pages_of_block * block;
  for(int i=0;i<jisc_ssd_pages_of_block;i++)
  {
    int page_addr = i + offset;
    jisc_ssd_lowlevel_operation_command_input(0x00);

    //カラムアドレスはページ内オフセットと思えば良い
    jisc_ssd_lowlevel_operation_address4_input(0x00, 0x00, page_addr & 0xFF, ((page_addr >> 8) & 0xFF));
    jisc_ssd_lowlevel_operation_command_input(0x30);
    jisc_ssd_lowlevel_operation_serial_data_output_page_to_block_buffer(
      &(jisc_ssd_block_buffer[jisc_ssd_page_size * i])
    );
  }
}

void eraseBlockOp(int block)
{
  int page = block * jisc_ssd_pages_of_block;
  jisc_ssd_lowlevel_operation_command_input(0x60);
  jisc_ssd_lowlevel_operation_address2_input(page & 0xFF, ((page >> 8) & 0xFF)); //下位6bitに意味はない？
  jisc_ssd_lowlevel_operation_command_input(0xD0);
}

//---------------------

extern "C" void read_fs(unsigned char* buffer, uint32_t lba, unsigned int offset, unsigned count){
  //LBA = Logical Block Address
  //ここではLBA = pageの1/2としている
  int page = bad_block_replace(lba/2);
  int in_page = page%jisc_ssd_pages_of_block;
  int block = page / jisc_ssd_pages_of_block;

  if(target_block_fs != block){
    target_block_fs = block;

    //ブロック読み出し
    readBlockOp_fs(block);
  }
  memcpy(buffer, &(jisc_ssd_block_buffer_fs[(jisc_ssd_page_size * in_page) + (512*(lba%2)) + offset]), count);
}

void readBlockOp_fs(int block){
  int offset = jisc_ssd_pages_of_block * block;
  for(int i=0;i<jisc_ssd_pages_of_block;i++)
  {
    int page_addr = i + offset;
    jisc_ssd_lowlevel_operation_command_input(0x00);

    //カラムアドレスはページ内オフセットと思えば良い
    jisc_ssd_lowlevel_operation_address4_input(0x00, 0x00, page_addr & 0xFF, ((page_addr >> 8) & 0xFF));
    jisc_ssd_lowlevel_operation_command_input(0x30);
    jisc_ssd_lowlevel_operation_serial_data_output_page_to_block_buffer(
      &(jisc_ssd_block_buffer_fs[jisc_ssd_page_size * i])
    );
  }
}


