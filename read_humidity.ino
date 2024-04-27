#include <DHT.h>

#define DHTPIN D4     // Pin al que está conectado el sensor DHT22 (GPIO2)
#define DHTTYPE DHT22 // Tipo de sensor DHT

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("Leyendo datos del sensor DHT22...");
  dht.begin();
}

void loop() {
  delay(2000); // Espera 2 segundos entre lecturas

  float humidity = dht.readHumidity();

  if (isnan(humidity)) {
    Serial.println("Error al leer el sensor DHT22.");
  } else {
    Serial.print("Humedad: ");
    Serial.print(humidity);
    Serial.println("%");
  }
}
