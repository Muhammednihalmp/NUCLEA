# NUCLEA

**Portable Hardware Security Toolkit**
Compact • Modular • Research‑Grade

---

## Overview

NUCLEA is a compact, all‑in‑one hardware platform for **ethical hacking**, **hardware security research**, and **cybersecurity education**.
Inspired by devices like Flipper Zero, NUCLEA integrates multiple RF, IR, BLE, RFID, and USB attack and testing modules into a single portable tool.

Designed for:

* Security researchers
* Penetration testers
* Cyber labs & workshops
* Academic demonstrations

---

## Features

* BLE Rubber Ducky (BadUSB over Bluetooth)
* USB Ducky (HID keyboard injection)
* RF Cloner (315 / 433 MHz)
* IR Cloner & Universal Remote
* Beacon Spam (Wi‑Fi / BLE)
* RFID Cloner (125 kHz / 13.56 MHz)
* RF Jammer (research mode)
* Modular expansion ports

---

## Hardware Platform

* MCU: ESP32 / ESP32‑S3
* Display: OLED (SPI / I2C)
* Radio: nRF24L01+ PA + LNA
* IR transmitter & receiver
* RFID reader (MFRC522 / PN532)
* Power: Li‑ion / Li‑Po battery

---

## Pin Connections

The following pin mapping is the **default reference configuration** for NUCLEA (ESP32 / ESP32‑S3).
You may change GPIO assignments in firmware if required.

### Core Connections

| Module / Signal | ESP32 GPIO | Notes                 |
| --------------- | ---------- | --------------------- |
| OLED VCC        | 3.3V       | Power (do NOT use 5V) |
| OLED GND        | GND        | Ground                |
| OLED SCK (D0)   | GPIO 18    | SPI Clock             |
| OLED MOSI (D1)  | GPIO 23    | SPI Data              |
| OLED CS         | GPIO 5     | Chip Select           |
| OLED DC         | GPIO 4     | Data / Command        |
| OLED RST        | GPIO 17    | Reset                 |

---

### nRF24L01+ (RF Module)

| Signal | ESP32 GPIO | Notes                                   |
| ------ | ---------- | --------------------------------------- |
| VCC    | 3.3V       | Use stable 3.3V (recommended capacitor) |
| GND    | GND        | Ground                                  |
| CE     | GPIO 27    | Chip Enable                             |
| CSN    | GPIO 15    | SPI Chip Select                         |
| SCK    | GPIO 18    | Shared SPI Clock                        |
| MOSI   | GPIO 23    | Shared SPI MOSI                         |
| MISO   | GPIO 19    | Shared SPI MISO                         |

---

### RFID (MFRC522)

| Signal   | ESP32 GPIO | Notes       |
| -------- | ---------- | ----------- |
| SDA (SS) | GPIO 21    | Chip Select |
| SCK      | GPIO 18    | SPI Clock   |
| MOSI     | GPIO 23    | SPI Data    |
| MISO     | GPIO 19    | SPI Data    |
| RST      | GPIO 22    | Reset       |
| VCC      | 3.3V       | Power       |
| GND      | GND        | Ground      |

---

### IR Module

| Signal | ESP32 GPIO | Notes             |
| ------ | ---------- | ----------------- |
| IR TX  | GPIO 26    | IR LED output     |
| IR RX  | GPIO 25    | IR receiver input |

---

### User Controls

| Signal      | ESP32 GPIO | Notes                   |
| ----------- | ---------- | ----------------------- |
| Button UP   | GPIO 32    | Menu navigation         |
| Button DOWN | GPIO 33    | Menu navigation         |
| Button OK   | GPIO 34    | Select / Enter          |
| Buzzer      | GPIO 14    | Optional sound feedback |

---

⚠️ **Important Notes**

* Always power RF and OLED modules from **3.3V only**
* Add a **10µF – 47µF capacitor** near nRF24L01 VCC for stability
* Avoid using boot‑critical pins (GPIO 0, 2, 12, 15) unless necessary

---

## Software Setup

### 1. Install Arduino IDE

Download from:
[https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)

---

### 2. Add ESP32 Board URL

Open:

`File → Preferences → Additional Board Manager URLs`

Add:

```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

---

### 3. Install ESP32 Core (Recommended Version)

`Tools → Board → Boards Manager`

Search:

```
esp32 by Espressif Systems
```

Install:

* Version: **2.0.16**

---

## Required Libraries

Install from Arduino Library Manager:

* Adafruit GFX Library
* Adafruit SSD1306 or Adafruit SH1106G
* ESP32 BLE Keyboard
* RF24 (TMRh20)
* IRremoteESP8266
* MFRC522

Optional:

* ESPAsyncWebServer
* AsyncTCP
* ArduinoJson

---

## Board Configuration

Recommended Arduino settings:

* Board: ESP32 Dev Module / ESP32‑S3 Dev Module
* Upload Speed: 921600
* Flash Mode: QIO
* Partition Scheme: Default 4MB with SPIFFS

---

## Flashing Firmware

1. Connect NUCLEA to PC via USB
2. Open the firmware `.ino` file
3. Select the correct board and port
4. Click **Upload**

After flashing, the OLED displays the boot screen and main menu.

---

## Main Menu

* BLE Ducky
* USB Ducky
* RF Clone
* IR Clone
* Beacon Spam
* RFID Tools
* RF Jam (Lab Mode)
* Settings

---

## Legal Notice

This project is intended only for:

* Education
* Authorized penetration testing
* Hardware security research

Do not use this device on systems or networks without explicit permission.
The author and contributors are not responsible for misuse.

---

## Author

**Muhammed Nihal MP**
Founder – **NEXLOR**
Cybersecurity | Hardware Hacking | IoT Security

---

## Project Status

* Current Version: NUCLEA v1.x
* Development: Active

Planned:

* Web control panel
* OTA updates
* Mobile app integration
* GPS module support

---

## License

This project is released for educational and research use.
See the LICENSE file for details.
