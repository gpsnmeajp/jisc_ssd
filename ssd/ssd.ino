void read_id(){
  ssd_command_input(0x90);
  ssd_address_input(0x00);

  int x1 = ssd_serial_data_output();
  int x2 = ssd_serial_data_output();
  int x3 = ssd_serial_data_output();
  int x4 = ssd_serial_data_output();
  int x5 = ssd_serial_data_output();
  
  Serial.print("Maker Code: ");
  Serial.print(x1,HEX);
  Serial.println();
  
  Serial.print("Device Code: ");
  Serial.print(x2,HEX);
  Serial.println();
  Serial.print("Chip Number, Cell Type: ");
  Serial.print(x3,HEX);  
  Serial.println();
  Serial.print(" Chip Number: ");
  Serial.print(1<<(x3&3));  
  Serial.println();
  Serial.print(" Cell Type: ");
  Serial.print(2<<((x3>>2) & 3));  
  Serial.println();

  Serial.print("Page Size, Block Size: ");
  Serial.print(x4,HEX);
  Serial.println();
  Serial.print(" Page Size: ");
  Serial.print(1<<(x4&3));  
  Serial.println();
  Serial.print(" Block Size: ");
  Serial.print(64<<((x4>>4)&3));  
  Serial.println();
  Serial.print(" I/O Width: ");
  Serial.print(8<<((x4>>6)&1));  
  Serial.println();

  Serial.print("District Number: ");
  Serial.print(x5,HEX);
  Serial.println();
  Serial.print(" District Number: ");
  Serial.print(1<<((x5>>2) & 3));  
  Serial.println();
}

void read_status(){
  ssd_command_input(0x70);

  int x1 = ssd_serial_data_output();
  
  Serial.print("Status :");
  Serial.print(x1,HEX);
  Serial.println();

  if(x1&1 == 0){
    Serial.println(" Chip Status1: Pass");
  }else{
    Serial.println(" Chip Status1: Fail");
  }
  if(x1&2 == 0){
    Serial.println(" Chip Status2: Pass");
  }else{
    Serial.println(" Chip Status2: Fail");
  }
  if(x1&32){
    Serial.println(" Page Buffer Ready/Budy: Ready");
  }else{
    Serial.println(" Page Buffer Ready/Budy: Busy");
  }
  if(x1&64){
    Serial.println(" Data Cache Ready/Budy: Ready");
  }else{
    Serial.println(" Data Cache Ready/Budy: Busy");
  }
  if(x1&128){
    Serial.println(" Write Protect: Not Protected");
  }else{
    Serial.println(" Write Protect: Protected");
  }
}


void setup() {
  pinMode(PIN_LED,OUTPUT);
  ssd_init();
  Serial.begin(115200);

  //Serial表示待ち
  while(!Serial){
    digitalWrite(PIN_LED,LOW);
    delay(100);
    digitalWrite(PIN_LED,HIGH);
    delay(100);
  }

  Serial.println("Ready");
  ssd_reset();
  read_id();
  read_status();
}

void loop() {
}