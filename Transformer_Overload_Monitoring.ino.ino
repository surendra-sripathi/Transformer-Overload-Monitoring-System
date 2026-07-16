#include <LiquidCrystal.h>  // LCD library

// LCD pins: RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Sensor pins
const int currentPin = A0;  // Current sensor (ACS712)
const int voltagePin = A1;  // Voltage sensor (PT or divider)
const int tempPin = A2;     // Temperature sensor (LM35)

// Output pins
const int buzzerPin = 8;
const int relayPin = 7;

// Safe limits
const float currentLimit = 5.0;    // Max current in Amps
const float voltageLimit = 230.0;  // Max voltage in Volts
const float tempLimit = 75.0;      // Max temperature in Celsius

void setup() {
  lcd.begin(16, 2);               // Initialize LCD
  pinMode(buzzerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(relayPin, LOW);

  lcd.print("Transformer OLM");   // Welcome message
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read sensors
  float current = readCurrent();
  float voltage = readVoltage();
  float temperature = readTemperature();

  // Display on LCD
  lcd.setCursor(0, 0);
  lcd.print("I:");
  lcd.print(current);
  lcd.print("A V:");
  lcd.print(voltage);

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print("C");

  // Check for overload
  if(current > currentLimit || voltage > voltageLimit || temperature > tempLimit){
    digitalWrite(buzzerPin, HIGH);  // Turn on buzzer
    digitalWrite(relayPin, HIGH);   // Trip relay
  } else {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(relayPin, LOW);    // Keep system normal
  }

  delay(1000);  // Update every second
}

// --- Functions to convert sensor readings ---

float readCurrent() {
  int sensorValue = analogRead(currentPin);
  float voltage = sensorValue * (5.0 / 1023.0);
  float current = (voltage - 2.5) / 0.185; // ACS712 5A version
  if(current < 0) current = -current;
  return current;
}

float readVoltage() {
  int sensorValue = analogRead(voltagePin);
  float voltage = sensorValue * (5.0 / 1023.0);
  voltage = voltage * (230.0 / 5.0); // Scale to real transformer voltage
  return voltage;
}

float readTemperature() {
  int sensorValue = analogRead(tempPin);
  float voltage = sensorValue * (5.0 / 1023.0);
  float temp = voltage * 100.0; // LM35: 10mV per °C
  return temp;
}