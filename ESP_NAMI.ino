#include <FastLED.h> // For LED Control
#include <WiFiUdp.h>
#include <WiFi.h> // Needed for captive portal
#include <DNSServer.h> // Needed for captive portal
#include <WebServer.h> // Needed for captive portal
#include <ArduinoOTA.h> // For OTA updates
#include <Preferences.h> // saves settings and sates in non-volatile storage
#include "UI.h" // Needed for main UI
#include "Settings.h" // Needed for settings page
WiFiUDP ddpUdp;
Preferences prefs;


// ===== DEFINES =====
// Hardware and LED config
#define NUM_LEDS        174    // Total LED count
#define DATA_PIN        18     // LED data output pin 16

#define BRAKE_START_LED 70    // Start index of brake segment (inclusive)
#define BRAKE_END_LED   104    // End index of brake segment (inclusive)
#define MAIN_LEN        (NUM_LEDS - (BRAKE_END_LED - BRAKE_START_LED + 1))

#define LIGHT_TOGGLE_PIN 26    // Light toggle input pin (HIGH/LOW) 17
#define BRAKE_PIN       19     // Brake input pin (HIGH/LOW) 13
#define LEFT_PIN        23     // Left indicator input pin (HIGH/LOW) 26
#define RIGHT_PIN       33     // Right indicator input pin (HIGH/LOW) 27

#define DDP_PORT 4048

// ===== RUNTIME CONFIGURABLE VARIABLES =====
// These replace the defines and can be changed at runtime
uint8_t brakeIdleBrightness = 50;      // Brake LEDs brightness when idle (0-255)
uint8_t brakeActiveBrightness = 255;   // Brake LEDs brightness when active (0-255)
uint8_t backgroundDimLevel = 50;       // Dim level for background LEDs (0-255)
uint8_t dimTransitionSpeed = 20;       // Speed for dimming transition in ms
uint16_t indicatorFlashTime = 300;     // Indicator flash interval in ms
uint16_t trailUpdateInterval = 15;     // Interval between trail animation steps (ms)
uint16_t bootDurationMs = 2210;        // Total boot effect duration in ms
CRGB bootColor = CRGB::White;          // Colour for boot effect
CRGB indicatorColor = CRGB(255, 255, 0); // Indicator LED colour (R,G,B)

// Calculated values
uint16_t bootSteps = NUM_LEDS / 2;     // Steps in boot effect
uint16_t bootStepDelay = bootDurationMs / bootSteps; // Delay per boot step

// ===== GLOBAL OBJECTS =====
CRGB leds[NUM_LEDS];  // LED array

bool isBrakeLED(int index) {
  return index >= BRAKE_START_LED && index <= BRAKE_END_LED;
}

bool isMainSegmentLED(int index) {
  return index < BRAKE_START_LED || index > BRAKE_END_LED;
}

// ===== STATE VARIABLES =====
// Toggle states
bool lightOverride = true; // Force lights on regardless of toggle pin
bool lightToggle = false;
bool braking = false;
bool leftIndicating = false;
bool rightIndicating = false;

// Boot effect state
bool bootDone = true; // set true to skip boot effect
unsigned long bootLastStepTime = 0;
int bootStep = 0;

// Indicator and underglow state
uint8_t backgroundBrightness = 255; // brightness for background LEDs
bool indicatorFlashState = false;   // flashing state for indicators

String selectedEffect = "rainbow";  // Default LED effect
CRGB underglowColor = CRGB::Blue;   // Default underglow colour

// Indicator trail mode and animation
bool indicatorTrailMode = true;    // false = flash mode, true = trail mode
int trailLength = 40;               // Length of the indicator trail
int trailPosition = 0;              // Current position for trail animation
bool leftTrailReverse = false;      // Reverse direction for left indicator trail
bool rightTrailReverse = true;      // Reverse direction for right indicator trail
unsigned long lastTrailUpdate = 0;  // Last time trail updated

// Timing helpers for indicators and dimming
unsigned long lastIndicatorToggle = 0;
unsigned long lastDimUpdate = 0;

