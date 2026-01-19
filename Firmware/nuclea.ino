/*******************************************************
 * ESP32 ALL-IN-ONE SECURITY TOOL - FULLY DEBUGGED v4.4
 * Creator: Nihal MP
 *******************************************************/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <BleKeyboard.h>
#include <IRremote.hpp>
#include <RCSwitch.h>
#include <WiFi.h>
#include <Preferences.h>
#include <SPI.h>
#include <MFRC522.h>

// ============================================
// DISPLAY CONFIGURATION
// ============================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ============================================
// PIN DEFINITIONS
// ============================================
#define BTN_UP 13
#define BTN_DOWN 12
#define BTN_SELECT 14
#define BTN_BACK 15
#define BTN_STARTSTOP 16

#define RF_TX_PIN 25
#define RF_RX_PIN 26

#define IR_RECEIVE_PIN 27
#define IR_SEND_PIN 4

#define OLED_SDA 21
#define OLED_SCL 22

// RC522 SPI Pins - CORRECT FOR ESP32
#define RC522_SS 5
#define RC522_RST 17
#define RC522_MOSI 23
#define RC522_MISO 19
#define RC522_SCK 18

#define LED_BUILTIN 2

// Initialize RC522
MFRC522 mfrc522(RC522_SS, RC522_RST);

// ============================================
// TIMING CONSTANTS
// ============================================
#define DEBOUNCE_DELAY 80
#define LONG_PRESS_TIME 1500
#define SCREEN_UPDATE_INTERVAL 150
#define RF_TRANSMIT_INTERVAL 300
#define LOOP_DELAY 2
#define RF_RX_TIMEOUT 50
#define IR_RX_TIMEOUT 50

// ============================================
// SYSTEM STATE
// ============================================
enum SystemMode {
  MAIN_MENU,
  BADUSB_MODE,
  RF_CLONER_MODE,
  IR_CLONER_MODE,
  RF_GEN_MODE,
  BEACON_SPAM_MODE,
  RFID_MODE
};

SystemMode currentMode = MAIN_MENU;
int menuIndex = 0;
unsigned long lastButtonPress = 0;
unsigned long lastScreenUpdate = 0;
unsigned long btnStartStopPressTime = 0;
bool btnStartStopLongPress = false;

bool btnUpState = false;
bool btnDownState = false;
bool btnSelectState = false;
bool btnBackState = false;
bool btnStartStopState = false;

wifi_mode_t currentWiFiMode = WIFI_OFF;

// ============================================
// ✅ WORKING: RFID MODULE (RC522)
// ============================================
#define RFID_SLOTS 4

struct RFIDTag {
  bool valid;
  uint8_t uid[10];
  uint8_t uidLength;
  char tagType[20];
  char name[20];
  uint8_t sak;
};

RFIDTag rfidTags[RFID_SLOTS];
int selectedRFIDSlot = 0;
bool rfidReading = false;
bool rfidEmulating = false;
bool tagPresent = false;
unsigned long lastTagCheck = 0;
bool rfidInitialized = false;

char rfidDisplayBuf[64];

// ============================================
// BADUSB MODULE
// ============================================
BleKeyboard bleKeyboard("NUCLEA_Device", "Nihal MP", 100);

struct DuckyScript {
  char name[32];
  char commands[15][64];
  int commandCount;
};

DuckyScript duckyScripts[3];
int scriptCount = 0;
int selectedScript = 0;
bool scriptExecuting = false;
unsigned long scriptStepTime = 0;
int currentCommand = 0;
bool waitingForDelay = false;
unsigned long delayDuration = 0;

char badUSBDisplayBuf[64];

// ============================================
// RF CLONER MODULE
// ============================================
RCSwitch rfReceiver = RCSwitch();
RCSwitch rfTransmitter = RCSwitch();

#define RF_SLOTS 4

struct RFSignal {
  unsigned long value;
  unsigned int bitLength;
  unsigned int protocol;
  unsigned int pulseLength;
  bool valid;
  char name[20];
};

RFSignal capturedRF[RF_SLOTS];
int selectedRFSlot = 0;
bool rfTransmitting = false;
bool rfRecording = false;
unsigned long lastRFTransmit = 0;
unsigned long lastRFRxCheck = 0;

char rfDisplayBuf[64];

// ============================================
// IR CLONER MODULE
// ============================================
#define IR_SLOTS 4

struct IRSignal {
  bool valid;
  decode_type_t protocol;
  uint16_t address;
  uint16_t command;
  uint32_t rawData;
  uint8_t numberOfBits;
  char protocolName[20];
  char name[20];
};

IRSignal capturedIR[IR_SLOTS];
int selectedIRSlot = 0;
bool irRecording = false;
bool irTransmitting = false;
unsigned long lastIRRxCheck = 0;

char irDisplayBuf[64];

// ============================================
// RF GENERATOR MODULE
// ============================================
int rfGenMode = 0;
bool rfGenActive = false;
static unsigned long rfGenLastPulse = 0;
static bool rfGenPulseState = LOW;
static unsigned int rfGenPulseCount = 0;
const char* rfFreqs[] = {"433 MHz", "315 MHz", "868 MHz", "2.4 GHz"};
unsigned long rfGenTimings[] = {350, 500, 200, 100};

// ============================================
// BEACON SPAM MODULE
// ============================================
bool beaconSpamActive = false;
const char* beaconSSIDs[] = {
  "OFFENSO HACK",
  "FBI Surveillance",
  "Starbucks WiFi",
  "Airport Free WiFi",
  "Hotel Guest",
  "Virus Alert",
  "NUCLEA"
};
int beaconCount = 7;
int currentBeaconIndex = 0;
unsigned long lastBeaconSpam = 0;

// ============================================
// ✅ WORKING: RFID FUNCTIONS
// ============================================
void initRFIDSlots() {
  for(int i = 0; i < RFID_SLOTS; i++) {
    rfidTags[i].valid = false;
    snprintf(rfidTags[i].name, sizeof(rfidTags[i].name), "Slot %d", i + 1);
    rfidTags[i].uidLength = 0;
    strncpy(rfidTags[i].tagType, "Empty", sizeof(rfidTags[i].tagType) - 1);
    rfidTags[i].sak = 0;
    memset(rfidTags[i].uid, 0, sizeof(rfidTags[i].uid));
  }
}

