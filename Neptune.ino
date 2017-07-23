// Output all Actisense data to a stream then parse and send to serial

#define N2k_CAN_INT_PIN 21
#include <Arduino.h>
#include <NMEA2000_CAN.h>

void setup() {
  Serial.begin(115200);
  NMEA2000.SetN2kCANMsgBufSize(8);
  NMEA2000.SetN2kCANReceiveFrameBufSize(100);
  NMEA2000.SetForwardStream(&Serial);  // PC output on due native port
  // NMEA2000.SetForwardType(tNMEA2000::fwdt_Text); // Show in clear text
  // NMEA2000.EnableForward(false);                      // Disable all msg forwarding to USB (=Serial)
  NMEA2000.Open();
}

void loop() {

  NMEA2000.ParseMessages();

}

