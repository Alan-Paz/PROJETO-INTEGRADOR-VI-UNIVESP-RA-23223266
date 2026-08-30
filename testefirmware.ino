
// WiFi
//const char* ssid = "SEU_WIFI";
//const char* password = "SUA_SENHA";
//const char* ssid = "Northale2";
//const char* password = "DragonAge143";

#include <WiFi.h>
#include <HTTPClient.h>
#include <PZEM004Tv30.h>

// --- Configurações ---
const char* ssid = "Northale2";
const char* password = "DragonAge143";

// URL do Django
const char* serverName = "http://192.168.0.15:8000/api/medicoes/";

// Pinos ESP32 conectados ao PZEM
#define RXD2 16   // RX do ESP32 vai ao TX do PZEM
#define TXD2 17   // TX do ESP32 vai ao RX do PZEM (via divisor de tensão se PZEM for 5V)

// Instancia PZEM apenas uma vez
//PZEM004Tv30 pzem(Serial2, RXD2, TXD2);

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(2000);

  // Mostra o endereço do PZEM
  Serial.print("PZEM Address: ");
  Serial.println(pzem.readAddress());

  // Conecta WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {

    // Lê medições do PZEM com pequenos delays
    float voltage = pzem.voltage(); delay(100);
    float current = pzem.current(); delay(100);
    float power = pzem.power(); delay(100);
    float energy = pzem.energy(); delay(100);
    float frequency = pzem.frequency(); delay(100);
    float pf = pzem.pf(); delay(100);

    // Mostra no Serial Monitor
    if (!isnan(voltage)) {
      Serial.print("Voltage: "); Serial.println(voltage);
      Serial.print("Current: "); Serial.println(current);
      Serial.print("Power: "); Serial.println(power);
      Serial.print("Energy: "); Serial.println(energy);
      Serial.print("Frequency: "); Serial.println(frequency);
      Serial.print("PF: "); Serial.println(pf);
      Serial.print("Address: "); Serial.println(pzem.readAddress());

      // Envia dados para Django
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");

      String json = "{";
      json += "\"voltage\":" + String(voltage, 2) + ",";
      json += "\"current\":" + String(current, 3) + ",";
      json += "\"power\":" + String(power, 2) + ",";
      json += "\"energy\":" + String(energy, 3) + ",";
      json += "\"frequency\":" + String(frequency, 2) + ",";
      json += "\"pf\":" + String(pf, 2);
      json += "}";

      int httpResponseCode = http.POST(json);
      Serial.print("Código HTTP: ");
      Serial.println(httpResponseCode);

      http.end();
    } else {
      Serial.println("Erro na leitura do PZEM");
    }
  }

  delay(5000); // envia a cada 5 segundos
}