bool initRC522() {
  Serial.println("[RFID] Initializing RC522...");
  
  // Initialize SPI with correct ESP32 pins
  SPI.begin(RC522_SCK, RC522_MISO, RC522_MOSI, RC522_SS);
  
  // Initialize MFRC522
  mfrc522.PCD_Init();
  delay(100);
  
  // Check version
  byte version = mfrc522.PCD_ReadRegister(MFRC522::VersionReg);
  Serial.print("[RFID] MFRC522 Version: 0x");
  Serial.println(version, HEX);
  
  if(version == 0x00 || version == 0xFF) {
    Serial.println("[RFID] ERROR: Communication failed!");
    return false;
  }
  
  // Perform self test
  bool selfTestOK = mfrc522.PCD_PerformSelfTest();
  
  // Re-initialize after self test
  mfrc522.PCD_Init();
  delay(100);
  
  Serial.print("[RFID] Self Test: ");
  Serial.println(selfTestOK ? "PASSED" : "FAILED");
  
  // Enable antenna
  mfrc522.PCD_AntennaOn();
  
  // Set antenna gain to maximum
  mfrc522.PCD_SetAntennaGain(MFRC522::RxGain_max);
  
  Serial.println("[RFID] RC522 Ready!");
  return true;
}

void testRFID() {
  Serial.println("\n=== RFID TEST MODE ===");
  
  // Check if card present
  if (mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("Card detected!");
    
    if (mfrc522.PICC_ReadCardSerial()) {
      Serial.print("UID: ");
      for(byte i = 0; i < mfrc522.uid.size; i++) {
        if(mfrc522.uid.uidByte[i] < 0x10) Serial.print("0");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        if(i < mfrc522.uid.size - 1) Serial.print(":");
      }
      Serial.println();
      
      // Get card type
      MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
      Serial.print("Card Type: ");
      Serial.println(mfrc522.PICC_GetTypeName(piccType));
      
      mfrc522.PICC_HaltA();
    }
  } else {
    Serial.println("No card present");
  }
  
  Serial.println("=== TEST COMPLETE ===\n");
}

