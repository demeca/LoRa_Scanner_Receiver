# LoRa Scanner Reciever
This is compainion code to the LoRa Scanner project. It will recieve signals over a LoRa radio and transmit them to a connected PC via USB to Serial. The designed use for this sofware is as a long range barcode scanner for our autocross club.  
I used a Heltec WiFi LoRa V2 board. Nothing special was needed to flash this board from the Arduino IDE.  
It is set to the 915 band, and the serial is set to 9600 baud due to a software limitation in my use case.  
The display lets you know when a barcode has been revieved.  
  
The Arduino IDE needs the Heltec URL added to the board manager. https://resource.heltec.cn/download/package_heltec_esp32_index.json  

A compiled version is included in addition to the source.