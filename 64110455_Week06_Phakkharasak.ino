#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"

const char* ssid = "SAMSUNGPHAT";
const char* password = "1234567890";
unsigned long channelID = 2339908;
const char* writeKey = "T8V3ZXZAL49705WR";

#define DHTPIN D4
#define DHTTYPE DHT11

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

void setupWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi");
  } else {
    Serial.println("\nFailed to connect to WiFi. Please check your credentials");
  }
}

void updateThingSpeak() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.println("°C ");
  
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int response = ThingSpeak.writeFields(channelID, writeKey);

  if (response == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(response));
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  dht.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() != WL_CONNECTED) {
      setupWiFi();
    }

    updateThingSpeak();
    lastTime = millis();
  }
}
