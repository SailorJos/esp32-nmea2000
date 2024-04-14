battVoltTask

added by Jos Schotman March 2024
Based on ExampleTask

Uses the ADconvertor of the ESP32 to measure voltage as a value for the voltage of the battery. It represents the voltage of the battery which supplies the ESP32NMEA2k hub.

A voltage divider of ca 10% is applied, consisting of a 8k8 and a 1k resistor. So a voltage of about 1.2 volt is supplied to the ADconvertor. A 0.1 uF capacitor is applied for damping.

The ADconversion is first calibrated in battVoltTask to represent the actual supply voltage of the hub. 
Through the user interface a correction can by given to correct for the voltage drop over the power supply cabling between the battery and the hub.

The battery voltage is presented in a XDR sentence.




Software issues:

[in GwConfigDefinitions.h:
  static constexpr const char* ADconvertor="ADconvertor";
  static constexpr const char* bvInstanceId="bvInstanceId";
  static constexpr const char* battvoltintv="battvoltintv";
is added at the standard GwConfigDefinitions.h file] automatically????


in GwbattVoltTask.h:
  #define inputPin 35
is defined at GwbattVoltTask.h

in main.ccp:
 // Set product information
  NMEA2000.SetProductInformation("ESP32NMEA2k by Jos Schotman",    //js  "1", // Manufacturer's Model serial code



in main.ccp:
void setup() {
//===============================================================
changeNX2baud();    //JS
//===============================================================



//===============================================================
void changeNX2baud() {
/*
  NX2 Server Baudrate control.
  It is possible to change the baudrate from 4800bps to 19200bps.
  To do that, a PC is required. Note 19200 is not to be considered
  as NMEA since the standard states 4800.
  a. The Requesting unit is allowed to transmit the message:
    "$PSILBPS,19200,R,<CR><LF>"
    once every 2s at nominal 4800 bps with normal NMEA start bit and stop bit settings.
    This message may be received on any of the two Server ports.
  b. The receiving unit (NX2 Server) will Confirm message:
    "$PSILBPS,19200,C,<CR><LF>"
    and send it back on output ports to the requesting unit.
  c. When the requesting unit receives the same message but with the flag set to "C"
    (Confirmed), both server ports (A and B) are set to 19200bps and transmission may
    start at the new baudrate. The sending unit may now stop sending the proprietary
    request message since it has entered the higher baudrate. There is no way back
    unless there is a power loss.
  From power up, baudrate is always set to 4800 and the above procedure must be
  repeated.
  The receiving unit (Nexus Server) will always check for the proprietary message when
  in normal baudrate, not when in high baudrate.
*/ 
Serial1.begin(4800, SERIAL_8N1, 22, 21);
delay(500);
Serial1.println("$PSILBPS,19200,R,/r/n");
delay(2000);
Serial1.println("$PSILBPS,19200,R,/r/n");
delay(2000);
Serial1.end();
delay(500);
}
//===============================================================

in platformio.ini:
[platformio]
default_envs=
	m5stack-atom
	m5stack-atom-canunit
	m5stickc-atom-canunit
	nodemcu-homberger
extra_configs=
	lib/*task*/platformio.ini

exchanged by:

[platformio]
default_envs = ttgo-t7-v14-mini32
extra_configs = 
	lib/*task*/platformio.ini

===============================================================
 