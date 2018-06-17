#include <U8x8lib.h>
#include <LoRa.h>

int i=0;
char contor[7];

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
  LoRa.setPins(SS, RST, DI0);
  Serial.begin(115200);
  while (!Serial); //if just the the basic function, must connect to a computer
  delay(1000);
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  Serial.println("LoRa Transmiter");
  u8x8.drawString(0, 1, "LoRa Tx pwr:14");
  u8x8.drawString(0, 2, "SF:12 BW:125k");
  u8x8.drawString(0, 6, "Heltec ESP32");

  u8x8.drawString(0, 7, "timisoso");
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    u8x8.drawString(0, 1, "Starting LoRa failed!");
    while (1);
  }
  LoRa.setTxPower(14);
   LoRa.setSignalBandwidth(125E3); 
  //7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, and 250E3.
   
   LoRa.setSpreadingFactor(12); 
   //6-12
}

void loop() {
    Serial.print("sending packet: Heltec esp32 timisoso ~ ");
    itoa(i,contor,10);
    u8x8.drawString(0, 3,"pckt:");
    u8x8.drawString(5, 3, contor);
  LoRa.beginPacket();
  LoRa.print("Heltec esp32 @timisoso ~ ");
  LoRa.print(contor); 
  LoRa.endPacket();
      Serial.println(i);
      delay (30000);
      i++;
}
