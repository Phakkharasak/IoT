#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

float temperature;
float humidity;
DHT dht14(D4, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  temperature = 0;
  humidity = 0;
  Serial.begin(9600);
  dht14.begin();
  lcd.begin();
  lcd.backlight();
}

void loop() {
  humidity = dht14.readHumidity();
  temperature = dht14.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.setCursor(7, 0);
  lcd.print(String(temperature, 1) + " C");

  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.setCursor(7, 1);
  lcd.print(String(humidity, 1) + " %");
  delay(2000);
}
