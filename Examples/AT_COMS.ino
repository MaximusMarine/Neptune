#include <DueFlashStorage.h>
DueFlashStorage dueFlashStorage;

//Struct of WIFI
struct Configuration {
  int join; // 0 = transmit 1 = join
  int mode;
  char* Name;
  char* Pass;
  int Channel;
  int ECN;
  int MaxCon;
  int Hidden;
  int CIPMUX;
  int SERVER;
  int PORT;
};

//initialize wifiStructure
Configuration wifiStruct;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(500);
  wifiSet();
  wifiConfiguration();
  
  }
    

void loop() {

  if(Serial1.available()) {
    byte b = Serial1.read();
    Serial.write(b);
  }

  if(Serial.available()) {
    byte c = Serial.read();
    Serial1.write(c); 
  }
  
}

void wifiSet() {
  //Set structure values
  wifiStruct.join = 0;
  wifiStruct.mode = 2;
  wifiStruct.Name = "NeptuneBox01";
  wifiStruct.Pass = "";
  wifiStruct.Channel = 11;
  wifiStruct.ECN = 0;
  wifiStruct.MaxCon = 4;
  wifiStruct.Hidden = 0;  
  wifiStruct.CIPMUX = 1
  ;
  wifiStruct.SERVER = 1;
  wifiStruct.PORT = 50000;

   // write configuration struct to flash at adress 4
  byte b2[sizeof(Configuration)]; // create byte array to store the struct
  memcpy(b2, &wifiStruct, sizeof(Configuration)); // copy the struct to the byte array
  dueFlashStorage.write(0, b2, sizeof(Configuration)); // write byte array to flash
}

void wifiConfiguration() {
  
  // read configuration struct from flash
  byte* b = dueFlashStorage.readAddress(0); // byte array which is read from flash at adress 4
  Configuration configurationFromFlash; // create a temporary struct
  memcpy(&configurationFromFlash, b, sizeof(Configuration)); // copy byte array to temporary struct

  delay(5000);
  
  Serial1.print("AT+CWMODE=");
  Serial1.println(wifiStruct.mode);
  delay(500);
  
  Serial1.print("AT+CWSAP=");
  Serial1.print("\"");
  Serial1.print(wifiStruct.Name);
  Serial1.print("\"");
  Serial1.print(",");
  Serial1.print("\"");
  Serial1.print(wifiStruct.Pass);
  Serial1.print("\"");
  Serial1.print(",");
  Serial1.print(wifiStruct.Channel);
  Serial1.print(",");
  Serial1.print(wifiStruct.ECN);
  Serial1.print(",");
  Serial1.print(wifiStruct.MaxCon);
  Serial1.print(",");
  Serial1.println(wifiStruct.Hidden);
  delay(500);
  
  Serial1.print("AT+CIPMUX=");
  Serial1.println(wifiStruct.CIPMUX);
  delay(500);

  Serial1.print("AT+CIPSERVER=");
  Serial1.print(wifiStruct.SERVER);
  Serial1.print(",");
  Serial1.println(wifiStruct.PORT);
  delay(500);
  
}

