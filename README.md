# ⚡🔥 NUCLEA – Advanced Portable Hardware Security Toolkit 🔥⚡

<p align="center">
  <b>Compact • Portable • Modular • Powerful</b><br>
  Inspired by <i>Flipper Zero</i> | Built for <b>Hackers & Researchers</b>
</p>

---

## 🚀✨ About NUCLEA

**NUCLEA** is an all-in-one, pocket-sized hardware cybersecurity platform designed for **ethical hacking**, **hardware security research**, and **hands-on cyber education**.

It combines multiple RF, IR, BLE, RFID, and USB attack & cloning modules into a single **stealth-grade cyber weapon** for laboratories, workshops, and demonstrations.

> 🧠 *“One Device. Unlimited Wireless Power.”*

---

## 🔥💣 Core Features

NUCLEA integrates multiple offensive & defensive modules:

* 🟦 **BLE Rubber Ducky (BadUSB over Bluetooth)**
* ⌨️ **USB Ducky Payload Executor**
* 📡 **RF Cloner (433 / 315 MHz)**
* 🔴 **IR Cloner & IR Universal Remote**
* 📶 **RF Generator & Signal Emulator**
* 📢 **Beacon Spam (Wi-Fi / BLE Flood)**
* 🪪 **RFID Cloner (125 kHz / 13.56 MHz)**
* 🚫 **RF Jammer (Research / Lab Mode)**
* 🔧 **Fully Modular Expansion Ports**
* 🌐 **Future Web Control Panel Support**

---

## 🧩⚙️ Hardware Platform

* 🧠 Main MCU: **ESP32 / ESP32-S3**
* 🖥 Display: **OLED (SPI / I2C)**
* 📡 Wireless: **nRF24L01+PA+LNA**
* 🔴 IR LED + IR Receiver
* 🪪 RFID Reader (MFRC522 / PN532)
* 🔋 Power: **Li-ion / Li-Po Battery + Regulation**
* 🧱 Enclosure: **Compact, Portable & Rugged Design**

---

## 💻🛠 Software Setup

### 1️⃣ Install Arduino IDE

Download from:

👉 [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)

Supported OS:

* 🐧 Linux
* 🪟 Windows
* 🍎 macOS

---

### 2️⃣ Add ESP32 Board Manager URL

Open:

`File → Preferences → Additional Board Manager URLs`

Paste:

```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

Click **OK** ✅

---

### 3️⃣ Install ESP32 Core (Version 2.0.16)

Go to:

`Tools → Board → Boards Manager`

Search:

```
esp32 by Espressif Systems
```

Select:

* Version: **2.0.16**
* Click: **Install** ⬇️

⚠️ NUCLEA firmware is **optimized and tested only on ESP32 core v2.0.16**

---

## 📚📦 Required Libraries

Install from **Arduino Library Manager**:

* 📚 `Adafruit GFX Library`
* 🖥 `Adafruit SSD1306` / `Adafruit SH1106G`
* 🟦 `ESP32 BLE Keyboard`
* 📡 `RF24 (TMRh20)`
* 🔴 `IRremoteESP8266`
* 🪪 `MFRC522`

Optional (Advanced Features):

* 🌐 `ESPAsyncWebServer`
* ⚡ `AsyncTCP`
* 🧩 `ArduinoJson`

---

## 🔌⚙️ Board Configuration

Recommended Settings:

* 🧠 Board: `ESP32 Dev Module` / `ESP32-S3 Dev Module`
* ⚡ Upload Speed: `921600`
* 💾 Flash Mode: `QIO`
* 🗂 Partition: `Default 4MB with SPIFFS`
* 🔌 Port: Select correct USB device

---

## ⚡🚀 Flashing Firmware

1. 🔌 Connect NUCLEA to PC
2. 📂 Open firmware `.ino` file
3. ⚙️ Select Board & Port
4. ⬆️ Click **Upload**

After flashing, OLED will show:

> 🔹 **NUCLEA Boot Logo**
> 🔹 **Main Control Menu**

---

## 🧭📋 Main Menu Modules

Typical firmware interface:

* 🟦 BLE Ducky
* ⌨️ USB Ducky
* 📡 RF Clone
* 🔴 IR Clone
* 📢 Beacon Spam
* 🪪 RFID Tools
* 🚫 RF Jam (Lab Mode)
* ⚙️ Settings & Calibration

---

## 🔐⚠️ Legal & Ethical Warning

> ❗ **IMPORTANT NOTICE**

NUCLEA is developed strictly for:

* 🎓 Cybersecurity education
* 🧪 Authorized penetration testing
* 🔬 Hardware security research

🚫 **Illegal use is strictly prohibited.**
You must have **explicit written permission** before testing any system.

The author & contributors are **not responsible** for misuse.

---

## 👨‍💻🏢 Author & Organization

Developed by:

**Muhammed Nihal MP**
Founder – **NEXLOR**
Cybersecurity | Hardware Hacking | IoT Security

🌐 *Building next-generation hacking tools & cyber labs.*

---

## 📌🧪 Project Status

* 🔹 Current Version: `NUCLEA v1.x`
* 🔹 Firmware: Actively Developing 🔧

### 🔮 Upcoming Features

* 🌐 Web Control Dashboard
* 📲 Mobile App Integration
* 🔄 OTA Firmware Updates
* 📍 GPS Tracking Module
* 🧩 Plugin-Based Module System

---

## ⭐🤝 Support & Contributions

* ⭐ Star this repository
* 🐛 Report bugs
* 💡 Suggest features
* 🔧 Submit pull requests

Let’s build the future of **hardware hacking** together.

---

<p align="center">
🔥 <b>NUCLEA – Pocket-Sized Cyber Power</b> 🔥
</p>