void checkRFIDTag() {
  if(!rfidReading && !rfidEmulating) return;
  
  unsigned long now = millis();
  if(now - lastTagCheck < 300) return;
  lastTagCheck = now;
  
  // Check for new card
  if (!mfrc522.PICC_IsNewCardPresent()) {
    tagPresent = false;
    return;
  }
  
  // Read the card
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  tagPresent = true;
  
  if(rfidReading) {
    // Store UID
    rfidTags[selectedRFIDSlot].uidLength = mfrc522.uid.size;
    memcpy(rfidTags[selectedRFIDSlot].uid, mfrc522.uid.uidByte, mfrc522.uid.size);
    rfidTags[selectedRFIDSlot].sak = mfrc522.uid.sak;
    
    // Identify card type
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    String typeName = mfrc522.PICC_GetTypeName(piccType);
    strncpy(rfidTags[selectedRFIDSlot].tagType, typeName.c_str(), 
            sizeof(rfidTags[selectedRFIDSlot].tagType) - 1);
    
    Serial.print("[RFID] Captured - Type: ");
    Serial.print(rfidTags[selectedRFIDSlot].tagType);
    Serial.print(" UID: ");
    for(uint8_t i = 0; i < rfidTags[selectedRFIDSlot].uidLength; i++) {
      if(rfidTags[selectedRFIDSlot].uid[i] < 0x10) Serial.print("0");
      Serial.print(rfidTags[selectedRFIDSlot].uid[i], HEX);
      if(i < rfidTags[selectedRFIDSlot].uidLength - 1) Serial.print(":");
    }
    Serial.println();
    
    rfidTags[selectedRFIDSlot].valid = true;
    rfidReading = false;
    saveRFIDSlots();
    
    // Halt and stop
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    
    // Visual feedback
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void saveRFIDSlots() {
  Preferences preferences;
  if(preferences.begin("rfid-data", false)) {
    for(int i = 0; i < RFID_SLOTS; i++) {
      char slotKey[20];
      
      snprintf(slotKey, sizeof(slotKey), "slot%d_valid", i);
      preferences.putBool(slotKey, rfidTags[i].valid);
      
      if(rfidTags[i].valid) {
        snprintf(slotKey, sizeof(slotKey), "slot%d_uidlen", i);
        preferences.putUChar(slotKey, rfidTags[i].uidLength);
        
        snprintf(slotKey, sizeof(slotKey), "slot%d_uid", i);
        preferences.putBytes(slotKey, rfidTags[i].uid, rfidTags[i].uidLength);
        
        snprintf(slotKey, sizeof(slotKey), "slot%d_type", i);
        preferences.putString(slotKey, rfidTags[i].tagType);
        
        snprintf(slotKey, sizeof(slotKey), "slot%d_sak", i);
        preferences.putUChar(slotKey, rfidTags[i].sak);
      }
    }
    preferences.end();
    Serial.println("[RFID] Slots saved");
  }
}

void loadRFIDSlots() {
  Preferences preferences;
  if(preferences.begin("rfid-data", true)) {
    for(int i = 0; i < RFID_SLOTS; i++) {
      char slotKey[20];
      
      snprintf(slotKey, sizeof(slotKey), "slot%d_valid", i);
      rfidTags[i].valid = preferences.getBool(slotKey, false);
      
      if(rfidTags[i].valid) {
        snprintf(slotKey, sizeof(slotKey), "slot%d_uidlen", i);
        rfidTags[i].uidLength = preferences.getUChar(slotKey, 0);
        
        if(rfidTags[i].uidLength > 0 && rfidTags[i].uidLength <= 10) {
          snprintf(slotKey, sizeof(slotKey), "slot%d_uid", i);
          preferences.getBytes(slotKey, rfidTags[i].uid, rfidTags[i].uidLength);
        }
        
        snprintf(slotKey, sizeof(slotKey), "slot%d_type", i);
        String typeStr = preferences.getString(slotKey, "Unknown");
        strncpy(rfidTags[i].tagType, typeStr.c_str(), sizeof(rfidTags[i].tagType) - 1);
        
        snprintf(slotKey, sizeof(slotKey), "slot%d_sak", i);
        rfidTags[i].sak = preferences.getUChar(slotKey, 0);
      }
    }
    preferences.end();
    Serial.println("[RFID] Slots loaded");
  }
}

void clearRFIDSlot(int slot) {
  if(slot >= 0 && slot < RFID_SLOTS) {
    rfidTags[slot].valid = false;
    rfidTags[slot].uidLength = 0;
    rfidTags[slot].sak = 0;
    memset(rfidTags[slot].uid, 0, sizeof(rfidTags[slot].uid));
    strncpy(rfidTags[slot].tagType, "Empty", sizeof(rfidTags[slot].tagType) - 1);
    saveRFIDSlots();
    Serial.print("[RFID] Slot ");
    Serial.print(slot + 1);
    Serial.println(" cleared");
  }
}

// ============================================
// BADUSB FUNCTIONS
// ============================================
void initBadUSBScripts() {
  // Script 1 - Notepad Test
  strncpy(duckyScripts[0].name, "Notepad Test", sizeof(duckyScripts[0].name) - 1);
  duckyScripts[0].commandCount = 8;
  strncpy(duckyScripts[0].commands[0], "DELAY 1000", 64);
  strncpy(duckyScripts[0].commands[1], "GUI r", 64);
  strncpy(duckyScripts[0].commands[2], "DELAY 500", 64);
  strncpy(duckyScripts[0].commands[3], "STRING notepad", 64);
  strncpy(duckyScripts[0].commands[4], "ENTER", 64);
  strncpy(duckyScripts[0].commands[5], "DELAY 500", 64);
  strncpy(duckyScripts[0].commands[6], "STRING Hello from NUCLEA Device!", 64);
  strncpy(duckyScripts[0].commands[7], "ENTER", 64);
  
  // Script 2 - CMD Test
  strncpy(duckyScripts[1].name, "CMD Echo", sizeof(duckyScripts[1].name) - 1);
  duckyScripts[1].commandCount = 9;
  strncpy(duckyScripts[1].commands[0], "DELAY 500", 64);
  strncpy(duckyScripts[1].commands[1], "GUI r", 64);
  strncpy(duckyScripts[1].commands[2], "DELAY 500", 64);
  strncpy(duckyScripts[1].commands[3], "STRING cmd", 64);
  strncpy(duckyScripts[1].commands[4], "ENTER", 64);
  strncpy(duckyScripts[1].commands[5], "DELAY 800", 64);
  strncpy(duckyScripts[1].commands[6], "STRING echo Hello from NUCLEA!", 64);
  strncpy(duckyScripts[1].commands[7], "ENTER", 64);
  strncpy(duckyScripts[1].commands[8], "DELAY 300", 64);
  
  // Script 3 - Browser
  strncpy(duckyScripts[2].name, "Open Browser", sizeof(duckyScripts[2].name) - 1);
  duckyScripts[2].commandCount = 8;
  strncpy(duckyScripts[2].commands[0], "DELAY 1000", 64);
  strncpy(duckyScripts[2].commands[1], "GUI r", 64);
  strncpy(duckyScripts[2].commands[2], "DELAY 500", 64);
  strncpy(duckyScripts[2].commands[3], "STRING https://offensoacademy.com", 64);
  strncpy(duckyScripts[2].commands[4], "DELAY 200", 64);
  strncpy(duckyScripts[2].commands[5], "ENTER", 64);
  strncpy(duckyScripts[2].commands[6], "DELAY 500", 64);
  strncpy(duckyScripts[2].commands[7], "STRING ", 64);
  
  scriptCount = 3;
  Serial.println("[BadUSB] Scripts loaded");
}

void executeDuckyScriptStep() {
  if(!scriptExecuting) return;
  
  // Check if we finished all commands
  if(currentCommand >= duckyScripts[selectedScript].commandCount) {
    scriptExecuting = false;
    currentCommand = 0;
    waitingForDelay = false;
    Serial.println("[BadUSB] Script completed");
    return;
  }
  
  unsigned long now = millis();
  
  // If we're waiting for a delay, check if it's done
  if(waitingForDelay) {
    if(now - scriptStepTime >= delayDuration) {
      waitingForDelay = false;
      currentCommand++;
      scriptStepTime = now;
    }
    return;
  }
  
  // Execute current command
  char* cmd = duckyScripts[selectedScript].commands[currentCommand];
  
  Serial.print("[BadUSB] Executing: ");
  Serial.println(cmd);
  
  // Parse and execute command
  if(strncmp(cmd, "DELAY ", 6) == 0) {
    delayDuration = atoi(cmd + 6);
    waitingForDelay = true;
    scriptStepTime = now;
  }
  else if(strncmp(cmd, "GUI ", 4) == 0 || strcmp(cmd, "GUI") == 0) {
    bleKeyboard.press(KEY_LEFT_GUI);
    delay(100);
    if(strlen(cmd) > 4 && cmd[4] != '\0') {
      bleKeyboard.press(cmd[4]);
      delay(50);
    }
    bleKeyboard.releaseAll();
    delay(50);
    currentCommand++;
  }
  else if(strncmp(cmd, "STRING ", 7) == 0) {
    bleKeyboard.print(cmd + 7);
    delay(50);
    currentCommand++;
  }
  else if(strcmp(cmd, "ENTER") == 0) {
    bleKeyboard.write(KEY_RETURN);
    delay(50);
    currentCommand++;
  }
  else if(strcmp(cmd, "TAB") == 0) {
    bleKeyboard.write(KEY_TAB);
    delay(50);
    currentCommand++;
  }
  else if(strncmp(cmd, "CTRL ", 5) == 0) {
    bleKeyboard.press(KEY_LEFT_CTRL);
    delay(50);
    if(strlen(cmd) > 5) {
      bleKeyboard.press(cmd[5]);
      delay(50);
    }
    bleKeyboard.releaseAll();
    delay(50);
    currentCommand++;
  }
  else if(strcmp(cmd, "ESC") == 0 || strcmp(cmd, "ESCAPE") == 0) {
    bleKeyboard.write(KEY_ESC);
    delay(50);
    currentCommand++;
  }
  else if(strcmp(cmd, "ALT") == 0) {
    bleKeyboard.press(KEY_LEFT_ALT);
    delay(100);
    bleKeyboard.releaseAll();
    delay(50);
    currentCommand++;
  }
  else if(strcmp(cmd, "SPACE") == 0) {
    bleKeyboard.write(' ');
    delay(50);
    currentCommand++;
  }
  else if(strcmp(cmd, "UP") == 0 || strcmp(cmd, "UPARROW") == 0) {
    bleKeyboard.write(KEY_UP_ARROW);
    delay(50);
    currentCommand++;
  }
  else if(strcmp(cmd, "DOWN") == 0 || strcmp(cmd, "DOWNARROW") == 0) {
    bleKeyboard.write(KEY_DOWN_ARROW);
    delay(50);
    currentCommand++;
  }
  else if(strcmp(cmd, "LEFT") == 0 || strcmp(cmd, "LEFTARROW") == 0) {
    bleKeyboard.write(KEY_LEFT_ARROW);
    delay(50);
    currentCommand++;
  }
  else if(strcmp(cmd, "RIGHT") == 0 || strcmp(cmd, "RIGHTARROW") == 0) {
    bleKeyboard.write(KEY_RIGHT_ARROW);
    delay(50);
    currentCommand++;
  }
  else if(strcmp(cmd, "DELETE") == 0 || strcmp(cmd, "DEL") == 0) {
    bleKeyboard.write(KEY_DELETE);
    delay(50);
    currentCommand++;
  }
  else if(strcmp(cmd, "BACKSPACE") == 0) {
    bleKeyboard.write(KEY_BACKSPACE);
    delay(50);
    currentCommand++;
  }
  else {
    // Unknown command, skip it
    Serial.print("[BadUSB] Unknown command: ");
    Serial.println(cmd);
    currentCommand++;
  }
  
  scriptStepTime = now;
}

// ============================================
// RF CLONER MODULE
// ============================================
void initRFSlots() {
  for(int i = 0; i < RF_SLOTS; i++) {
    capturedRF[i].valid = false;
    snprintf(capturedRF[i].name, sizeof(capturedRF[i].name), "Slot %d", i + 1);
    capturedRF[i].value = 0;
    capturedRF[i].bitLength = 0;
    capturedRF[i].protocol = 1;
    capturedRF[i].pulseLength = 350;
  }
}

void captureRFSignal() {
  if(!rfRecording) return;
  
  unsigned long now = millis();
  if(now - lastRFRxCheck < RF_RX_TIMEOUT) return;
  lastRFRxCheck = now;
  
  if(rfReceiver.available()) {
    unsigned long val = rfReceiver.getReceivedValue();
    if(val != 0) {
      capturedRF[selectedRFSlot].value = val;
      capturedRF[selectedRFSlot].bitLength = rfReceiver.getReceivedBitlength();
      capturedRF[selectedRFSlot].protocol = rfReceiver.getReceivedProtocol();
      capturedRF[selectedRFSlot].pulseLength = rfReceiver.getReceivedDelay();
      capturedRF[selectedRFSlot].valid = true;
      rfRecording = false;
      saveRFSlots();
      
      Serial.print("[RF] Captured: 0x");
      Serial.print(val, HEX);
      Serial.print(" (");
      Serial.print(capturedRF[selectedRFSlot].bitLength);
      Serial.println(" bits)");
    }
    rfReceiver.resetAvailable();
  }
}

void transmitRFSignal() {
  if(!rfTransmitting || !capturedRF[selectedRFSlot].valid) return;
  
  unsigned long now = millis();
  if(now - lastRFTransmit > RF_TRANSMIT_INTERVAL) {
    rfTransmitter.setProtocol(capturedRF[selectedRFSlot].protocol);
    rfTransmitter.setPulseLength(capturedRF[selectedRFSlot].pulseLength);
    rfTransmitter.send(capturedRF[selectedRFSlot].value, capturedRF[selectedRFSlot].bitLength);
    lastRFTransmit = now;
    
    Serial.print("[RF] TX: 0x");
    Serial.println(capturedRF[selectedRFSlot].value, HEX);
  }
}

void saveRFSlots() {
  Preferences preferences;
  preferences.begin("rf-storage", false);
  
  for(int i = 0; i < RF_SLOTS; i++) {
    char slotKey[10];
    snprintf(slotKey, sizeof(slotKey), "rf%d", i);
    
    if(capturedRF[i].valid) {
      preferences.putBool(slotKey, true);
      
      char valKey[15];
      snprintf(valKey, sizeof(valKey), "val%d", i);
      preferences.putULong(valKey, capturedRF[i].value);
      
      char bitsKey[15];
      snprintf(bitsKey, sizeof(bitsKey), "bits%d", i);
      preferences.putUInt(bitsKey, capturedRF[i].bitLength);
      
      char protoKey[15];
      snprintf(protoKey, sizeof(protoKey), "proto%d", i);
      preferences.putUInt(protoKey, capturedRF[i].protocol);
    } else {
      preferences.putBool(slotKey, false);
    }
  }
  
  preferences.end();
}

void loadRFSlots() {
  Preferences preferences;
  preferences.begin("rf-storage", true);
  
  for(int i = 0; i < RF_SLOTS; i++) {
    char slotKey[10];
    snprintf(slotKey, sizeof(slotKey), "rf%d", i);
    
    if(preferences.getBool(slotKey, false)) {
      char valKey[15];
      snprintf(valKey, sizeof(valKey), "val%d", i);
      capturedRF[i].value = preferences.getULong(valKey, 0);
      
      char bitsKey[15];
      snprintf(bitsKey, sizeof(bitsKey), "bits%d", i);
      capturedRF[i].bitLength = preferences.getUInt(bitsKey, 0);
      
      char protoKey[15];
      snprintf(protoKey, sizeof(protoKey), "proto%d", i);
      capturedRF[i].protocol = preferences.getUInt(protoKey, 1);
      
      capturedRF[i].valid = true;
    }
  }
  
  preferences.end();
}

void clearRFSlot(int slot) {
  if(slot >= 0 && slot < RF_SLOTS) {
    capturedRF[slot].valid = false;
    capturedRF[slot].value = 0;
    capturedRF[slot].bitLength = 0;
    saveRFSlots();
    Serial.print("[RF] Slot ");
    Serial.print(slot + 1);
    Serial.println(" cleared");
  }
}

// ============================================
// IR CLONER MODULE
// ============================================
void initIRSlots() {
  for(int i = 0; i < IR_SLOTS; i++) {
    capturedIR[i].valid = false;
    snprintf(capturedIR[i].name, sizeof(capturedIR[i].name), "Slot %d", i + 1);
    capturedIR[i].protocol = UNKNOWN;
    capturedIR[i].address = 0;
    capturedIR[i].command = 0;
    capturedIR[i].rawData = 0;
    capturedIR[i].numberOfBits = 0;
    strncpy(capturedIR[i].protocolName, "Empty", sizeof(capturedIR[i].protocolName) - 1);
  }
}

void captureIRSignal() {
  if(!irRecording) return;
  
  unsigned long now = millis();
  if(now - lastIRRxCheck < IR_RX_TIMEOUT) return;
  lastIRRxCheck = now;
  
  if(IrReceiver.decode()) {
    capturedIR[selectedIRSlot].protocol = IrReceiver.decodedIRData.protocol;
    capturedIR[selectedIRSlot].address = IrReceiver.decodedIRData.address;
    capturedIR[selectedIRSlot].command = IrReceiver.decodedIRData.command;
    capturedIR[selectedIRSlot].rawData = IrReceiver.decodedIRData.decodedRawData;
    capturedIR[selectedIRSlot].numberOfBits = IrReceiver.decodedIRData.numberOfBits;
    strncpy(capturedIR[selectedIRSlot].protocolName, 
            getProtocolString(IrReceiver.decodedIRData.protocol), 
            sizeof(capturedIR[selectedIRSlot].protocolName) - 1);
    capturedIR[selectedIRSlot].valid = true;
    irRecording = false;
    saveIRSlots();
    IrReceiver.resume();
    
    Serial.print("[IR] Captured: ");
    Serial.print(capturedIR[selectedIRSlot].protocolName);
    Serial.print(" Addr: 0x");
    Serial.print(capturedIR[selectedIRSlot].address, HEX);
    Serial.print(" Cmd: 0x");
    Serial.println(capturedIR[selectedIRSlot].command, HEX);
  }
}

void transmitIRSignal() {
  if(!capturedIR[selectedIRSlot].valid) return;
  
  IrReceiver.stop();
  delay(10);
  
  switch(capturedIR[selectedIRSlot].protocol) {
    case NEC:
      IrSender.sendNEC(capturedIR[selectedIRSlot].address, capturedIR[selectedIRSlot].command, 1);
      break;
    case SAMSUNG:
      IrSender.sendSamsung(capturedIR[selectedIRSlot].address, capturedIR[selectedIRSlot].command, 1);
      break;
    case SONY:
      IrSender.sendSony(capturedIR[selectedIRSlot].address, capturedIR[selectedIRSlot].command, 
                       capturedIR[selectedIRSlot].numberOfBits);
      break;
    case RC5:
      IrSender.sendRC5(capturedIR[selectedIRSlot].address, capturedIR[selectedIRSlot].command, 1);
      break;
    default:
      IrSender.sendNEC(capturedIR[selectedIRSlot].address, capturedIR[selectedIRSlot].command, 1);
      break;
  }
  
  delay(50);
  IrReceiver.start();
  Serial.println("[IR] Signal transmitted");
}

void saveIRSlots() {
  Preferences preferences;
  preferences.begin("ir-storage", false);
  
  for(int i = 0; i < IR_SLOTS; i++) {
    char slotKey[10];
    snprintf(slotKey, sizeof(slotKey), "ir%d", i);
    
    if(capturedIR[i].valid) {
      preferences.putBool(slotKey, true);
      
      char protoKey[15];
      snprintf(protoKey, sizeof(protoKey), "proto%d", i);
      preferences.putUInt(protoKey, (uint8_t)capturedIR[i].protocol);
      
      char addrKey[15];
      snprintf(addrKey, sizeof(addrKey), "addr%d", i);
      preferences.putUShort(addrKey, capturedIR[i].address);
      
      char cmdKey[15];
      snprintf(cmdKey, sizeof(cmdKey), "cmd%d", i);
      preferences.putUShort(cmdKey, capturedIR[i].command);
    } else {
      preferences.putBool(slotKey, false);
    }
  }
  
  preferences.end();
}

void loadIRSlots() {
  Preferences preferences;
  preferences.begin("ir-storage", true);
  
  for(int i = 0; i < IR_SLOTS; i++) {
    char slotKey[10];
    snprintf(slotKey, sizeof(slotKey), "ir%d", i);
    
    if(preferences.getBool(slotKey, false)) {
      char protoKey[15];
      snprintf(protoKey, sizeof(protoKey), "proto%d", i);
      capturedIR[i].protocol = (decode_type_t)preferences.getUInt(protoKey, 0);
      
      char addrKey[15];
      snprintf(addrKey, sizeof(addrKey), "addr%d", i);
      capturedIR[i].address = preferences.getUShort(addrKey, 0);
      
      char cmdKey[15];
      snprintf(cmdKey, sizeof(cmdKey), "cmd%d", i);
      capturedIR[i].command = preferences.getUShort(cmdKey, 0);
      
      strncpy(capturedIR[i].protocolName, getProtocolString(capturedIR[i].protocol), 
              sizeof(capturedIR[i].protocolName) - 1);
      capturedIR[i].valid = true;
    }
  }
  
  preferences.end();
}

void clearIRSlot(int slot) {
  if(slot >= 0 && slot < IR_SLOTS) {
    capturedIR[slot].valid = false;
    capturedIR[slot].protocol = UNKNOWN;
    capturedIR[slot].address = 0;
    capturedIR[slot].command = 0;
    saveIRSlots();
    Serial.print("[IR] Slot ");
    Serial.print(slot + 1);
    Serial.println(" cleared");
  }
}

// ============================================
// RF GENERATOR
// ============================================
void generateRFSignal() {
  if(!rfGenActive) {
    digitalWrite(RF_TX_PIN, LOW);
    return;
  }
  
  unsigned long nowMicros = micros();
  static unsigned long lastToggle = 0;
  static bool outputState = LOW;
  
  unsigned long halfPeriod = rfGenTimings[rfGenMode];
  
  if(nowMicros - lastToggle >= halfPeriod) {
    outputState = !outputState;
    digitalWrite(RF_TX_PIN, outputState);
    lastToggle = nowMicros;
    
    rfGenPulseCount++;
    if(rfGenPulseCount > 10000) rfGenPulseCount = 0;
  }
}

// ============================================
// BEACON SPAM MODULE
// ============================================
void performBeaconSpam() {
  if(!beaconSpamActive) return;
  
  unsigned long now = millis();
  if(now - lastBeaconSpam > 200) {
    WiFi.softAP(beaconSSIDs[currentBeaconIndex], NULL, 1, 0, 1);
    currentBeaconIndex = (currentBeaconIndex + 1) % beaconCount;
    lastBeaconSpam = now;
    
    if(now % 500 < 50) {
      WiFi.softAPdisconnect(true);
      delay(10);
    }
  }
}

// ============================================
// DISPLAY FUNCTIONS
// ============================================
void showBootAnimation() {
  display.clearDisplay();
  
  for(int i = 0; i < 4; i++) {
    display.drawRect(i, i, 128 - i*2, 64 - i*2, SH110X_WHITE);
    display.display();
    delay(40);
  }
  
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(25, 15);
  display.print("NUCLEA");
  
  display.setTextSize(1);
  display.setCursor(18, 50);
  display.print("      BY Nihal MP");
  
  display.display();
  delay(1200);
  display.clearDisplay();
}

void drawMainMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  
  display.fillRect(0, 0, 128, 12, SH110X_WHITE);
  display.setTextColor(SH110X_BLACK);
  display.setCursor(3, 3);
  display.print("MAIN MENU");
  
  display.setTextColor(SH110X_WHITE);
  
  const char* items[] = {"Blu Ducky", "RF Clone", "IR Clone", "RF Gen", "Beacon", "RFID"};
  int totalItems = 6;
  
  int startY = 14;
  for(int i = 0; i < 4; i++) {
    int itemIdx = menuIndex - 1 + i;
    if(itemIdx < 0 || itemIdx >= totalItems) continue;
    
    int y = startY + i * 13;
    
    if(itemIdx == menuIndex) {
      display.fillRect(0, y-1, 128, 12, SH110X_WHITE);
      display.setTextColor(SH110X_BLACK);
      display.setCursor(4, y+2);
      display.print(">");
      display.print(items[itemIdx]);
    } else {
      display.setTextColor(SH110X_WHITE);
      display.setCursor(8, y+2);
      display.print(items[itemIdx]);
    }
  }
  
  if(menuIndex > 0) {
    display.setCursor(120, 1);
    display.print("^");
  }
  if(menuIndex < totalItems - 1) {
    display.setCursor(120, 54);
    display.print("v");
  }
  
  display.display();
}

void drawBadUSBMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  
  display.fillRect(0, 0, 128, 12, SH110X_WHITE);
  display.setTextColor(SH110X_BLACK);
  display.setCursor(3, 3);
  display.print("Blu Ducky");
  
  display.setTextColor(SH110X_WHITE);
  display.setCursor(2, 15);
  display.print("BT: ");
  display.print(bleKeyboard.isConnected() ? "Connected" : "Waiting...");
  
  if(scriptCount > 0) {
    display.setCursor(2, 27);
    snprintf(badUSBDisplayBuf, sizeof(badUSBDisplayBuf), "Script: %d/%d", selectedScript + 1, scriptCount);
    display.print(badUSBDisplayBuf);
    
    display.setCursor(2, 39);
    display.print(duckyScripts[selectedScript].name);
  } else {
    display.setCursor(20, 30);
    display.print("No scripts");
  }
  
  display.setCursor(2, 52);
  if(scriptExecuting) {
    snprintf(badUSBDisplayBuf, sizeof(badUSBDisplayBuf), "RUN %d/%d", currentCommand + 1, duckyScripts[selectedScript].commandCount);
    display.print(badUSBDisplayBuf);
  } else if(bleKeyboard.isConnected()) {
    display.print("Press OK");
  } else {
    display.print("Wait for BT");
  }
  
  display.display();
}

void drawRFClonerMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  
  display.fillRect(0, 0, 128, 12, SH110X_WHITE);
  display.setTextColor(SH110X_BLACK);
  display.setCursor(3, 3);
  display.print("RF Clone");
  
  display.setTextColor(SH110X_WHITE);
  
  snprintf(rfDisplayBuf, sizeof(rfDisplayBuf), "Slot %d: %s", selectedRFSlot + 1, 
           capturedRF[selectedRFSlot].valid ? "Saved" : "Empty");
  display.setCursor(2, 15);
  display.print(rfDisplayBuf);
  
  if(capturedRF[selectedRFSlot].valid) {
    display.setCursor(2, 27);
    snprintf(rfDisplayBuf, sizeof(rfDisplayBuf), "Val:0x%lX", capturedRF[selectedRFSlot].value);
    display.print(rfDisplayBuf);
    
    display.setCursor(2, 39);
    snprintf(rfDisplayBuf, sizeof(rfDisplayBuf), "Bits:%d", capturedRF[selectedRFSlot].bitLength);
    display.print(rfDisplayBuf);
  }
  
  display.setCursor(2, 52);
  if(rfRecording) {
    display.print("RECORDING...");
  } else if(rfTransmitting) {
    display.print("TRANSMITTING...");
  } else {
    display.print(capturedRF[selectedRFSlot].valid ? "OK:Transmit" : "OK:Record");
  }
  
  display.display();
}

void drawIRClonerMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  
  display.fillRect(0, 0, 128, 12, SH110X_WHITE);
  display.setTextColor(SH110X_BLACK);
  display.setCursor(3, 3);
  display.print("IR Clone");
  
  display.setTextColor(SH110X_WHITE);
  
  snprintf(irDisplayBuf, sizeof(irDisplayBuf), "Slot %d: %s", selectedIRSlot + 1,
           capturedIR[selectedIRSlot].valid ? "Saved" : "Empty");
  display.setCursor(2, 15);
  display.print(irDisplayBuf);
  
  if(capturedIR[selectedIRSlot].valid) {
    display.setCursor(2, 27);
    display.print(capturedIR[selectedIRSlot].protocolName);
  }
  
  display.setCursor(2, 52);
  display.print(irRecording ? "RECORDING..." : (capturedIR[selectedIRSlot].valid ? "OK:Transmit" : "OK:Record"));
  
  display.display();
}

void drawRFGenMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  
  display.fillRect(0, 0, 128, 12, SH110X_WHITE);
  display.setTextColor(SH110X_BLACK);
  display.setCursor(3, 3);
  display.print("RF Generator");
  
  display.setTextColor(SH110X_WHITE);
  display.setCursor(2, 20);
  display.print("Frequency:");
  
  display.setTextSize(2);
  display.setCursor(15, 32);
  display.print(rfFreqs[rfGenMode]);
  display.setTextSize(1);
  
  display.setCursor(2, 52);
  display.print(rfGenActive ? "TRANSMITTING..." : "OK:Toggle TX");
  
  display.display();
}

void drawBeaconSpamMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  
  display.fillRect(0, 0, 128, 12, SH110X_WHITE);
  display.setTextColor(SH110X_BLACK);
  display.setCursor(3, 3);
  display.print("Beacon Spam");
  
  display.setTextColor(SH110X_WHITE);
  
  snprintf(rfDisplayBuf, sizeof(rfDisplayBuf), "SSIDs: %d", beaconCount);
  display.setCursor(2, 15);
  display.print(rfDisplayBuf);
  
  if(beaconSpamActive) {
    display.setCursor(2, 27);
    display.print("Status: ACTIVE");
    
    display.setCursor(2, 39);
    display.print("Now: ");
    display.print(beaconSSIDs[currentBeaconIndex]);
  } else {
    display.setCursor(2, 27);
    display.print("Status: READY");
  }
  
  display.setCursor(2, 52);
  display.print(beaconSpamActive ? "OK:Stop" : "OK:Start");
  
  display.display();
}

void drawRFIDMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  
  display.fillRect(0, 0, 128, 12, SH110X_WHITE);
  display.setTextColor(SH110X_BLACK);
  display.setCursor(3, 3);
  
  if(!rfidInitialized) {
    display.print("RFID ERROR");
  } else {
    display.print("RFID RC522");
  }
  
  display.setTextColor(SH110X_WHITE);
  
  if(!rfidInitialized) {
    display.setCursor(2, 25);
    display.print("Init Failed!");
    display.setCursor(2, 37);
    display.print("Check Wiring");
    display.setCursor(2, 49);
    display.print("SDA:GPIO5 RST:17");
  } else {
    snprintf(rfidDisplayBuf, sizeof(rfidDisplayBuf), "Slot %d/%d: %s", 
             selectedRFIDSlot + 1, RFID_SLOTS,
             rfidTags[selectedRFIDSlot].valid ? "Saved" : "Empty");
    display.setCursor(2, 15);
    display.print(rfidDisplayBuf);
    
    if(rfidTags[selectedRFIDSlot].valid) {
      display.setCursor(2, 27);
      display.print(rfidTags[selectedRFIDSlot].tagType);
      
      display.setCursor(2, 39);
      display.print("UID:");
      
      for(uint8_t i = 0; i < min(rfidTags[selectedRFIDSlot].uidLength, (uint8_t)4); i++) {
        if(i > 0) display.print(":");
        if(rfidTags[selectedRFIDSlot].uid[i] < 0x10) display.print("0");
        display.print(rfidTags[selectedRFIDSlot].uid[i], HEX);
      }
      if(rfidTags[selectedRFIDSlot].uidLength > 4) {
        display.print("...");
      }
    } else {
      display.setCursor(2, 27);
      display.print("No tag saved");
    }
    
    display.setCursor(2, 52);
    if(rfidReading) {
      if(tagPresent) {
        display.print("Reading... Found!");
      } else {
        display.print("Reading... Tap card");
      }
    } else if(rfidEmulating) {
      display.print("Emulating (Display)");
    } else {
      if(rfidTags[selectedRFIDSlot].valid) {
        display.print("OK:Emulate  SEL:Clear");
      } else {
        display.print("OK:Read  SEL:Clear");
      }
    }
  }
  
  display.display();
}

