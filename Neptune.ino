// Output all Actisense data to a stream then parse and send to serial

#define N2k_CAN_INT_PIN 21
#include <Arduino.h>
#include <NMEA2000_CAN.h>
#include <N2kStream.h>
#include <DueFlashStorage.h>
#include <N2kMsg.h>

DueFlashStorage dueFlashStorage;

//Struct of WIFI
struct WifiConfiguration {
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

void setup() {
  Serial.begin(115200);
  Serial1.begin(112200);
  delay(500);
  WifiConfiguration wifiConfig = CreateInitialWifiConfig();
  InitalizeWifi(wifiConfig);
  
  NMEA2000.SetN2kCANMsgBufSize(8);
  NMEA2000.SetN2kCANReceiveFrameBufSize(100);
  //NMEA2000.SetForwardStream(&Serial);  // PC output on due native port
  //NMEA2000.SetForwardType(tNMEA2000::fwdt_Text); // Show in clear text
  NMEA2000.SetMsgHandler(HandleN2KMsg);
  //NMEA2000.EnableForward(false);  // Disable all msg forwarding to USB (=Serial)
  NMEA2000.Open();
}

void loop() {
  NMEA2000.ParseMessages();
  //SendActisenseToToWifi();
  DebugWifiAllowSendingCommands();
}

void HandleN2KMsg(const tN2kMsg &msg){
  //msg.SendInActisenseFormat(&Serial);
  ActisenseData data = msg.GetActisenseData();
  SendActisenseToToWifi(data);
}

void SendActisenseToToWifi(ActisenseData data){
  //Inform ESP8266 of incoming data
  //Serial1.print("\n");
  Serial1.print("AT+CIPSEND=0,");
  Serial1.println(data.DataSize);
  /*
  String s = Serial1.readString();

  if (s.indexOf("busy")) {
      SendActisenseToToWifi(data);
      Serial.println("busy response");
  }
    else {
      Serial.println("No busy response");
      return;
    }
  */
  //To serial connection for debuging
  //Serial.print("\n");
  //Serial.print("AT+CIPSEND=0,");
  //Serial.println(data.DataSize);
  getReply(2000);

  //Send data to ESP8266
  Serial1.write(data.Data, data.DataSize);
  
  //Send data to serial connection for debugging
  //Serial.write(data.Data, data.DataSize);
  getReply(2000);

}

void DebugWifiAllowSendingCommands(){
  //Debug Wifi Card
  int bufSize = 1024;
  char buf[bufSize];

  while(int bytes = Serial.available()){
    if (bytes > bufSize) {
      bytes = bufSize;
    }
    Serial.readBytes(buf, bytes);
    Serial1.write(buf, bytes);
  }
}

WifiConfiguration CreateInitialWifiConfig() {
  //Set structure values
  WifiConfiguration wifiStruct;
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

  return wifiStruct;
}

WifiConfiguration LoadConfiguration(){
  // read configuration struct from flash
  byte* b = dueFlashStorage.readAddress(0); // byte array which is read from flash at adress 4
  WifiConfiguration configurationFromFlash; // create a temporary struct
  memcpy(&configurationFromFlash, b, sizeof(WifiConfiguration)); // copy byte array to temporary struct
  return configurationFromFlash;
}

void SaveConfiguration(WifiConfiguration config){
  // write configuration struct to flash at adress 4
  byte b2[sizeof(WifiConfiguration)]; // create byte array to store the struct
  memcpy(b2, &config, sizeof(WifiConfiguration)); // copy the struct to the byte array
  dueFlashStorage.write(0, b2, sizeof(WifiConfiguration)); // write byte array to flash
}

void InitalizeWifi(WifiConfiguration wifiStruct) {
  getReply(200);
  
  Serial1.print("AT+CWMODE=");
  Serial1.println(wifiStruct.mode);
  getReply(100);
  
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
  getReply(100);
  
  Serial1.print("AT+CIPMUX=");
  Serial1.println(wifiStruct.CIPMUX);
  getReply(100);

  Serial1.print("AT+CIPSERVER=");
  Serial1.print(wifiStruct.SERVER);
  Serial1.print(",");
  Serial1.println(wifiStruct.PORT);
  getReply(100);
}

void getReply(int wait)
{
    int tempPos = 0;
    long int time = millis();
    int bufSize = 1024;
    char buf[bufSize];
    
    while( (time + wait) > millis())
    {      
        while(int bytes = Serial1.available()){
          if (bytes > bufSize) {
            bytes = bufSize;
          }
          Serial1.readBytes(buf, bytes);
          Serial.write(buf, bytes);
          if (bytes <= bufSize) {
            return;
          }
        }
    }
}

