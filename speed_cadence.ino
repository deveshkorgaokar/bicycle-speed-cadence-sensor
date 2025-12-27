#include <U8glib.h>

// Hall Effect sensor pins
const int speedSensorPin = 2;
const int cadenceSensorPin = 3;

// Variables for speed and cadence calculations
volatile unsigned long lastSpeedPulseTime = 0;
volatile unsigned long lastCadencePulseTime = 0;
volatile unsigned long speedPulseInterval = 0;
volatile unsigned long cadencePulseInterval = 0;

unsigned long lastSpeedUpdateTime = 0;
const unsigned long speedUpdateInterval = 500; // Update every 500ms

#define SPEED_TIMEOUT 2000 // 2 seconds timeout

// Wheel circumference in meters
const float wheelCircumference = 2.01061; //Change this value depending on tyre size.

volatile float speedKph = 0.0;
volatile float cadenceRpm = 0.0;
volatile float distanceKm = 0.0;

// OLED Display
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);

// Flags for cadence update
volatile bool updateCadence = false;

// ---------- Interrupt service routines ----------

void speedSensorISR() {
  unsigned long currentTime = millis();
  if (lastSpeedPulseTime > 0) {
    speedPulseInterval = currentTime - lastSpeedPulseTime;
  }
  lastSpeedPulseTime = currentTime;
}

void cadenceSensorISR() {
  unsigned long currentTime = millis();
  if (lastCadencePulseTime > 0) {
    cadencePulseInterval = currentTime - lastCadencePulseTime;
    updateCadence = true;
  }
  lastCadencePulseTime = currentTime;
}

void setup() {
  Serial.begin(9600);

  pinMode(speedSensorPin, INPUT_PULLUP);
  pinMode(cadenceSensorPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(speedSensorPin), speedSensorISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(cadenceSensorPin), cadenceSensorISR, FALLING);
}

void loop() {

  unsigned long currentTime = millis();

  // Reset speed when bike stops
  if (currentTime - lastSpeedPulseTime > SPEED_TIMEOUT) {
    speedKph = 0.0;
  } 
  else if (speedPulseInterval > 0) {
    speedKph = (wheelCircumference / (speedPulseInterval / 1000.0)) * 3.6;
  }

  // Update cadence
  if (updateCadence) {
    if (cadencePulseInterval > 0) {
      cadenceRpm = (60000.0 / cadencePulseInterval);
    } else {
      cadenceRpm = 0.0;
    }
    updateCadence = false;
  }

  // Distance accumulation (each second)
  static unsigned long lastDistanceUpdateTime = millis();
  if (currentTime - lastDistanceUpdateTime >= 1000) {
    distanceKm += (speedKph / 3600.0);
    lastDistanceUpdateTime = currentTime;
  }

  // ------- OLED update every second -------
  static unsigned long lastUpdateTime = millis();
  if (currentTime - lastUpdateTime >= 1000) {
    lastUpdateTime = currentTime;

    u8g.firstPage();
    do {
      u8g.setFont(u8g_font_gdr25);
      u8g.setPrintPos(25, 30);
      u8g.print(speedKph, 1);

      u8g.setFont(u8g_font_6x10);
      u8g.setPrintPos(95, 30);
      u8g.print("km/h");

      u8g.setPrintPos(2, 50);
      u8g.print("Cadence: ");
      u8g.print(cadenceRpm, 1);
      u8g.print(" rpm");

      u8g.setPrintPos(2, 60);
      u8g.print("Distance: ");
      u8g.print(distanceKm, 2);
      u8g.print(" km");

    } while (u8g.nextPage());
  }
}
