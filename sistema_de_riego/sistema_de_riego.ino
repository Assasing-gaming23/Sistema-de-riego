#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// Pantalla LCD I2C en dirección 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int humedadPin = 34;
const int relayPin = 26;
int humedad;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("RiegoESP32");
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Sistema Riego");
  delay(2000);
}

void loop() {
  humedad = analogRead(humedadPin);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humedad: ");
  lcd.print(humedad);

  String estado = "Apagado";

  if (SerialBT.available()) {
    String comando = SerialBT.readStringUntil('\n');
    comando.trim();
    
    if (comando == "ON") {
      digitalWrite(relayPin, HIGH);
      estado = "Encendido";
      SerialBT.println("Bomba ENCENDIDA");
    } else if (comando == "OFF") {
      digitalWrite(relayPin, LOW);
      estado = "Apagado";
      SerialBT.println("Bomba APAGADA");
    } else if (comando == "AUTO") {
      if (humedad > 600) {
        digitalWrite(relayPin, HIGH);
        estado = "Auto: ENC.";
        SerialBT.println("Auto-riego activado");
      } else {
        digitalWrite(relayPin, LOW);
        estado = "Auto: OFF";
        SerialBT.println("Humedad suficiente");
      }
    }
  }

  lcd.setCursor(0, 1);
  lcd.print("Bomba: ");
  lcd.print(estado);

  delay(2000);
}