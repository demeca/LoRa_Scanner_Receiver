     
#include <SPI.h>
#include <RH_RF95.h>
#include "heltec.h"
    
#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6
#define RFM95_RST     14   // "A"
#define RFM95_CS      18   // "B"
#define RFM95_INT     26    // "C"
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
  
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

//#define DebugOn

void setup()
{
    Heltec.begin(true /*DisplayEnable Enable*/, true /*LoRa Enable*/, true /*Serial Enable*/, true /*LoRa use PABOOST*/, BAND /*LoRa RF working band*/);
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
   
    Serial.begin(9600);
    while (!Serial) {
      delay(1);
    }
    delay(100);
   
    //Serial.println("Feather LoRa RX Test!");
   
    // manual reset
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
   
    while (!rf95.init()) {
      Heltec.display -> clear();
      Heltec.display -> drawString(0, 0, "LoRa radio init failed");
      Heltec.display -> display();
      ////Serial.println("LoRa radio init failed");
      ////Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
      while (1);
    }
    Heltec.display -> clear();
    Heltec.display -> drawString(0, 0, "LoRa radio init OK!");
    Heltec.display -> display();
    delay(2000);
   
    // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
    if (!rf95.setFrequency(RF95_FREQ)) {
      Heltec.display -> clear();
      Heltec.display -> drawString(0, 0, "setFrequency failed");
      Heltec.display -> display();
      ////Serial.println("setFrequency failed");
      while (1);
    }
    Heltec.display -> clear();
    Heltec.display -> drawString(0, 0, "sSet Freq to: " + String(RF95_FREQ));
    Heltec.display -> display();
    ////Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
   
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
   
    // The default transmitter power is 13dBm, using PA_BOOST.
    // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
    // you can set transmitter powers from 5 to 23 dBm:
    rf95.setTxPower(23, false);
    Heltec.display -> clear();
    Heltec.display -> drawString(0, 0, "Waiting for Data...");
    Heltec.display -> display();
}
     
void loop()
{
    if (rf95.available())
    {
      // Should be a message for us now
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
   
      if (rf95.recv(buf, &len))
      {
        //RH_RF95::printBuffer("Received: ", buf, len);
        ////Serial.println((char*)buf);
        ////Serial.print("RSSI: ");
        ////Serial.println(rf95.lastRssi(), DEC);
        Heltec.display -> clear();
        Heltec.display -> drawString(0, 0, "Got Car# " + String((char*)buf));
        Heltec.display -> drawString(0, 20, "Strength: " + String(rf95.lastRssi()) + "db");
        Heltec.display -> display();
        Serial.print((char*)buf);
   
        // Send a reply
        uint8_t data[] = "Recieved";
        rf95.send(data, sizeof(data));
        rf95.waitPacketSent();
        Heltec.display -> drawString(0, 30, "Sent Reply...");
        Heltec.display -> display();
        ////Serial.println("Sent a reply");
      }
      else
      {
        Heltec.display -> clear();
        Heltec.display -> drawString(0, 0, "Receive failed");
        Heltec.display -> display();
        delay(1000);
        Heltec.display -> clear();
        Heltec.display -> drawString(0, 0, "Waiting for Data...");
        Heltec.display -> display();
        ////Serial.println("Receive failed");
      }
    }
    delay(500);
    Heltec.display -> clear();
    Heltec.display -> drawString(0, 0, "Waiting for Data...");
    Heltec.display -> display();
}