// ===== Hazard effect globals =====
// Hazard effect globals
unsigned long hazardLastFlash = 0;
unsigned long hazardPauseStart = 0;
int hazardFlashCount = 0;     // Start at 0, counts flashes per side
bool hazardSideLeft = true;   // Start flashing left side

const int hazardFlashInterval = 90;   // time between each flash (ms)
const int hazardPauseInterval = 500;  // pause time between left/right flashes (ms)
const int hazardFlashesPerSide = 4;   // number of flashes per side

// Misc animation variables
uint16_t rainbowOffset = 0;  // Offset for moving rainbow effect
uint8_t pulseBrightness = 0; // Current brightness for pulse effect
int pulseDirection = 5;      // Pulse brightness step
int chasePosition = 0;       // Position for chase effect


// ===== WIFI CAPTIVE PORTAL =====
const char* ssid = "Nami-LEDS"; // SSID name for network
const char* password = "Joeyrides123"; // Password for network


const byte DNS_PORT = 53;
DNSServer dnsServer;

WebServer server(80);

// Sends the UI 
void handleRoot() {
  server.send_P(200, "text/html", captivePortalHTML);

}

// Sends the settings page
void handleSettings() {
  server.send_P(200, "text/html", settingsPageHTML);
}
void setup() {
  Serial.begin(115200);
  Serial.println("==== Booting Up ====");

  // Setup pins
  pinMode(RIGHT_PIN, INPUT_PULLUP);
  pinMode(LEFT_PIN, INPUT_PULLUP);
  pinMode(BRAKE_PIN, INPUT_PULLUP);
  pinMode(LIGHT_TOGGLE_PIN, INPUT_PULLUP);

  // FastLED init
  FastLED.addLeds<WS2815, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();

  // Open prefs with namespace "leds", read/write
  prefs.begin("leds", false);
    // Load saved color
  String savedColor = prefs.getString("color", "");
  if (savedColor.length() == 7 && savedColor[0] == '#') {
    long number = strtol(savedColor.substring(1).c_str(), NULL, 16);
    underglowColor = CRGB((number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF);
    Serial.println("Loaded saved color: " + savedColor);
  }

  // Load saved effect
  selectedEffect = prefs.getString("effect", "rainbow");
  Serial.println("Loaded saved effect: " + selectedEffect);

  // Load saved settings
  trailLength = prefs.getInt("trailLength", 40);
  lightOverride = prefs.getBool("lightOverride", true);
  bootDone = prefs.getBool("bootDone", false);
  indicatorTrailMode = prefs.getBool("indicatorTrailMode", true);
  leftTrailReverse = prefs.getBool("leftTrailReverse", false);
  rightTrailReverse = prefs.getBool("rightTrailReverse", true);
  
  // Load runtime configurable settings
  brakeActiveBrightness = prefs.getInt("fullBrakeBrightness", brakeActiveBrightness);
  brakeIdleBrightness = prefs.getInt("idleBrakeBrightness", brakeIdleBrightness);
  backgroundDimLevel = prefs.getInt("indicatorBackroundDim", backgroundDimLevel);
  dimTransitionSpeed = prefs.getInt("indicatorTransition", dimTransitionSpeed);
  indicatorFlashTime = prefs.getInt("indicatorFlashTime", indicatorFlashTime);
  trailUpdateInterval = prefs.getInt("trailFlashInterval", trailUpdateInterval);
  bootDurationMs = prefs.getInt("bootDuration", bootDurationMs);
  
  // Load colors
  String savedBootColor = prefs.getString("bootColour", "");
  if (savedBootColor.length() == 7 && savedBootColor[0] == '#') {
    long number = strtol(savedBootColor.substring(1).c_str(), NULL, 16);
    bootColor = CRGB((number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF);
  }
  
  String savedIndicatorColor = prefs.getString("indicatorColour", "");
  if (savedIndicatorColor.length() == 7 && savedIndicatorColor[0] == '#') {
    long number = strtol(savedIndicatorColor.substring(1).c_str(), NULL, 16);
    indicatorColor = CRGB((number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF);
  }
  
  // Recalculate derived values
  bootSteps = NUM_LEDS / 2;
  bootStepDelay = bootDurationMs / bootSteps;
  
  Serial.println("Loaded saved settings:");
  Serial.println("  trailLength: " + String(trailLength));
  Serial.println("  lightOverride: " + String(lightOverride));
  Serial.println("  bootDone: " + String(bootDone));
  Serial.println("  indicatorTrailMode: " + String(indicatorTrailMode));
  Serial.println("  leftTrailReverse: " + String(leftTrailReverse));
  Serial.println("  rightTrailReverse: " + String(rightTrailReverse));
  Serial.println("  brakeActiveBrightness: " + String(brakeActiveBrightness));
  Serial.println("  brakeIdleBrightness: " + String(brakeIdleBrightness));
  Serial.println("  backgroundDimLevel: " + String(backgroundDimLevel));
  Serial.println("  indicatorFlashTime: " + String(indicatorFlashTime));
  Serial.println("  trailUpdateInterval: " + String(trailUpdateInterval));
  Serial.println("  bootDurationMs: " + String(bootDurationMs));
WiFi.softAP(ssid, password);
Serial.print("Access Point IP: ");
Serial.println(WiFi.softAPIP());


// === OTA setup ===
ArduinoOTA.setHostname(ssid);       // Use same as AP SSID
ArduinoOTA.setPassword(password);   // Use same as AP password


ArduinoOTA.onStart([]() {
  Serial.println("Starting OTA update...");
});
ArduinoOTA.onEnd([]() {
  Serial.println("\nOTA update finished!");
});
ArduinoOTA.onError([](ota_error_t error) {
  Serial.printf("OTA Error[%u]: ", error);
  if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
  else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
  else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
  else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
  else if (error == OTA_END_ERROR) Serial.println("End Failed");
});

ArduinoOTA.begin();
Serial.println("OTA Ready");

ddpUdp.begin(DDP_PORT);
Serial.println("DDP listener started on port 4048");


  // Start DNS server to redirect to captive portal IP
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/settings", handleSettings);
  server.onNotFound([]() {
    // Redirect any other URL to root (captive portal)
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
  });

server.on("/setColor", HTTP_GET, []() {
  if (server.hasArg("color")) {
    String color = server.arg("color");
    Serial.println("Color set to: " + color);

    long number = strtol(color.substring(1).c_str(), NULL, 16);
    underglowColor = CRGB((number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF);

    // Save to prefs
    prefs.putString("color", color);
  }
  server.send(200, "text/plain", "OK");
});

server.on("/setEffect", HTTP_GET, []() {
  if (server.hasArg("effect")) {
    String effect = server.arg("effect");
    Serial.println("Effect set to: " + effect);
    selectedEffect = effect;

    // Save to prefs
    prefs.putString("effect", effect);
  }
  server.send(200, "text/plain", "OK");
});


// === GET CURRENT STATE ===
server.on("/getState", HTTP_GET, []() {
  String colorHex = "#";
  colorHex += String(underglowColor.r, HEX);
  colorHex += String(underglowColor.g, HEX);
  colorHex += String(underglowColor.b, HEX);

  // Pad hex digits if needed (eg: "f" => "0f")
  if (colorHex.length() != 7) {
    colorHex = "#";
    if (underglowColor.r < 16) colorHex += "0";
    colorHex += String(underglowColor.r, HEX);
    if (underglowColor.g < 16) colorHex += "0";
    colorHex += String(underglowColor.g, HEX);
    if (underglowColor.b < 16) colorHex += "0";
    colorHex += String(underglowColor.b, HEX);
  }

  String json = "{\"color\": \"" + colorHex + "\", \"effect\": \"" + selectedEffect + "\"}";
  server.send(200, "application/json", json);
});

// === GET ALL SETTINGS ===
server.on("/getSettings", HTTP_GET, []() {
  String json = "{";
  
  // Get saved values from preferences, fallback to runtime variables
  int fullBrakeBrightness = prefs.getInt("fullBrakeBrightness", brakeActiveBrightness);
  int idleBrakeBrightness = prefs.getInt("idleBrakeBrightness", brakeIdleBrightness);
  int indicatorBackroundDim = prefs.getInt("indicatorBackroundDim", backgroundDimLevel);
  String bootColour = prefs.getString("bootColour", "#ffffff");
  String indicatorColour = prefs.getString("indicatorColour", "#ffff00");
  int indicatorTransition = prefs.getInt("indicatorTransition", dimTransitionSpeed);
  int indicatorFlashTimeValue = prefs.getInt("indicatorFlashTime", indicatorFlashTime);
  int trailFlashInterval = prefs.getInt("trailFlashInterval", trailUpdateInterval);
  int bootDuration = prefs.getInt("bootDuration", bootDurationMs);
  String captiveSsid = prefs.getString("captiveSsid", ssid);
  String captivePassword = prefs.getString("captivePassword", password);
  
  json += "\"fullBrakeBrightness\":" + String(fullBrakeBrightness) + ",";
  json += "\"idleBrakeBrightness\":" + String(idleBrakeBrightness) + ",";
  json += "\"indicatorBackroundDim\":" + String(indicatorBackroundDim) + ",";
  json += "\"bootColour\":\"" + bootColour + "\",";
  json += "\"indicatorColour\":\"" + indicatorColour + "\",";
  json += "\"indicatorTransition\":" + String(indicatorTransition) + ",";
  json += "\"colourTransition\":200,"; // TODO: Make configurable
  json += "\"indicatorFlashTime\":" + String(indicatorFlashTimeValue) + ",";
  json += "\"trailFlashInterval\":" + String(trailFlashInterval) + ",";
  json += "\"bootDuration\":" + String(bootDuration) + ",";
  json += "\"totalLedCount\":" + String(NUM_LEDS) + ",";
  json += "\"brakeLedStart\":" + String(BRAKE_START_LED) + ",";
  json += "\"brakeLedEnd\":" + String(BRAKE_END_LED) + ",";
  json += "\"indicatorTrailLength\":" + String(trailLength) + ",";
  json += "\"defaultLightMode\":" + String(lightOverride ? "true" : "false") + ",";
  json += "\"bootEffectToggle\":" + String(bootDone ? "false" : "true") + ",";
  json += "\"indicatorEffectToggle\":" + String(indicatorTrailMode ? "true" : "false") + ",";
  json += "\"leftTrailReverse\":" + String(leftTrailReverse ? "true" : "false") + ",";
  json += "\"rightTrailReverse\":" + String(rightTrailReverse ? "true" : "false") + ",";
  json += "\"xlightsToggle\":" + String(selectedEffect.equalsIgnoreCase("X-lights") ? "true" : "false") + ",";
  json += "\"ledDataGpio\":" + String(DATA_PIN) + ",";
  json += "\"brakeSignalGpio\":" + String(BRAKE_PIN) + ",";
  json += "\"lightSignalGpio\":" + String(LIGHT_TOGGLE_PIN) + ",";
  json += "\"leftSignalGpio\":" + String(LEFT_PIN) + ",";
  json += "\"rightSignalGpio\":" + String(RIGHT_PIN) + ",";
  json += "\"captiveSsid\":\"" + captiveSsid + "\",";
  json += "\"captivePassword\":\"" + captivePassword + "\"";
  json += "}";
  server.send(200, "application/json", json);
});

// === SET ALL SETTINGS ===
server.on("/setSettings", HTTP_POST, []() {
  if (server.hasArg("plain")) {
    String json = server.arg("plain");
    Serial.println("Received settings: " + json);
    
    // Parse JSON and apply settings
    // Note: For now, we'll only apply the settings that can be changed at runtime
    // Hardware settings like GPIO pins and LED counts would require a restart
    
    // Helper function to extract JSON values
    auto extractInt = [](const String& json, const String& key) -> int {
      int start = json.indexOf("\"" + key + "\":") + key.length() + 3;
      if (start == -1) return -1;
      int end = json.indexOf(",", start);
      if (end == -1) end = json.indexOf("}", start);
      return json.substring(start, end).toInt();
    };
    
    auto extractBool = [](const String& json, const String& key) -> bool {
      int start = json.indexOf("\"" + key + "\":") + key.length() + 3;
      if (start == -1) return false;
      int end = json.indexOf(",", start);
      if (end == -1) end = json.indexOf("}", start);
      return json.substring(start, end).indexOf("true") != -1;
    };
    
    auto extractString = [](const String& json, const String& key) -> String {
      int start = json.indexOf("\"" + key + "\":\"") + key.length() + 4;
      if (start == -1) return "";
      int end = json.indexOf("\"", start);
      return json.substring(start, end);
    };
    
    // Apply runtime-configurable settings
    int trailLengthValue = extractInt(json, "indicatorTrailLength");
    if (trailLengthValue != -1) {
      trailLength = trailLengthValue;
      prefs.putInt("trailLength", trailLength);
      Serial.println("Updated trailLength: " + String(trailLength));
    }
    
    bool defaultLightMode = extractBool(json, "defaultLightMode");
    lightOverride = defaultLightMode;
    prefs.putBool("lightOverride", lightOverride);
    Serial.println("Updated lightOverride: " + String(lightOverride));
    
    bool bootEffectToggle = extractBool(json, "bootEffectToggle");
    bootDone = !bootEffectToggle; // Inverted logic
    prefs.putBool("bootDone", bootDone);
    Serial.println("Updated bootDone: " + String(bootDone));
    
    bool indicatorEffectToggle = extractBool(json, "indicatorEffectToggle");
    indicatorTrailMode = indicatorEffectToggle;
    prefs.putBool("indicatorTrailMode", indicatorTrailMode);
    Serial.println("Updated indicatorTrailMode: " + String(indicatorTrailMode));
    
    bool leftTrailReverse = extractBool(json, "leftTrailReverse");
    prefs.putBool("leftTrailReverse", leftTrailReverse);
    Serial.println("Updated leftTrailReverse: " + String(leftTrailReverse));
    
    bool rightTrailReverse = extractBool(json, "rightTrailReverse");
    prefs.putBool("rightTrailReverse", rightTrailReverse);
    Serial.println("Updated rightTrailReverse: " + String(rightTrailReverse));
    
    bool xlightsToggle = extractBool(json, "xlightsToggle");
    if (xlightsToggle) {
      selectedEffect = "X-lights";
      prefs.putString("effect", selectedEffect);
      Serial.println("Updated selectedEffect: " + selectedEffect);
    }
    
    // Apply runtime-configurable settings immediately
    int fullBrakeBrightness = extractInt(json, "fullBrakeBrightness");
    if (fullBrakeBrightness != -1) {
      brakeActiveBrightness = fullBrakeBrightness;
      prefs.putInt("fullBrakeBrightness", fullBrakeBrightness);
      Serial.println("Updated brakeActiveBrightness: " + String(brakeActiveBrightness));
    }
    
    int idleBrakeBrightness = extractInt(json, "idleBrakeBrightness");
    if (idleBrakeBrightness != -1) {
      brakeIdleBrightness = idleBrakeBrightness;
      prefs.putInt("idleBrakeBrightness", idleBrakeBrightness);
      Serial.println("Updated brakeIdleBrightness: " + String(brakeIdleBrightness));
    }
    
    int indicatorBackroundDim = extractInt(json, "indicatorBackroundDim");
    if (indicatorBackroundDim != -1) {
      backgroundDimLevel = indicatorBackroundDim;
      prefs.putInt("indicatorBackroundDim", indicatorBackroundDim);
      Serial.println("Updated backgroundDimLevel: " + String(backgroundDimLevel));
    }
    
    String bootColour = extractString(json, "bootColour");
    if (bootColour.length() > 0) {
      // Convert hex to CRGB
      long number = strtol(bootColour.substring(1).c_str(), NULL, 16);
      bootColor = CRGB((number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF);
      prefs.putString("bootColour", bootColour);
      Serial.println("Updated bootColor: " + bootColour);
    }
    
    String indicatorColour = extractString(json, "indicatorColour");
    if (indicatorColour.length() > 0) {
      // Convert hex to CRGB
      long number = strtol(indicatorColour.substring(1).c_str(), NULL, 16);
      indicatorColor = CRGB((number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF);
      prefs.putString("indicatorColour", indicatorColour);
      Serial.println("Updated indicatorColor: " + indicatorColour);
    }
    
    int indicatorTransition = extractInt(json, "indicatorTransition");
    if (indicatorTransition != -1) {
      dimTransitionSpeed = indicatorTransition;
      prefs.putInt("indicatorTransition", indicatorTransition);
      Serial.println("Updated dimTransitionSpeed: " + String(dimTransitionSpeed));
    }
    
    int indicatorFlashTimeValue = extractInt(json, "indicatorFlashTime");
    if (indicatorFlashTimeValue != -1) {
      indicatorFlashTime = indicatorFlashTimeValue;
      prefs.putInt("indicatorFlashTime", indicatorFlashTimeValue);
      Serial.println("Updated indicatorFlashTime: " + String(indicatorFlashTime));
    }
    
    int trailFlashInterval = extractInt(json, "trailFlashInterval");
    if (trailFlashInterval != -1) {
      trailUpdateInterval = trailFlashInterval;
      prefs.putInt("trailFlashInterval", trailFlashInterval);
      Serial.println("Updated trailUpdateInterval: " + String(trailUpdateInterval));
    }
    
    int bootDuration = extractInt(json, "bootDuration");
    if (bootDuration != -1) {
      bootDurationMs = bootDuration;
      bootSteps = NUM_LEDS / 2;
      bootStepDelay = bootDurationMs / bootSteps;
      prefs.putInt("bootDuration", bootDuration);
      Serial.println("Updated bootDurationMs: " + String(bootDurationMs));
    }
    
    String captiveSsid = extractString(json, "captiveSsid");
    if (captiveSsid.length() > 0) {
      prefs.putString("captiveSsid", captiveSsid);
      Serial.println("Saved captiveSsid: " + captiveSsid);
    }
    
    String captivePassword = extractString(json, "captivePassword");
    if (captivePassword.length() > 0) {
      prefs.putString("captivePassword", captivePassword);
      Serial.println("Saved captivePassword: " + captivePassword);
    }
    
    server.send(200, "text/plain", "Settings applied");
  } else {
    server.send(400, "text/plain", "No data received");
  }
});

// === GET GPIO STATES ===
server.on("/getGpioStates", HTTP_GET, []() {
  String json = "{";
  json += "\"lightState\":" + String(!digitalRead(LIGHT_TOGGLE_PIN)) + ",";
  json += "\"brakeState\":" + String(!digitalRead(BRAKE_PIN)) + ",";
  json += "\"leftState\":" + String(!digitalRead(LEFT_PIN)) + ",";
  json += "\"rightState\":" + String(!digitalRead(RIGHT_PIN));
  json += "}";
  server.send(200, "application/json", json);
});

  server.begin();
}

void handleDDP() {
  int packetSize = ddpUdp.parsePacket();
  if (packetSize < 10) return;

  uint8_t header[10];
  ddpUdp.read(header, 10);

  // Confirm it's a DDP packet
  if ((header[0] & 0xF0) != 0x40) return;

  int dataLen = packetSize - 10;
  if (dataLen > NUM_LEDS * 3) dataLen = NUM_LEDS * 3;

  for (int i = 0; i < dataLen / 3; i++) {
    int ledIndex = i;
    if (ledIndex >= NUM_LEDS) break;

    uint8_t r, g, b;
    ddpUdp.read(&r, 1);
    ddpUdp.read(&g, 1);
    ddpUdp.read(&b, 1);

    leds[ledIndex] = CRGB(r, g, b);
  }
}

void runBootEffect() {
  unsigned long now = millis();
  if (bootStep >= bootSteps) {
    bootDone = true;
    // Leave LEDs fully lit white after boot
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = bootColor;
    }
    FastLED.show();
    return;
  }

  if (now - bootLastStepTime >= bootStepDelay) {
    leds[bootStep] = bootColor;
    leds[NUM_LEDS - 1 - bootStep] = bootColor;
    FastLED.show();

    bootStep++;
    bootLastStepTime = now;
  }
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  ArduinoOTA.handle();

  if (!bootDone) {
    runBootEffect();
    return;  // Skip rest of loop until boot effect done
  }

  if (selectedEffect.equalsIgnoreCase("X-lights")) {
    handleDDP();
    FastLED.show();
    return;  // skip everything else when Xlights active
  }

  lightToggle = lightOverride || (!digitalRead(LIGHT_TOGGLE_PIN));//lightToggle     = !digitalRead(LIGHT_TOGGLE_PIN);
  braking         = !digitalRead(BRAKE_PIN);
  leftIndicating  = !digitalRead(LEFT_PIN);
  rightIndicating = !digitalRead(RIGHT_PIN);

  if (!lightToggle) {
    runSignalOnlyMode();
  } else {
    runFullEffectsMode();
  }

  FastLED.show();
  delay(5);
}


// ===== LIGHT OFF MODE =====
void runSignalOnlyMode() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  backgroundBrightness = 0;

  handleIndicatorsOverlay();
  updateBrakeLights();
}

// ===== LIGHT ON MODE =====
void runFullEffectsMode() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  runEffect();
  handleIndicatorsOverlay();
  updateBrakeLights();
}

// ===== INDICATOR OVERLAY =====
void handleIndicatorsOverlay() {
  unsigned long now = millis();

  bool indicating = leftIndicating || rightIndicating;

  // Smooth dimming for background brightness (same for both modes)
  if (now - lastDimUpdate >= dimTransitionSpeed) {
    if (indicating && backgroundBrightness > backgroundDimLevel) {
      backgroundBrightness -= 5;
    } else if (!indicating && backgroundBrightness < 255) {
      backgroundBrightness += 5;
    }
    backgroundBrightness = constrain(backgroundBrightness, backgroundDimLevel, 255);
    lastDimUpdate = now;
  }

  int leftSegmentStart = 0;
  int leftSegmentEnd = BRAKE_START_LED - 1;
  int rightSegmentStart = BRAKE_END_LED + 1;
  int rightSegmentEnd = NUM_LEDS - 1;

  int leftSegmentLength = leftSegmentEnd - leftSegmentStart + 1;
  int rightSegmentLength = rightSegmentEnd - rightSegmentStart + 1;

  if (!indicatorTrailMode) {
    // Classic flash mode
    if (now - lastIndicatorToggle >= indicatorFlashTime) {
      indicatorFlashState = !indicatorFlashState;
      lastIndicatorToggle = now;
    }

    if (indicatorFlashState) {
      if (leftIndicating) {
        for (int i = leftSegmentStart; i <= leftSegmentEnd; i++) {
          leds[i] = indicatorColor;
        }
      }

      if (rightIndicating) {
        for (int i = rightSegmentStart; i <= rightSegmentEnd; i++) {
          leds[i] = indicatorColor;
        }
      }
    }
  } else {
    // Trail mode

    if (now - lastTrailUpdate >= trailUpdateInterval) {
      trailPosition = (trailPosition + 1) % max(leftSegmentLength, rightSegmentLength);
      lastTrailUpdate = now;
    }

    if (leftIndicating) {
      for (int i = 0; i < trailLength; i++) {
        int pos = (trailPosition + i) % leftSegmentLength;
        if (leftTrailReverse) {
          pos = leftSegmentLength - 1 - pos;
        }
        leds[leftSegmentStart + pos] = indicatorColor;
      }
    }

    if (rightIndicating) {
      for (int i = 0; i < trailLength; i++) {
        int pos = (trailPosition + i) % rightSegmentLength;
        if (rightTrailReverse) {
          pos = rightSegmentLength - 1 - pos;
        }
        leds[rightSegmentStart + pos] = indicatorColor;
      }
    }
  }
}

// ===== BRAKE LIGHT SEGMENT =====
void updateBrakeLights() {
  uint8_t brightness;

  if (braking) {
    brightness = brakeActiveBrightness;
  } else {
    brightness = lightToggle ? brakeIdleBrightness : 0;
  }

  for (int i = BRAKE_START_LED; i <= BRAKE_END_LED; i++) {
    leds[i] = CRGB(brightness, 0, 0);
  }
}

void runEffect() {
  rainbowOffset++;

  if (selectedEffect.equalsIgnoreCase("rainbow")) {
    for (int i = 0; i < NUM_LEDS; i++) {
      if (isBrakeLED(i)) continue;
      uint8_t hue = (i * 255 / MAIN_LEN + rainbowOffset) % 255;
      leds[i] = CHSV(hue, 255, backgroundBrightness);
    }
  } else if (selectedEffect.equalsIgnoreCase("solid")) {
    CRGB scaledColor = underglowColor;
    scaledColor.nscale8_video(backgroundBrightness);
    for (int i = 0; i < NUM_LEDS; i++) {
      if (isBrakeLED(i)) continue;
      leds[i] = scaledColor;
    }
  } else if (selectedEffect.equalsIgnoreCase("sparkle")) {
    CRGB baseColor = underglowColor;
    baseColor.nscale8_video(backgroundBrightness / 2);
    for (int i = 0; i < NUM_LEDS; i++) {
      if (isBrakeLED(i)) continue;
      leds[i] = baseColor;
    }

    int sparkleIndex = random(MAIN_LEN);
    leds[sparkleIndex] = CRGB::White;  // Sparkle white pixel
  } else if (selectedEffect.equalsIgnoreCase("pulse")) {
    // Pulse effect: brightness goes up and down smoothly
    pulseBrightness += pulseDirection;
    if (pulseBrightness <= 50 || pulseBrightness >= backgroundBrightness) {
      pulseDirection = -pulseDirection;  // reverse direction
      pulseBrightness = constrain(pulseBrightness, 50, backgroundBrightness);
    }
    CRGB scaledColor = underglowColor;
    scaledColor.nscale8_video(pulseBrightness);
    for (int i = 0; i < NUM_LEDS; i++) {
      if (isBrakeLED(i)) continue;
      leds[i] = scaledColor;
    }
  } else if (selectedEffect.equalsIgnoreCase("chase")) {
    int chaseLength = 40;       // Number of LEDs lit at once — just change this number for bigger or smaller
    int fadeAmount = 150;      // Trail fade speed (0-255)

    // Fade all LEDs to create trailing effect
    for (int i = 0; i < NUM_LEDS; i++) {
      if (isBrakeLED(i)) continue;
      leds[i].nscale8(fadeAmount);
    }

    // Light up chaseLength LEDs starting at chasePosition
    for (int i = 0; i < chaseLength; i++) {
      int ledIndex = (chasePosition + i) % MAIN_LEN;
      leds[ledIndex] = underglowColor;
    }

    // Move the chase forward
    chasePosition = (chasePosition + 1) % MAIN_LEN;
  } else if (selectedEffect.equalsIgnoreCase("Hazard")) {
    unsigned long now = millis();

    int leftSegmentStart = 0;
    int leftSegmentEnd = BRAKE_START_LED - 1;
    int rightSegmentStart = BRAKE_END_LED + 1;
    int rightSegmentEnd = NUM_LEDS - 1;

    // Clear main LEDs first
    for (int i = 0; i < NUM_LEDS; i++) {
      if (isBrakeLED(i)) continue;
      leds[i] = CRGB::Black;
    }

    if (hazardFlashCount >= hazardFlashesPerSide * 2) {
      hazardFlashCount = 0;          // Reset total flash count every full cycle
      hazardPauseStart = now;        // Start pause timer
      hazardSideLeft = true;         // Restart on left side after pause
    }

    if (hazardPauseStart != 0) {
      if (now - hazardPauseStart >= hazardPauseInterval) {
        hazardPauseStart = 0;        // End pause
        hazardFlashCount = 0;        // Reset flash count to start flashing again
      }
      // During pause, LEDs off
      return;
    }

    if (now - hazardLastFlash >= hazardFlashInterval) {
      hazardLastFlash = now;
      hazardFlashCount++;
    }

    bool flashOn = (hazardFlashCount % 2) == 1;  // On for odd counts, off for even counts

    if (flashOn) {
      if (hazardFlashCount <= hazardFlashesPerSide * 2) {
        if (hazardFlashCount <= hazardFlashesPerSide) {
          // Left side flash
          for (int i = leftSegmentStart; i <= leftSegmentEnd; i++) {
            leds[i] = underglowColor;
          }
        } else {
          // Right side flash
          for (int i = rightSegmentStart; i <= rightSegmentEnd; i++) {
            leds[i] = underglowColor;
          }
        }
      }
    }
  } else {
    fill_solid(leds, MAIN_LEN, CRGB::Black);
  }
}
