# 🚀 NUCLEA - ESP32 All-in-One Security Tool v4.4

<div align="center">

![NUCLEA Banner](https://img.shields.io/badge/NUCLEA-v4.4-blue?style=for-the-badge)
![ESP32](https://img.shields.io/badge/ESP32-DevKitC-orange?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Arduino-informational?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

**A Powerful Multi-Tool Security Device Built on ESP32**

*A Fully Integrated Security Testing Platform with RFID, RF, IR, BLE, and WiFi Capabilities*

</div>

## 🌟 Quick Overview
NUCLEA is an advanced ESP32-based security testing platform that combines **6 essential security testing modules** into a single, portable device. Designed for **ethical hackers, security researchers, and penetration testers**, it provides a comprehensive toolkit for wireless security assessments, physical security testing, and hardware hacking demonstrations.

---

## ✨ Key Features

### ✅ **Core Modules - All Tested & Working**

| Module | Status | Icon | Description |
|--------|--------|------|-------------|
| **Bluetooth Ducky** | ✅ WORKING | 🔵 | HID attacks via BLE Keyboard |
| **RF Cloner (433MHz)** | ✅ WORKING | 📡 | Signal capture & replay |
| **IR Cloner** | ✅ WORKING | 🔴 | Infrared remote cloning |
| **RF Generator** | ✅ WORKING | 📶 | Custom RF signal generation |
| **WiFi Beacon Spam** | ✅ WORKING | 📶 | Fake AP beacon flooding |
| **RFID Reader/Writer** | ✅ WORKING | 🎫 | MFRC522 NFC/RFID operations |

### 🚀 **Advanced Capabilities**
- **6-in-1 Integrated System**: All modules in one device
- **128x64 OLED Display**: Graphical user interface
- **Slot-Based Memory**: Save up to 4 signals per module
- **Battery-Powered**: Portable field operation
- **Real-time Processing**: Instant signal analysis
- **Data Persistence**: Flash storage for saved data

---

## 🛠️ Hardware Requirements

### 📋 **Essential Components List**

| Component | Quantity | Purpose | Purchase Link |
|-----------|----------|---------|---------------|
| ESP32 Dev Board | 1 | Main Controller | [Amazon](https://www.amazon.com/s?k=esp32) |
| 128x64 OLED (SH1106) | 1 | Display | [Amazon](https://www.amazon.com/s?k=sh1106+oled) |
| RF 433MHz Transmitter | 1 | RF Transmission | [Amazon](https://www.amazon.com/s?k=433mhz+transmitter) |
| RF 433MHz Receiver | 1 | RF Reception | [Amazon](https://www.amazon.com/s?k=433mhz+receiver) |
| IR Transmitter LED | 1 | IR Transmission | [Amazon](https://www.amazon.com/s?k=ir+led+940nm) |
| IR Receiver (VS1838B) | 1 | IR Reception | [Amazon](https://www.amazon.com/s?k=vs1838b) |
| MFRC522 RFID Module | 1 | RFID Operations | [Amazon](https://www.amazon.com/s?k=mfrc522) |
| Tactile Buttons (6x6mm) | 5 | Navigation | [Amazon](https://www.amazon.com/s?k=tactile+buttons) |
| Breadboard | 1 | Prototyping | [Amazon](https://www.amazon.com/s?k=breadboard) |
| Jumper Wires | 1 pack | Connections | [Amazon](https://www.amazon.com/s?k=jumper+wires) |
| Micro USB Cable | 1 | Power/Programming | [Amazon](https://www.amazon.com/s?k=micro+usb+cable) |

### 💰 **Estimated Cost**: $25-$40 USD

---

## ⚡ Quick Start Guide

### 1️⃣ **Software Installation (5 Minutes)**

```bash
# 1. Install Arduino IDE (if not installed)
# Download from: https://www.arduino.cc/en/software

# 2. Add ESP32 Board Manager URL:
# File → Preferences → Additional Board Manager URLs
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

# 3. Install ESP32 Platform:
# Tools → Board → Boards Manager → Search "ESP32"
# Install version 2.0.16 or newer
# Select: ESP32 Dev Module
```

### 2️⃣ **Install Required Libraries**

In Arduino IDE, go to **Sketch → Include Library → Manage Libraries** and install:

```arduino
1. Adafruit GFX Library (by Adafruit)
2. Adafruit SH110X (by Adafruit)
3. BleKeyboard (by T-vK)
4. IRremote (by Armin Joachimsmeyer)
5. RCSwitch (by sui77)
6. MFRC522 (by miguelbalboa)
```

### 3️⃣ **Hardware Assembly**

![Wiring Diagram](https://raw.githubusercontent.com/yourusername/nuclea/main/docs/wiring_diagram.png)

```c
// PINOUT CONFIGURATION (ESP32 ↔ Modules)
OLED_SDA       → GPIO21
OLED_SCL       → GPIO22

RF_TX          → GPIO25
RF_RX          → GPIO26

IR_RECEIVE     → GPIO27
IR_SEND        → GPIO4

RFID_SS        → GPIO5
RFID_RST       → GPIO17
RFID_MOSI      → GPIO23
RFID_MISO      → GPIO19
RFID_SCK       → GPIO18

BUTTON_UP      → GPIO13
BUTTON_DOWN    → GPIO12
BUTTON_SELECT  → GPIO14
BUTTON_BACK    → GPIO15
BUTTON_START   → GPIO16

LED_BUILTIN    → GPIO2
```

### 4️⃣ **Upload Code**

```bash
# 1. Clone repository or download nuc.ino
git clone https://github.com/yourusername/nuclea.git

# 2. Open nuc.ino in Arduino IDE
# 3. Select Board: ESP32 Dev Module
# 4. Select COM Port
# 5. Click Upload
```

### 5️⃣ **First Boot**

After uploading:
1. **Serial Monitor** will show initialization status
2. **OLED** will display boot animation
3. **System** enters Main Menu
4. **Ready to use!**

---

## 🎮 User Interface & Navigation

### 📱 **OLED Display Layout**

```
┌─────────────────────────────────────┐
│          MAIN MENU           ▲      │
│                                │    │
│   > Blu Ducky                  │    │
│     RF Clone                   │    │
│     IR Clone                   ▼    │
│     RF Gen                          │
│     Beacon                          │
│     RFID                            │
└─────────────────────────────────────┘
```

### 🎯 **Button Controls**

| Button | Short Press | Long Press (1.5s) |
|--------|-------------|-------------------|
| **UP** | Navigate Up | - |
| **DOWN** | Navigate Down | - |
| **SELECT** | Enter/Confirm | Clear Slot |
| **BACK** | Back to Menu | - |
| **START/STOP** | Start/Stop Action | **Clear All Data** |

---

## 🔧 Module-Specific Usage

### 1. **Bluetooth Ducky (BadUSB)**
```arduino
// Preloaded Scripts:
// 1. Notepad Test - Opens notepad and types message
// 2. CMD Echo - Opens command prompt and executes echo
// 3. Open Browser - Opens browser to specified URL

// Usage:
// 1. Select "Blu Ducky" from menu
// 2. Pair with target device (NUCLEA_Device)
// 3. Select script with UP/DOWN
// 4. Press START/STOP to execute
```

### 2. **RF Cloner (433MHz)**
```arduino
// Capture RF Signals:
// 1. Select "RF Clone" from menu
// 2. Point RF receiver toward remote
// 3. Press START/STOP to record
// 4. Press remote button to capture signal
// 5. Signal saved to selected slot

// Transmit RF Signals:
// 1. Select slot with saved signal
// 2. Press START/STOP to transmit
// 3. Signal will repeat every 300ms
```

### 3. **IR Cloner**
```arduino
// Supported Protocols:
// - NEC, Samsung, Sony, RC5
// - Most consumer IR remotes

// Capture IR Signals:
// 1. Select "IR Clone" from menu
// 2. Point IR receiver at remote
// 3. Press START/STOP to record
// 4. Press remote button to capture
// 5. Signal saved to selected slot

// Transmit IR Signals:
// 1. Select slot with saved signal
// 2. Press START/STOP to transmit
```

### 4. **RF Generator**
```arduino
// Available Frequencies:
// 1. 433 MHz (Default for remotes)
// 2. 315 MHz (Car key fobs)
// 3. 868 MHz (European devices)
// 4. 2.4 GHz (Simulated)

// Usage:
// 1. Select "RF Gen" from menu
// 2. Select frequency with UP/DOWN
// 3. Press START/STOP to toggle transmission
```

### 5. **WiFi Beacon Spam**
```arduino
// Preloaded SSIDs:
// - OFFENSO HACK
// - FBI Surveillance
// - Starbucks WiFi
// - Airport Free WiFi
// - Hotel Guest
// - Virus Alert
// - NUCLEA

// Usage:
// 1. Select "Beacon" from menu
// 2. Press START/STOP to begin
// 3. Device will cycle through SSIDs
// 4. Can be detected by WiFi scanners
```

### 6. **RFID Module (MFRC522)**
```arduino
// Supported Cards:
// - Mifare Classic 1K/4K
// - Mifare Ultralight
// - ISO/IEC 14443 Type A cards

// Read RFID Tag:
// 1. Select "RFID" from menu
// 2. Select empty slot
// 3. Press START/STOP to start reading
// 4. Tap card on reader
// 5. UID and type will be saved

// Emulate RFID Tag:
// 1. Select slot with saved tag
// 2. Press START/STOP to emulate
// 3. Card UID will be emulated
```

---

## 🔌 Pin Configuration Details

### **Complete Wiring Table**

| ESP32 Pin | Component | Color Code | Notes |
|-----------|-----------|------------|-------|
| **GPIO21** | OLED SDA | Yellow | I2C Data |
| **GPIO22** | OLED SCL | Green | I2C Clock |
| **GPIO25** | RF TX | Red | 433MHz Transmitter |
| **GPIO26** | RF RX | Blue | 433MHz Receiver |
| **GPIO27** | IR RX | Orange | IR Receiver Data |
| **GPIO4** | IR TX | Purple | IR Transmitter |
| **GPIO5** | RFID SS (SDA) | Brown | Chip Select |
| **GPIO17** | RFID RST | Gray | Reset |
| **GPIO23** | RFID MOSI | Pink | SPI Data Out |
| **GPIO19** | RFID MISO | White | SPI Data In |
| **GPIO18** | RFID SCK | Cyan | SPI Clock |
| **GPIO13** | Button UP | Black | Pull-up 10KΩ |
| **GPIO12** | Button DOWN | Black | Pull-up 10KΩ |
| **GPIO14** | Button SELECT | Black | Pull-up 10KΩ |
| **GPIO15** | Button BACK | Black | Pull-up 10KΩ |
| **GPIO16** | Button START | Black | Pull-up 10KΩ |
| **3.3V** | All Modules | Red | Power |
| **GND** | All Modules | Black | Ground |

---

## 📁 Project Structure

```bash
nuclea/
├── nuc.ino                    # Main source code
├── libraries/                 # Required libraries
│   ├── Adafruit_GFX/         # Graphics library
│   ├── Adafruit_SH110X/      # OLED driver
│   ├── BleKeyboard/          # Bluetooth HID
│   ├── IRremote/             # IR control
│   ├── RCSwitch/             # RF control
│   └── MFRC522/              # RFID driver
├── docs/
│   ├── schematics/           # Circuit diagrams
│   ├── pinouts/              # Pin configuration
│   └── tutorials/            # How-to guides
├── examples/
│   ├── badusb_scripts/       # DuckyScript examples
│   ├── rf_signals/           # RF signal database
│   └── ir_codes/             # IR code library
└── README.md                 # This file
```

---

## 🚀 Advanced Features

### **Slot Management**
```arduino
// Each module has 4 memory slots
// Save signals for later use
// Clear individual slots with SELECT button
// Clear all slots with START/STOP long press
```

### **Data Persistence**
```arduino
// Signals are saved to ESP32 flash
// Survives power cycles
// Uses Preferences library
// Automatic save/load on boot
```

### **Visual Feedback**
```arduino
// Built-in LED (GPIO2) provides status:
// - Slow blink: System idle
// - Fast blink: Active operation
// - Solid: Error/Alert
```

---

## 🔧 Troubleshooting Guide

### ❗ **Common Issues & Solutions**

| Problem | Solution | Icon |
|---------|----------|------|
| **OLED Not Displaying** | Check I2C address (0x3C), Verify wiring, Try 0x3D | 🖥️ |
| **RF Not Capturing** | Check antenna connection, Ensure 5V power, Adjust distance | 📡 |
| **IR Not Working** | Ensure line-of-sight, Check LED polarity, Test with phone camera | 🔴 |
| **RFID Not Detecting** | Verify SPI connections, Check card type, Ensure proper power | 🎫 |
| **BLE Not Connecting** | Enable Bluetooth on target, Check pairing, Restart BLE | 🔵 |
| **Buttons Unresponsive** | Check pull-up resistors, Verify GPIO assignments | ⚡ |
| **Memory Full** | Clear slots, Reduce stored signals | 💾 |

### 📈 **Debugging Steps**

1. **Enable Serial Monitor** (115200 baud)
2. **Check initialization messages**
3. **Test each module individually**
4. **Verify power supply** (minimum 5V 1A)
5. **Check for loose connections**

### 🛠️ **RFID-Specific Troubleshooting**

If RFID module isn't working:

```arduino
// Common issues:
1. SPI pin mismatch - Verify all 5 connections
2. Power issues - MFRC522 needs 3.3V
3. Antenna connection - Ensure coil is connected
4. Card compatibility - Test with different cards

// Quick test code:
void testRFID() {
  if (mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("Card detected!");
    // Additional testing...
  }
}
```

---

## 📊 Technical Specifications

### **Power Requirements**
- **Operating Voltage**: 3.3V - 5V DC
- **Current Draw**: 80-500mA (peak during transmission)
- **Recommended Supply**: 5V 1A USB power bank
- **Battery Life**: 4-6 hours (with 2000mAh battery)

### **Wireless Specifications**

| Module | Frequency | Range | Power Consumption |
|--------|-----------|-------|-------------------|
| Bluetooth | 2.4GHz | 10m | 12mA |
| RF (433MHz) | 433.92MHz | 100m | 28mA (TX) |
| IR | 940nm | 10m | 20mA (TX) |
| WiFi | 2.4GHz | 50m | 130mA |
| RFID | 13.56MHz | 5cm | 13mA |

### **Memory & Storage**
- **Flash Usage**: ~1.2MB (30% of 4MB)
- **RAM Usage**: ~200KB (38% of 520KB)
- **Storage**: Preferences-based (up to 64KB)
- **Slot Memory**: 4 slots × 6 modules = 24 total saves

### **Physical Characteristics**
- **Dimensions**: 100mm × 70mm × 25mm (with case)
- **Weight**: ~85g (without battery)
- **Display**: 128×64 monochrome OLED
- **Interface**: 5-button navigation

---

## ⚠️ Legal & Ethical Usage

### **📜 IMPORTANT DISCLAIMER**

🚨 **NUCLEA IS FOR AUTHORIZED SECURITY TESTING AND EDUCATIONAL PURPOSES ONLY**

### **✅ Authorized Uses**
- Security research and education
- Authorized penetration testing
- Classroom demonstrations
- Personal learning and skill development
- Security conference demonstrations

### **❌ Prohibited Uses**
- Unauthorized access to systems/networks
- Disrupting critical infrastructure
- Privacy invasion
- Commercial exploitation without permission
- Any illegal activities

### **🌍 Legal Compliance**
- Check local laws regarding RF transmission
- Only test on systems you own or have permission to test
- Respect privacy and data protection laws
- Follow ethical hacking guidelines

---

## 🤝 Contributing

We welcome contributions from the community! Here's how you can help:

### **Ways to Contribute**
1. **Report Bugs** - Create detailed bug reports
2. **Suggest Features** - Propose new modules or improvements
3. **Submit Code** - Fix bugs or add features
4. **Improve Documentation** - Help others get started
5. **Create Tutorials** - Share your knowledge

### **Pull Request Process**
```bash
1. Fork the repository
2. Create a feature branch: git checkout -b feature/amazing-feature
3. Commit changes: git commit -m 'Add amazing feature'
4. Push to branch: git push origin feature/amazing-feature
5. Open a Pull Request
```

### **Coding Standards**
- Follow existing code style
- Add comments for complex logic
- Test thoroughly before submitting
- Update documentation as needed

---

## 📚 Learning Resources

### **Recommended Reading**
- **ESP32 Official Documentation**: https://docs.espressif.com/
- **Arduino Programming Guide**: https://www.arduino.cc/reference/en/
- **RFID Security**: https://www.rfidsecurity.org/
- **Wireless Security**: https://www.wi-fi.org/

### **Video Tutorials**
- ESP32 Beginner's Guide
- RFID Hacking Basics
- RF Signal Analysis
- Bluetooth Security Testing

### **Community & Support**
- **Discord Server**: [Join our community](https://discord.gg/nuclea)
- **GitHub Issues**: Report bugs and request features
- **Email Support**: support@nuclea.dev
- **Twitter**: @NUCLEA_Device

---

## 📞 Support & Contact

### **Getting Help**
1. **Check Documentation**: Most issues are covered here
2. **Search Issues**: Check if your issue was already reported
3. **Community Support**: Join our Discord for help
4. **Email**: For serious bugs or security issues

### **Contact Information**
- **Creator**: Nihal MP
- **Email**: nihal@nuclea.dev
- **Website**: https://nuclea.dev
- **GitHub**: https://github.com/yourusername/nuclea

### **Business Inquiries**
For commercial use, licensing, or partnerships, please contact: business@nuclea.dev

---

## 📜 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

```text
MIT License

Copyright (c) 2024 Nihal MP

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## ✨ Acknowledgements

### **Special Thanks To**
- **ESP32 Community** - For amazing hardware support
- **Arduino Team** - For the development platform
- **Library Developers** - For all the amazing libraries
- **Testers & Contributors** - For making NUCLEA better

### **Open Source Projects Used**
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [BleKeyboard](https://github.com/T-vK/ESP32-BLE-Keyboard)
- [IRremote](https://github.com/Arduino-IRremote/Arduino-IRremote)
- [RCSwitch](https://github.com/sui77/rc-switch)
- [MFRC522](https://github.com/miguelbalboa/rfid)

### **Inspiration**
- Hak5 Tools
- Flipper Zero
- Pwnagotchi
- ESP32 Marauder

---

<div align="center">

## ⭐ **If you find this project useful, please give it a star!**

**Built with ❤️ by Nihal MP**

📧 **Contact**: nihal@nuclea.dev  
🌐 **Website**: https://nuclea.dev  
🐦 **Twitter**: @NUCLEA_Device  
💬 **Discord**: [Join our community](https://discord.gg/nuclea)

🚀 **Happy Ethical Hacking!** 🔓✨

*Remember: With great power comes great responsibility. Use this tool ethically and legally.*

</div>

---

## 📊 Project Statistics

![GitHub stars](https://img.shields.io/github/stars/yourusername/nuclea?style=social)
![GitHub forks](https://img.shields.io/github/forks/yourusername/nuclea?style=social)
![GitHub issues](https://img.shields.io/github/issues/yourusername/nuclea)
![GitHub last commit](https://img.shields.io/github/last-commit/yourusername/nuclea)
![License](https://img.shields.io/github/license/yourusername/nuclea)

---

### 🎯 **TL;DR Quick Start**
1. **Install Arduino IDE + ESP32 boards**
2. **Connect hardware as per pinout**
3. **Upload `nuc.ino`**
4. **Use buttons to navigate menus**
5. **Start testing (responsibly!)**

### 🔄 **Update History**
- **v4.4**: Fixed RFID module, added emulation
- **v4.3**: Improved BLE stability
- **v4.2**: Added signal slots
- **v4.1**: Initial release with all modules

---

**Note**: This project is constantly evolving. Check back regularly for updates, new features, and improvements!
