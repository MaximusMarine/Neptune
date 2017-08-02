Source Code for NeptuneBox Device: neptunebox.com

Allows reading of NMEA2000 network bus. Transmits data via wifi in Seasmart.

Hardware specifications:
- Arduino Due board
- ESP8266 *ESP-07
- Waveshare SN65HVD230 CAN bus
- Active NMEA2000 bus

Neptune relies on the following libraries to run on the Arduino Due and make use of the internal CAN bus:

- NMEA2000: https://github.com/ttlappalainen/NMEA2000
- NMEA2000_due: https://github.com/ttlappalainen/NMEA2000_due
- due_can: https://github.com/collin80/due_can

*Previously implemented and tested with ESP-01s. Switched to ESP-07 for external antenna.
