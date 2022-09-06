#include "jisc_ssd.h"

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);

  jisc_ssd_lowlevel_operation_init();
  jisc_ssd_lowlevel_operation_reset();

  //Serial接続待ち
  while (!Serial) {
    digitalWrite(PIN_LED, LOW);
    delay(100);
    digitalWrite(PIN_LED, HIGH);
    delay(100);
  }
}

void loop() {
  Serial.println("--- JISC-SSD Test console v0.01 ---");
  Serial.println("1.Read Block");
  Serial.println("2.Write Block");
  Serial.println("3.Erase Block");
  Serial.println("4.Erase All");
  Serial.println("5.Check Bad Block (Auto Erase all blocks)");
  Serial.println("6.Read ID");
  Serial.println("7.Read Status");
  Serial.println("0.Show help(this text)");
  Serial.print(">");

  int c = read();
  Serial.write((byte)c);
  Serial.println();

  switch (c)
  {
  case '1':
    ReadBlock();
    break;
  case '2':
    WriteBlock();
    break;
  case '3':
    EraseBlock();
    break;
  case '4':
    EraseAll();
    break;
  case '5':
    CheckBadBlock();
    break;
  case '6':
    ReadID();
    break;
  case '7':
    ReadStatus();
    break;

  case '0':
    break;
  default:
    break;
  }
}

void ReadBlock()
{
  //ブロック番号取得
  Serial.println("Enter block number[0-1023]");
  int num = enterNum();
  if(num > (jisc_ssd_blocks-1) || num < 0){
    Serial.println("Range error.");
    return;
  }
  Serial.print("block:");
  Serial.println(num);

  Serial.println("loading...");

  //ゼロクリア
  memset(jisc_ssd_block_buffer,0,jisc_ssd_block_size);

  //ブロック読み出し
  readBlockOp(num);

  //ダンプ
  int offset = jisc_ssd_pages_of_block * num;
  dumpBlockBuffer(offset);
  Serial.println("Done");
}

void WriteBlock()
{
  Serial.println("Select write pattern");
  Serial.println("1. 00");
  Serial.println("2. AA");
  Serial.println("3. 55");
  Serial.println("4. AA 55");
  Serial.println("5. DE AD BE EF");
  Serial.println("6. page num");
  Serial.println("7. Sequence");
  Serial.println("8. Hello World");
  Serial.println("9. random");
  Serial.println("0. Cancel");

  int c = read();
  Serial.write((byte)c);
  Serial.println();

  //FFクリア
  memset(jisc_ssd_block_buffer,0xFF,jisc_ssd_block_size);

  switch (c)
  {
  case '1':
    memset(jisc_ssd_block_buffer,0x00,jisc_ssd_block_size);
    break;
  case '2':
    memset(jisc_ssd_block_buffer,0xAA,jisc_ssd_block_size);
    break;
  case '3':
    memset(jisc_ssd_block_buffer,0x55,jisc_ssd_block_size);
    break;
  case '4':
    {
      const char pattern[] = {0xAA, 0x55, 0x00};
      fillPattern(pattern);
    }
    break;
  case '5':
    {
      const char pattern[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x00};
      fillPattern(pattern);
    }
    break;
  case '6':
    {
      for(int i=0;i<jisc_ssd_block_size;i++){
        jisc_ssd_block_buffer[i] = i/jisc_ssd_page_size;
      }
    }
    break;
  case '7':
    {
      for(int i=0;i<jisc_ssd_block_size;i++){
        jisc_ssd_block_buffer[i] = i&0xFF;
      }
    }
    break;
  case '8':
    fillPattern("Hello World");
    break;
  case '9':
      for(int i=0;i<jisc_ssd_block_size;i++){
        jisc_ssd_block_buffer[i] = random(256);
      }
    break;  
  default:
    return;
  }

  //ブロック番号取得
  Serial.println("Enter block number[0-1023]");
  int num = enterNum();
  if(num > (jisc_ssd_blocks-1) || num < 0){
    Serial.println("Range error.");
    return;
  }
  Serial.print("block:");
  Serial.println(num);

  Serial.println("write...");
  writeBlockOp(num);
  Serial.println("Done");

}

void EraseBlock()
{
  //ブロック番号取得
  Serial.println("Enter block number[0-1023]");
  int num = enterNum();
  if(num > (jisc_ssd_blocks-1) || num < 0){
    Serial.println("Range error.");
    return;
  }
  Serial.print("block:");
  Serial.println(num);

  Serial.println("erase...");

  //消去
  eraseBlockOp(num);
  if(checkErrorStatus()){
    Serial.println("Error");
  }else{
    Serial.println("Done");
  }
}

