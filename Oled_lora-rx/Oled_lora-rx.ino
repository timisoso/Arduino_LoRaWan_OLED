#include <U8x8lib.h>
#include <LoRa.h>

String receivedText;
String receivedRssi;

// WIFI_LoRa_32 ports
// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)

#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6

// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

void setup() {
  SPI.begin(5, 19, 27, 18);
    pinMode(SS, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(DI0, INPUT);
  LoRa.setPins(SS, RST, DI0);
  
  Serial.begin(115200);
  while (!Serial); //if just the the basic function, must connect to a computer
  delay(1000);
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  Serial.println("LoRa Receiver");
  u8x8.drawString(0, 1, "LoRa Rx");
  u8x8.drawString(0, 2, "SF:14 BW:125k");
  u8x8.drawString(0, 7, "@timisoso");
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    u8x8.drawString(0, 1, "Starting LoRa failed!");
    while (1);
  }
 LoRa.setSignalBandwidth(125E3); 
  //7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, and 250E3.
   
  LoRa.setSpreadingFactor(14); 
   //6-12
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  char len[5];
  itoa(packetSize,len,10);
  u8x8.drawString(0, 4, len);
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
    delay (100);
    int i=0;
    // read packet
    while (LoRa.available()) {
      char currenttext[64];
      receivedText =(char) LoRa.read();
      Serial.print(receivedText);    
      receivedText.toCharArray(currenttext, 64);
      u8x8.drawString(i, 5, currenttext);
      i=i+1;
    }
    // print RSSI of packet
    Serial.print("' with RSSI: ");
    Serial.print(LoRa.packetRssi());
     Serial.print("' with SNR: ");
    Serial.println(LoRa.packetSnr());
    u8x8.drawString(0, 3, "RSSI");
    receivedRssi = LoRa.packetRssi();
    char currentrs[64];
    receivedRssi.toCharArray(currentrs, 64);
    u8x8.drawString(5, 3, currentrs);
    Serial.println(packetSize);
  }
}
