#include <PZEM004Tv30.h>

// Pinos ESP32 conectados ao PZEM
#define RXD2 16   // RX do ESP32 -> TX do PZEM
#define TXD2 17   // TX do ESP32 -> RX do PZEM (use divisor se PZEM for 5V)

// Instancia PZEM (apenas uma vez)
PZEM004Tv30 pzem(Serial2, RXD2, TXD2);

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Comunicação com PZEM
  delay(2000);

  Serial.print("PZEM Address: ");
  Serial.println(pzem.readAddress());
}

void loop() {
  float voltage = pzem.voltage(); // lê apenas a tensão

  if (!isnan(voltage)) {
    Serial.print("Tensão: ");
    Serial.print(voltage);
    Serial.println(" V");
  } else {
    Serial.println("Sem resposta do PZEM!");
  }

  delay(1000); // lê a cada 1 segundo
}