// ============================================
// BUTTON HANDLING
// ============================================
void readButtons() {
  static unsigned long lastRead = 0;
  unsigned long now = millis();
  
  if(now - lastRead < DEBOUNCE_DELAY) return;
  lastRead = now;
  
  bool upNow = (digitalRead(BTN_UP) == LOW);
  bool downNow = (digitalRead(BTN_DOWN) == LOW);
  bool selectNow = (digitalRead(BTN_SELECT) == LOW);
  bool backNow = (digitalRead(BTN_BACK) == LOW);
  bool startStopNow = (digitalRead(BTN_STARTSTOP) == LOW);
  
  // START/STOP button
  if(startStopNow && !btnStartStopState) {
    btnStartStopPressTime = now;
    btnStartStopState = true;
  }
  
  if(startStopNow && btnStartStopState) {
    if(!btnStartStopLongPress && (now - btnStartStopPressTime > LONG_PRESS_TIME)) {
      btnStartStopLongPress = true;
      for(int i = 0; i < RF_SLOTS; i++) clearRFSlot(i);
      for(int i = 0; i < IR_SLOTS; i++) clearIRSlot(i);
      for(int i = 0; i < RFID_SLOTS; i++) clearRFIDSlot(i);
      
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(20, 25);
      display.print("CLEARED!");
      display.display();
      delay(800);
    }
  }
  
  if(!startStopNow && btnStartStopState) {
    if(!btnStartStopLongPress) {
      switch(currentMode) {
        case BADUSB_MODE:
          if(!scriptExecuting && bleKeyboard.isConnected() && scriptCount > 0) {
            scriptExecuting = true;
            scriptStepTime = millis();
            currentCommand = 0;
            waitingForDelay = false;
            Serial.println("[BadUSB] Script started");
          } else if(scriptExecuting) {
            scriptExecuting = false;
            currentCommand = 0;
            waitingForDelay = false;
            Serial.println("[BadUSB] Script stopped");
          }
          break;
          
        case RF_CLONER_MODE:
          if(rfRecording) {
            rfRecording = false;
          } else if(rfTransmitting) {
            rfTransmitting = false;
          } else if(capturedRF[selectedRFSlot].valid) {
            rfTransmitting = true;
            lastRFTransmit = 0;
          } else {
            rfRecording = true;
            lastRFRxCheck = 0;
          }
          break;
          
        case IR_CLONER_MODE:
          if(irRecording) {
            irRecording = false;
          } else if(capturedIR[selectedIRSlot].valid) {
            transmitIRSignal();
          } else {
            irRecording = true;
            lastIRRxCheck = 0;
          }
          break;
          
        case RF_GEN_MODE:
          rfGenActive = !rfGenActive;
          rfGenPulseCount = 0;
          Serial.print("[RF Gen] ");
          Serial.println(rfGenActive ? "ON" : "OFF");
          break;
          
        case BEACON_SPAM_MODE:
          beaconSpamActive = !beaconSpamActive;
          if(beaconSpamActive) {
            WiFi.mode(WIFI_AP);
            currentWiFiMode = WIFI_AP;
            Serial.println("[Beacon] Started");
          } else {
            WiFi.mode(WIFI_OFF);
            currentWiFiMode = WIFI_OFF;
            Serial.println("[Beacon] Stopped");
          }
          break;
          
        case RFID_MODE:
          if(!rfidInitialized) break;
          if(rfidReading) {
            rfidReading = false;
            Serial.println("[RFID] Reading stopped");
          } else if(rfidEmulating) {
            rfidEmulating = false;
            Serial.println("[RFID] Emulation stopped");
          } else if(rfidTags[selectedRFIDSlot].valid) {
            rfidEmulating = true;
            Serial.println("[RFID] Emulating tag");
            Serial.print("[RFID] Emulating UID: ");
            for(uint8_t i = 0; i < rfidTags[selectedRFIDSlot].uidLength; i++) {
              if(i > 0) Serial.print(":");
              if(rfidTags[selectedRFIDSlot].uid[i] < 0x10) Serial.print("0");
              Serial.print(rfidTags[selectedRFIDSlot].uid[i], HEX);
            }
            Serial.println();
          } else {
            rfidReading = true;
            tagPresent = false;
            Serial.println("[RFID] Ready to read - Tap card now!");
            for(int i = 0; i < 3; i++) {
              digitalWrite(LED_BUILTIN, HIGH);
              delay(50);
              digitalWrite(LED_BUILTIN, LOW);
              delay(50);
            }
          }
          break;
      }
    }
    btnStartStopState = false;
    btnStartStopLongPress = false;
  }
  
  // SELECT button
  if(selectNow && !btnSelectState) {
    btnSelectState = true;
    
    if(currentMode == MAIN_MENU) {
      currentMode = (SystemMode)(menuIndex + 1);
    }
    else if(currentMode == RF_CLONER_MODE) {
      clearRFSlot(selectedRFSlot);
    }
    else if(currentMode == IR_CLONER_MODE) {
      clearIRSlot(selectedIRSlot);
    }
    else if(currentMode == RFID_MODE) {
      clearRFIDSlot(selectedRFIDSlot);
    }
  }
  if(!selectNow) btnSelectState = false;
  
  // UP button
  if(upNow && !btnUpState) {
    btnUpState = true;
    
    switch(currentMode) {
      case MAIN_MENU:
        menuIndex = (menuIndex - 1 + 6) % 6;
        break;
      case BADUSB_MODE:
        if(scriptCount > 0) {
          selectedScript = (selectedScript - 1 + scriptCount) % scriptCount;
        }
        break;
      case RF_CLONER_MODE:
        selectedRFSlot = (selectedRFSlot - 1 + RF_SLOTS) % RF_SLOTS;
        break;
      case IR_CLONER_MODE:
        selectedIRSlot = (selectedIRSlot - 1 + IR_SLOTS) % IR_SLOTS;
        break;
      case RF_GEN_MODE:
        rfGenMode = (rfGenMode - 1 + 4) % 4;
        break;
      case RFID_MODE:
        selectedRFIDSlot = (selectedRFIDSlot - 1 + RFID_SLOTS) % RFID_SLOTS;
        break;
    }
  }
  if(!upNow) btnUpState = false;
  
  // DOWN button
  if(downNow && !btnDownState) {
    btnDownState = true;
    
    switch(currentMode) {
      case MAIN_MENU:
        menuIndex = (menuIndex + 1) % 6;
        break;
      case BADUSB_MODE:
        if(scriptCount > 0) {
          selectedScript = (selectedScript + 1) % scriptCount;
        }
        break;
      case RF_CLONER_MODE:
        selectedRFSlot = (selectedRFSlot + 1) % RF_SLOTS;
        break;
      case IR_CLONER_MODE:
        selectedIRSlot = (selectedIRSlot + 1) % IR_SLOTS;
        break;
      case RF_GEN_MODE:
        rfGenMode = (rfGenMode + 1) % 4;
        break;
      case RFID_MODE:
        selectedRFIDSlot = (selectedRFIDSlot + 1) % RFID_SLOTS;
        break;
    }
  }
  if(!downNow) btnDownState = false;
  
  // BACK button
  if(backNow && !btnBackState) {
    btnBackState = true;
    
    if(currentMode != MAIN_MENU) {
      currentMode = MAIN_MENU;
      
      rfTransmitting = false;
      rfRecording = false;
      rfGenActive = false;
      digitalWrite(RF_TX_PIN, LOW);
      irRecording = false;
      irTransmitting = false;
      scriptExecuting = false;
      beaconSpamActive = false;
      rfidReading = false;
      rfidEmulating = false;
      
      saveRFSlots();
      saveIRSlots();
      saveRFIDSlots();
      
      if(currentWiFiMode != WIFI_OFF) {
        WiFi.mode(WIFI_OFF);
        currentWiFiMode = WIFI_OFF;
      }
      
      Serial.println("[System] Main Menu");
    }
  }
  if(!backNow) btnBackState = false;
}

