#include <PZEM004Tv30.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
//#include <LiquidCrystal_I2C.h>

// Pinos ESP32 conectados ao PZEM
#define RXD2 16   // RX do ESP32 -> TX do PZEM
#define TXD2 17   // TX do ESP32 -> RX do PZEM (use divisor se PZEM for 5V)

#define SDA_PIN 18  // LCD I2C
#define SCL_PIN 19

// Identificador do dispositivo
const char* SERIAL = "NTH000001";

// Instancia PZEM (apenas uma vez)
PZEM004Tv30 pzem(Serial2, RXD2, TXD2);

// endereço comum: 0x27 ou 0x3F
//LiquidCrystal_I2C lcd(0x27, 20, 4);
LiquidCrystal_PCF8574 lcd(0x27);

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Comunicação com PZEM
  delay(2000);

  Serial.print("PZEM Address: ");
  Serial.println(pzem.readAddress());

  Wire.begin(SDA_PIN, SCL_PIN);  // <<< I2C nos pinos 18 e 19
  Wire.begin(SDA_PIN, SCL_PIN);

  lcd.begin(20, 4);
  lcd.setBacklight(255);

  lcd.setCursor(0,0);
  lcd.print("LCD OK");

}

void loop() {
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();

  if (!isnan(voltage)) {
    Serial.print("Tensão: ");
    Serial.print(voltage);
    Serial.println(" V");
  } else {
    Serial.println("Sem resposta do PZEM!");
  }
  if (!isnan(current)) {
    Serial.print("Corrente: ");
    Serial.print(current);
    Serial.println(" A");
  }
  //LCD 
  lcd.setCursor(0,0);
  lcd.print("V:");
  lcd.print(voltage,1);
  lcd.print(" A:");
  lcd.print(current,2);
  lcd.print("   ");

  lcd.setCursor(0,1);
  lcd.print("P:");
  lcd.print(power,0);
  lcd.print("W        ");

  lcd.setCursor(0,2);
  lcd.print("E:");
  lcd.print(energy,3);
  lcd.print("kWh      ");

  lcd.setCursor(0,3);
  lcd.print("F:");
  lcd.print(frequency,1);
  lcd.print("Hz PF:");
  lcd.print(pf,2);
  lcd.print(" ");

  delay(1000); // lê a cada 1 segundo
}