void EraseAll()
{
  //意思確認
  while(true){
    Serial.println("Erase continue? (Y/N)");
    Serial.print(">");
    
    int c = read();
    Serial.write((byte)c);
    Serial.println();

    if(c == 'Y' || c == 'y'){
      break;
    }
    if(c == 'n' || c == 'n'){
      return;
    }
  }

  Serial.println("erase...");
  //全消去
  for (int block = 0; block < jisc_ssd_blocks; block++) {
    eraseBlockOp(block);
    //エラーチェックはせず強行する
  }
  Serial.println("Done");
}

void CheckBadBlock()
{
  EraseAll();

  for (int block = 0; block < jisc_ssd_blocks; block++) {
    bool bad = false;
    int page = block * jisc_ssd_pages_of_block;
    jisc_ssd_lowlevel_operation_command_input(0x00);
    jisc_ssd_lowlevel_operation_address4_input(0x00, 0x00, page & 0xFF, ((page >> 8) & 0xFF));
    jisc_ssd_lowlevel_operation_command_input(0x30);
    if (jisc_ssd_lowlevel_operation_serial_data_output() == 0) {
      bad = true;
    }
    if (bad) {
      Serial.print("BAD BLOCK: ");
      Serial.print(block);
      Serial.print(" (page:");
      Serial.print(page);
      Serial.print(" - ");
      Serial.print(page + 63);
      Serial.print(" addr: 0x");
      Serial.print(page * jisc_ssd_page_size,HEX);
      Serial.print(" - 0x");
      Serial.print((page + 63) * jisc_ssd_page_size,HEX);
      Serial.print(")");
      Serial.println();
    }
  }

}

void ReadID()
{
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

void ReadStatus(){
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

bool checkErrorStatus(){
  jisc_ssd_lowlevel_operation_command_input(0x70);
  int x1 = jisc_ssd_lowlevel_operation_serial_data_output();
  if ((x1 & 1) != 0) {
    return true;
  }
  return false;
}

void fillPattern(const char* pattern){
  int p = 0;
  for(int i=0;i<jisc_ssd_block_size;i++){
    if(pattern[p] == '\0'){
      p = 0;
    }
    jisc_ssd_block_buffer[i] = pattern[p];
    p++;
  }
}

void writeBlockOp(int block){
  int offset = jisc_ssd_pages_of_block * block;
  for(int i=0;i<jisc_ssd_pages_of_block;i++)
  {
    int page_addr = i + offset;
    jisc_ssd_lowlevel_operation_command_input(0x80);
    jisc_ssd_lowlevel_operation_address4_input(0x00, 0x00, page_addr & 0xFF, ((page_addr >> 8) & 0xFF));

    jisc_ssd_lowlevel_operation_data_input_page_from_block_buffer(
      &(jisc_ssd_block_buffer[jisc_ssd_page_size * i])
    );
    jisc_ssd_lowlevel_operation_command_input(0x10);
    jisc_ssd_lowlevel_operation_command_input(0x10);
    if(checkErrorStatus()){
      Serial.println("Error! ABORT");
      return;
    }
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

//ブロックバッファをダンプする
void dumpBlockBuffer(int offset)
{
  char buf[16]={0};
  Serial.println("ADDRESS |00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |      ASCII     |");
  for (int i = 0; i < jisc_ssd_block_size/16; i++) {
    int adr = (offset*jisc_ssd_page_size) + i*16;
    if(adr%2176==0){
      Serial.println("--------+-----------------------PAGE---------------------+----------------|");      
    }

    sprintf(buf,"%08X",adr);
    Serial.print(buf);

    Serial.print("|");
    for(int k=0; k<16;k++){
      int c = jisc_ssd_block_buffer[(i*16)+k];
      if(c < 0x10){
        Serial.print("0");
      }
      Serial.print(c, HEX);
      Serial.print(" ");
    }
    Serial.print("|");
    for(int k=0; k<16;k++){
      int c = jisc_ssd_block_buffer[(i*16)+k];
      if(c>=0x20 && c<=0x7E){
        Serial.write(c);
      }else{
        Serial.write(0x2E);
      }
    }
    Serial.print("|\n");
  }
}

//ブロック番号取得
int enterNum(){
  Serial.print(">");
  String input = readLine();
  Serial.println();

  char buf[16]={0};
  input.toCharArray(buf,16);
  int num = atoi(buf);
  return num;
}

//受信バッファを空にする
void flush(){
  while(Serial.read() != -1);
}

//1文字読み込む
int read(){
  int c=-1;
  flush();
  while(c==-1){
    c = Serial.read();
  }
  flush();
  return c;
}

//1行読み込む
String readLine(){
  int c=-1;
  String s{};

  flush();
  while(true){
    do{
      c = Serial.read();
    }while(c==-1);
    if(c == 0x0D || c == 0x0A){
      flush();
      return s;
    }
    if(c>=0x20 && c<=0x7E){
      Serial.write(c);
      s += String((char)c);
    }
  }
}