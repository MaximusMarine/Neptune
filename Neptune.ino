// Output all Actisense data to a stream then parse and send to serial

#define N2k_CAN_INT_PIN 21
#include <Arduino.h>
#include <NMEA2000_CAN.h>

//Stream *OutputStream;
//OutputStream.begin(115200);
int dataSize;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  NMEA2000.SetN2kCANMsgBufSize(8);
  NMEA2000.SetN2kCANReceiveFrameBufSize(100);
  NMEA2000.SetForwardStream(&Serial);  // PC output on due native port
  // NMEA2000.SetForwardType(tNMEA2000::fwdt_Text); // Show in clear text
  // NMEA2000.EnableForward(false);                      // Disable all msg forwarding to USB (=Serial)
  NMEA2000.Open();
}

void loop() {
  /*
  if(Serial1.available()) {
    byte b = Serial1.read();
    Serial.write(b);
  }
  */

  if(Serial1.available()) {
    byte b = Serial1.read();
    Serial.write(b);
  }

  if(Serial.available()) {
    byte c = Serial.read();
    //dataSize = sizeof(c);
    //Serial1.print("AT+CIPSEND=0,");
    //Serial1.println(dataSize);
    Serial1.write(c); 
  }
  
  NMEA2000.ParseMessages();

}

