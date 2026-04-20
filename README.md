# NUCLEA — ESP32 All-in-One Security Tool
**Creator:** Nihal MP | **Version:** 4.4

A multi-function security research and testing device built on the ESP32 platform, featuring an OLED display and hardware button navigation.

---

## ⚠️ Disclaimer
This tool is intended **for educational purposes and authorized security research only**. Use only on systems and devices you own or have explicit written permission to test. Unauthorized interception or transmission of signals may be illegal in your jurisdiction.

---

## Hardware Requirements

| Component | Details |
|-----------|---------|
| Microcontroller | ESP32 (any standard dev board) |
| Display | SH1106G 128×64 OLED (I2C) |
| RFID Reader | MFRC522 (SPI) |
| RF Module | 433 MHz TX/RX pair |
| IR Module | IR receiver + IR LED |
| Buttons | 5× tactile push buttons |

---

## Pin Configuration

### Buttons
| Function | GPIO |
|----------|------|
| UP | 13 |
| DOWN | 12 |
| SELECT | 14 |
| BACK | 15 |
| START/STOP | 16 |

### RF (433 MHz)
| Signal | GPIO |
|--------|------|
| TX | 25 |
| RX | 26 |

### IR
| Signal | GPIO |
|--------|------|
| Receive | 27 |
| Send | 4 |

### OLED (I2C)
| Signal | GPIO |
|--------|------|
| SDA | 21 |
| SCL | 22 |

### RC522 RFID (SPI)
| Signal | GPIO |
|--------|------|
| SS (SDA) | 5 |
| RST | 17 |
| MOSI | 23 |
| MISO | 19 |
| SCK | 18 |

---

## Features & Modules

### 1. Blu Ducky (BadUSB via Bluetooth)
Emulates a Bluetooth HID keyboard to execute pre-loaded Ducky Script-style payloads.
- 3 built-in scripts: Notepad Test, CMD Echo, Open Browser
- Bluetooth pairing name: `NUCLEA_Device`
- Requires BT connection before execution

### 2. RF Cloner
Captures and replays 433 MHz RF signals (e.g., garage remotes, key fobs).
- 4 save slots with persistent storage
- Supports multiple RCSwitch protocols
- Auto-repeating transmission

### 3. IR Cloner
Captures and replays infrared remote signals.
- 4 save slots with persistent storage
- Supports NEC, Samsung, Sony, RC5 protocols
- Auto-detects protocol on capture

### 4. RF Generator
Generates continuous RF carrier signals at selectable frequencies.
- 433 MHz, 315 MHz, 868 MHz, 2.4 GHz modes
- Toggle on/off with START/STOP button

### 5. Beacon Spam
Broadcasts multiple fake Wi-Fi SSIDs in rapid succession.
- 7 pre-configured SSID names
- Uses ESP32 SoftAP mode

### 6. RFID (RC522)
Reads and logs RFID/NFC card UIDs using the MFRC522 reader.
- 4 save slots with persistent storage
- Supports MIFARE and ISO 14443 card types
- Displays card type and UID on screen

---

## Navigation

| Button | Action |
|--------|--------|
| UP / DOWN | Navigate menu / change slot or mode |
| SELECT | Enter menu item / clear current slot |
| BACK | Return to main menu (saves all data) |
| START/STOP | Begin/stop the current module action |
| START/STOP (hold 1.5s) | **Clear ALL saved slots** |

---

## Libraries Required

Install via Arduino Library Manager or PlatformIO:

```
Adafruit GFX Library
Adafruit SH110X
ESP32 BLE Keyboard        (T-vK/ESP32-BLE-Keyboard)
IRremote
RCSwitch
MFRC522
```

---

## Building & Flashing

1. Install [Arduino IDE](https://www.arduino.cc/en/software) or [PlatformIO](https://platformio.org/)
2. Add ESP32 board support: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
3. Install all required libraries listed above
4. Select board: **ESP32 Dev Module**
5. Set upload speed: **115200**
6. Flash the `main.ino` / `main.cpp` file

---

## Serial Monitor
Connect at **115200 baud** to view debug output including RFID version checks, RF capture values, IR protocol details, and script execution logs.

---

## Persistent Storage
All captured RF, IR, and RFID slots are saved to ESP32 NVMe flash using the `Preferences` library and survive power cycles. Long-pressing START/STOP wipes all stored data.
