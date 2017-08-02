/* Saves wifi details to structure at slot 0 of due flash. Then reads output to serial. */

#include <DueFlashStorage.h>
DueFlashStorage dueFlashStorage;

//Struct of WIFI
struct Configuration {
  int mode; // 0 = transmit 1 = join
  char* wifiSSID;
  char* wifiPass;
  int wifiChannel;
  int wifiECN;
  int wifiCIPMUX;
  int wifiSERVER;
  int wifiPORT;
};

//initialize wifiStructure
Configuration wifiStruct;

void setup() {
  //initialize serial
  Serial.begin(115200);
  delay(500);

  //Set structure values
  wifiStruct.mode = 0;
  wifiStruct.wifiSSID = "NeptuneBox01";
  wifiStruct.wifiPass = "";
  wifiStruct.wifiChannel = 11;


  // write configuration struct to flash at adress 4
  byte b2[sizeof(Configuration)]; // create byte array to store the struct
  memcpy(b2, &wifiStruct, sizeof(Configuration)); // copy the struct to the byte array
  dueFlashStorage.write(0, b2, sizeof(Configuration)); // write byte array to flash

}

void loop() {
  // read configuration struct from flash
  byte* b = dueFlashStorage.readAddress(0); // byte array which is read from flash at adress 4
  Configuration configurationFromFlash; // create a temporary struct
  memcpy(&configurationFromFlash, b, sizeof(Configuration)); // copy byte array to temporary struct

  // print the content
  Serial.print("mode: ");
  Serial.println(configurationFromFlash.mode);

  Serial.print("SSID: ");
  Serial.println(configurationFromFlash.wifiSSID);

  Serial.print("PASS: ");
  Serial.println(configurationFromFlash.wifiPass);

  Serial.print("Channel: ");
  Serial.println(configurationFromFlash.wifiChannel);

  delay(1000);
}
