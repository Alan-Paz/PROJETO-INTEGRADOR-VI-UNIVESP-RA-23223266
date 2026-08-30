#include <PZEM004Tv30.h>

#define RXD2 16
#define TXD2 17

PZEM004Tv30 pzem(Serial2, RXD2, TXD2);//0xF8

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(3000);
}

void loop() {

  while (Serial2.available()) {
    int c = Serial2.read();
    Serial.print(c, HEX);
    Serial.print(" ");
  }


  float voltage = pzem.voltage();
  float frequency = pzem.frequency();

  Serial.print("Tensão: ");
  Serial.println(voltage);

  Serial.print("Frequência: ");
  Serial.println(frequency);

  delay(2000);
}
