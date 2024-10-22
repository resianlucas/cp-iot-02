#include <DHT.h>

#define DHTPIN 2      // Pino do DHT11
#define DHTTYPE DHT11 // Tipo de sensor DHT
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); // Comunicação Serial
  dht.begin();        // Inicia o sensor
}

void loop() {
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Erro ao ler o sensor!");
    return;
  }

  // Envia os dados de temperatura e umidade para o Node-RED via Serial
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(", Umidade: ");
  Serial.println(umidade);

  delay(2000); // Espera 2 segundos
}
