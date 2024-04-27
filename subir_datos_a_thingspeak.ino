#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN D4     // Pin donde está conectado el sensor DHT22
#define DHTTYPE DHT22 // Tipo de sensor DHT

DHT dht(DHTPIN, DHTTYPE);


const char* ssid = "CRAZYPAPA";       // Nombre de la red WiFi
const char* password = "valparaiso2023"; // Contraseña de la red WiFi
const char* server = "api.thingspeak.com";
String apiKey = "187ZYBJ3C51WBHME"; // Tu clave API de ThingSpeak
const int updateInterval = 60000;    // Intervalo de actualización en milisegundos (60000 = 1 minuto)



void setup() {
  Serial.begin(115200);
  delay(10);
  
  Serial.println();
  Serial.println("Conectando a la red WiFi...");

  dht.begin();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectada.");
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // if (isnan(humidity) || isnan(temperature)) {
  //   Serial.println("Error al leer el sensor DHT22");
  //   return;
  // }

  Serial.print("Humedad: ");
  Serial.print(humidity);
  Serial.print("%\t");
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println("°C");
  delay(1000);
  sendToThingSpeak(temperature, humidity);
  
  delay(updateInterval);
}

void sendToThingSpeak(float temp, float hum) {
  WiFiClient client;
  if (!client.connect(server, 80)) {
    Serial.println("Error al conectar con ThingSpeak");
    return;
  }

  String postStr = apiKey;
  postStr += "&field1=";
  postStr += String(temp);
  postStr += "&field2=";
  postStr += String(hum);

  client.print("POST /update HTTP/1.1\n");
  client.print("Host: api.thingspeak.com\n");
  client.print("Connection: close\n");
  client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
  client.print("Content-Type: application/x-www-form-urlencoded\n");
  client.print("Content-Length: ");
  client.print(postStr.length());
  client.print("\n\n");
  client.print(postStr);

  Serial.println("Datos enviados a ThingSpeak correctamente");

  client.stop();
}