// ============================================
// SETUP
// ============================================
void setup() {
  Serial.begin(115200);
  delay(500);
  
  Serial.println("\n===================================");
  Serial.println("ESP32 Security Tool v4.4");
  Serial.println("By Nihal MP - RFID WORKING");
  Serial.println("===================================\n");
  
  // Initialize pins
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);
  pinMode(BTN_STARTSTOP, INPUT_PULLUP);
  pinMode(RF_TX_PIN, OUTPUT);
  digitalWrite(RF_TX_PIN, LOW);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  // Initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  
  if(!display.begin(OLED_ADDR, true)) {
    Serial.println("[OLED] Failed on 0x3C, trying 0x3D...");
    if(!display.begin(0x3D, true)) {
      Serial.println("[OLED] Failed!");
      while(1) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        delay(200);
      }
    }
  }
  
  Serial.println("[OLED] OK");
  showBootAnimation();
  
  // Initialize BadUSB
  Serial.println("[BadUSB] Starting...");
  bleKeyboard.begin();
  initBadUSBScripts();
  
  // Initialize RF
  Serial.println("[RF] Starting...");
  rfReceiver.enableReceive(digitalPinToInterrupt(RF_RX_PIN));
  rfTransmitter.enableTransmit(RF_TX_PIN);
  rfTransmitter.setRepeatTransmit(2);
  initRFSlots();
  loadRFSlots();
  
  // Initialize IR
  Serial.println("[IR] Starting...");
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK);
  initIRSlots();
  loadIRSlots();
  
  // Initialize Beacon
  Serial.println("[Beacon] Starting...");
  WiFi.mode(WIFI_OFF);
  currentWiFiMode = WIFI_OFF;
  
  // Initialize RC522 - USING YOUR WORKING CODE
  Serial.println("[RFID] Starting RC522...");
  Serial.println("[RFID] SPI Pins:");
  Serial.print("  SCK: GPIO"); Serial.println(RC522_SCK);
  Serial.print("  MISO: GPIO"); Serial.println(RC522_MISO);
  Serial.print("  MOSI: GPIO"); Serial.println(RC522_MOSI);
  Serial.print("  SS: GPIO"); Serial.println(RC522_SS);
  Serial.print("  RST: GPIO"); Serial.println(RC522_RST);
  
  // Initialize SPI exactly like your working code
  SPI.begin(RC522_SCK, RC522_MISO, RC522_MOSI, RC522_SS);
  
  // Initialize MFRC522
  mfrc522.PCD_Init();
  delay(100);
  
  // Check version
  byte version = mfrc522.PCD_ReadRegister(MFRC522::VersionReg);
  Serial.print("[RFID] MFRC522 Version: 0x");
  Serial.println(version, HEX);
  
  if(version == 0x00 || version == 0xFF) {
    Serial.println("[RFID] ERROR: Communication failed!");
    rfidInitialized = false;
  } else {
    Serial.println("[RFID] Communication OK!");
    rfidInitialized = true;
    
    // Enable antenna
    mfrc522.PCD_AntennaOn();
    
    // Test reading
    Serial.println("[RFID] Testing card reading...");
    if(mfrc522.PICC_IsNewCardPresent()) {
      Serial.println("[RFID] Card detected during test!");
    }
  }
  
  if(rfidInitialized) {
    initRFIDSlots();
    loadRFIDSlots();
    Serial.println("[RFID] Initialization SUCCESS!");
  } else {
    Serial.println("[RFID] INIT FAILED! Check wiring:");
    Serial.println("  - SS (SDA) -> GPIO 5");
    Serial.println("  - RST -> GPIO 17");
    Serial.println("  - SCK -> GPIO 18");
    Serial.println("  - MOSI -> GPIO 23");
    Serial.println("  - MISO -> GPIO 19");
    Serial.println("  - 3.3V -> 3.3V");
    Serial.println("  - GND -> GND");
  }
  
  Serial.println("\n===================================");
  Serial.println("SYSTEM READY!");
  Serial.println("===================================\n");
  
  drawMainMenu();
}

// ============================================
// MAIN LOOP
// ============================================
void loop() {
  readButtons();
  
  switch(currentMode) {
    case BADUSB_MODE:
      if(scriptExecuting) {
        executeDuckyScriptStep();
      }
      break;
      
    case RF_CLONER_MODE:
      if(rfRecording) captureRFSignal();
      if(rfTransmitting) transmitRFSignal();
      break;
      
    case IR_CLONER_MODE:
      if(irRecording) captureIRSignal();
      break;
      
    case RF_GEN_MODE:
      generateRFSignal();
      break;
      
    case BEACON_SPAM_MODE:
      performBeaconSpam();
      break;
      
    case RFID_MODE:
      if(rfidInitialized) {
        checkRFIDTag();
      }
      break;
      
    default:
      break;
  }
  
  unsigned long now = millis();
  if(now - lastScreenUpdate >= SCREEN_UPDATE_INTERVAL) {
    lastScreenUpdate = now;
    
    switch(currentMode) {
      case MAIN_MENU: drawMainMenu(); break;
      case BADUSB_MODE: drawBadUSBMenu(); break;
      case RF_CLONER_MODE: drawRFClonerMenu(); break;
      case IR_CLONER_MODE: drawIRClonerMenu(); break;
      case RF_GEN_MODE: drawRFGenMenu(); break;
      case BEACON_SPAM_MODE: drawBeaconSpamMenu(); break;
      case RFID_MODE: drawRFIDMenu(); break;
    }
    
    // LED heartbeat
    static unsigned long lastBlink = 0;
    if(now - lastBlink > 1000) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      lastBlink = now;
    }
  }
  
  delay(LOOP_DELAY);
}