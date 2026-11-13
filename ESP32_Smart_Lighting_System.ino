#include <BluetoothSerial.h>
BluetoothSerial SerialBT;

const int RELAY_PIN = 23;
const int LDR_PIN   = 4;
const int PIR_PIN   = 22;

// Modes
const int AUTO_MODE   = 3;
const int MANUAL_MODE = 2;

int mode         = AUTO_MODE;
int manualState  = LOW;   // tracks what manual mode wants

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Bluetooth Light Bulb System");

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LDR_PIN,   INPUT);
  pinMode(PIR_PIN,   INPUT);

  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  // --- Handle Bluetooth commands ---
  if (SerialBT.available()) {
    char cmd = (char)SerialBT.read();
    Serial.print("CMD received: ");
    Serial.println(cmd);

    switch (cmd) {

      case '2':   // Switch to MANUAL mode
        mode = MANUAL_MODE;
        // Apply last known manual state immediately
        digitalWrite(RELAY_PIN, manualState);
        break;

      case '3':   // Switch to AUTO mode
        mode = AUTO_MODE;
        break;

      case '1':   // Manual ON
        if (mode == MANUAL_MODE) {
          manualState = HIGH;
          digitalWrite(RELAY_PIN, HIGH);
        }
        break;

      case '0':   // Manual OFF
        if (mode == MANUAL_MODE) {
          manualState = LOW;
          digitalWrite(RELAY_PIN, LOW);
        }
        break;
    }
  }

  // --- AUTO mode logic (only runs when in AUTO mode) ---
  if (mode == AUTO_MODE) {
    int motion = digitalRead(PIR_PIN);
    int light  = digitalRead(LDR_PIN);

    // Turn ON only when: dark (LDR LOW) AND motion detected (PIR HIGH)
    if (motion == HIGH && light == HIGH) {
      digitalWrite(RELAY_PIN, HIGH);
    } else {
      digitalWrite(RELAY_PIN, LOW);
    }
      delay(5000); // small debounce delay

  }
}