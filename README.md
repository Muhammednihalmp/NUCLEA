# NUCLEA - ESP32 Security Tool v4.4

![ESP32](https://img.shields.io/badge/ESP32-DevKitC-blue)
![Version](https://img.shields.io/badge/Version-4.4-green)
![License](https://img.shields.io/badge/License-MIT-yellow)

**6-in-1 Security Testing Device** - RFID • RF • IR • BLE • WiFi • Signal Gen

---

## 🚀 Features

✅ **All Modules Working:**
- **RFID** - MFRC522 Reader/Writer/Emulator
- **RF Cloner** - 433MHz Capture & Replay  
- **IR Cloner** - Infrared Remote Learning
- **Bluetooth Ducky** - HID Keyboard Attacks
- **WiFi Beacon Spam** - Fake AP Flooding
- **RF Generator** - Custom Signal Generation

## 📦 Hardware

### Required Components:
1. ESP32 Dev Board
2. 128x64 OLED (SH1106, I2C)
3. MFRC522 RFID Module
4. 433MHz RF Transmitter & Receiver
5. IR Transmitter LED & VS1838B Receiver
6. 5x Tactile Buttons
7. Breadboard & Jumper Wires

### Pin Configuration:
```c
OLED:      SDA=GPIO21, SCL=GPIO22
RF:        TX=GPIO25, RX=GPIO26  
IR:        RX=GPIO27, TX=GPIO4
RFID:      SS=GPIO5, RST=GPIO17, MOSI=23, MISO=19, SCK=18
Buttons:   UP=13, DOWN=12, SELECT=14, BACK=15, START=16
LED:       GPIO2 (Built-in)
```

## ⚡ Quick Setup

### 1. Install Software
```arduino
Board URL: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
Board: ESP32 Dev Module (v2.0.16+)
Libraries: Adafruit_GFX, Adafruit_SH110X, BleKeyboard, IRremote, RCSwitch, MFRC522
```

### 2. Upload Code
```bash
git clone https://github.com/yourusername/nuclea.git
# Open nuc.ino in Arduino IDE
# Select ESP32 Dev Module
# Upload to ESP32
```

### 3. Wire Hardware
Follow pinout above. Power with 5V 1A USB.

## 🎮 Usage

### Navigation:
- **UP/DOWN**: Navigate menus
- **SELECT**: Enter/Clear slot
- **BACK**: Return to main menu
- **START/STOP**: Start action
- **Long Press START**: Clear all data

### Module Quick Start:
1. **RFID**: Read cards → Save to slots → Emulate
2. **RF Cloner**: Capture remote signals → Replay
3. **IR Cloner**: Learn IR remotes → Transmit
4. **BadUSB**: Connect via Bluetooth → Run scripts
5. **Beacon Spam**: Start to flood fake WiFi APs
6. **RF Gen**: Select frequency → Transmit signals

## 🔧 Troubleshooting

| Issue | Solution |
|-------|----------|
| OLED not working | Check I2C address (0x3C or 0x3D) |
| RFID not detecting | Verify SPI pins, check 3.3V power |
| RF not capturing | Ensure antenna connected, check wiring |
| BLE not connecting | Enable Bluetooth on target device |
| Buttons not working | Check pull-up resistors (10kΩ) |

## 📊 Specifications

- **Power**: 5V USB or 3.7V LiPo
- **Storage**: 4 slots per module (24 total)
- **Range**: RF ~100m, IR ~10m, BLE ~10m
- **Memory**: Flash storage persists after power off

## ⚠️ Legal Notice

**FOR AUTHORIZED TESTING ONLY**
- Use only on systems you own
- Check local laws for RF transmission
- Educational/research purposes only
- No illegal activities

## 📄 License

MIT License - See LICENSE file

---

## 📞 Support

**GitHub**: [Issues Page](https://github.com/yourusername/nuclea/issues)  
**Email**: support@nuclea.dev

---

**Built by Nihal MP**  
*Use responsibly. Test ethically.*
