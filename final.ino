#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int prev = 0, stepCount = 0;
unsigned long previousMillis = 0;
const long interval = 1000;  // 1-second interval for resetting voltage display
unsigned long currentMillis;
unsigned long lastStepMillis = 0;

float v, vout, vin, totalVoltage = 0;
bool stepProcessed = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void clearSerialMonitor() {
  for (int i = 0; i < 50; i++) {
    Serial.println();
  }
  Serial.println("Serial Monitor Cleared");
  Serial.println("----------------------");
}

void setup() {
  Serial.begin(9600);
  clearSerialMonitor();

  pinMode(8, OUTPUT); // LED indication
  lcd.init();
  lcd.backlight();

  lcd.print("FOOT STEP POWER");
  lcd.setCursor(0, 1);
  lcd.print("   GENERATOR");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("STEP COUNT:");
  lcd.setCursor(0, 1);
  lcd.print("VOLTAGE:");
}

void loop() {
  v = analogRead(A0);
  currentMillis = millis();

  if (v != 0 && prev == 0) {
    stepCount += 1;
    digitalWrite(8, HIGH);
    stepProcessed = false;
    lastStepMillis = currentMillis;

    vout = (v * 5.00) / 1024.00;
    vin = (vout / 0.040909) * 100;
    totalVoltage += vin;

    lcd.setCursor(12, 0);
    lcd.print(stepCount);
    lcd.setCursor(9, 1);
    lcd.print(vin, 2);
    lcd.print("mV ");

    Serial.print("Step Count: ");
    Serial.print(stepCount);
    Serial.print(", Raw Analog Value: ");
    Serial.print(v); // Debugging raw analog value
    Serial.print(", Current Voltage: ");
    Serial.print(vin);
    Serial.print(" mV, Total Voltage: ");
    Serial.print(totalVoltage);
    Serial.println(" mV");
  } else {
    if (currentMillis - previousMillis >= 100) {
      previousMillis = currentMillis;
      digitalWrite(8, LOW);
    }
  }

  if (currentMillis - lastStepMillis >= interval && vin != 0) {
    vin = 0;
    lcd.setCursor(9, 1);
    lcd.print("  0.00 mV ");
  }

  prev = v;
  delay(200);
}
