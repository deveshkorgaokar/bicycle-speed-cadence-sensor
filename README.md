# bicycle-speed-cadence-sensor
Bicycle speed and cadence meter using Arduino, hall sensors and OLED display.

# Bicycle Speed and Cadence Sensor (Arduino)

This project measures the speed of a bicycle wheel and the pedalling cadence using Hall-effect (or reed) sensors and displays the values on an OLED display. It is built using Arduino and inexpensive components and is suitable for DIY cycle computers.

## 🚴 What it measures

- Wheel speed (km/h)
- Cadence (pedal RPM)
- Trip distance
- Wheel circumference configurable in code

## 🧩 Hardware Used

- Arduino Nano
- 2× Hall-effect or magnetic reed sensors
- 2× small magnets
  - one on wheel spoke (speed)
  - one on pedal crank (cadence)
- 1.3" SSD1306 OLED display (I²C)
- Jumper wires
- battery pack

## 🧭 How it works

- A magnet passes near each Hall sensor
- Each pass creates a pulse → interrupt on Arduino
- Wheel pulses → calculate speed from time difference
- Crank pulses → calculate cadence (RPM)
- Results are displayed on the OLED screen

## 🪛 Connections

### Sensors

| Function | Arduino Pin |
|----------|-------------|
| Speed sensor signal | D2 |
| Cadence sensor signal | D3 |
| VCC | 5V |
| GND | GND |

### OLED Display (I²C)

| OLED Pin | Arduino Pin |
|----------|--------------|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

## ⚙️ Software

- Arduino IDE
- Libraries required:
  - Adafruit SSD1306
  - Adafruit GFX
  - Wire

## 📐 Important parameter

Wheel circumference:

```cpp
float wheelCircumference = 2.1;   // meters
````

Change this value depending on tyre size.

Examples:

* 26" MTB ≈ 2.07 m
* 27.5" MTB ≈ 2.19 m
* 700c road ≈ 2.10–2.15 m

## ▶️ Displayed values

* **Speed** in km/h
* **Cadence** in RPM
* **Distance** in km

## 🔮 Possible future upgrades

* Average speed
* Bluetooth to phone
* Battery voltage display
* Waterproof enclosure

## 📝 License

This project is free for learning and hobby use.
