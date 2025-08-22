// Version 1.0 Beta 3
// Revision: 1.2
// Status: Beta - Testing phase, use with caution
// Author: JoeyGE0
// Description: ESP32 LED Strip Controller for vehicle lighting
// Features: WiFi control, DDP protocol, OTA updates, brake/indicator lights
// Hardware: ESP32 + WS2815 LED strip

// ===== INCLUDES =====
#include <Arduino.h>
#include <FastLED.h>           // For LED Control
#include <WiFiUdp.h>           // For DDP protocol
#include <WiFi.h>              // For WiFi functionality
#include <DNSServer.h>         // For captive portal
#include <WebServer.h>         // For web server
#include "ArduinoOTA.h"        // For OTA updates
#include <Update.h>            // For OTA firmware updates
#include <Preferences.h>       // For non-volatile storage
#include "UI.h"                // Main UI HTML
#include "Settings.h"          // Settings page HTML
#include "styles.h"            // Main page CSS
#include "script.h"            // Main page JavaScript
#include "iro.h"               // iro.min.js library (Colour picker)
#include "settings-styles.h"   // Settings page CSS
#include "settings-script.h"   // Settings page JavaScript
#include "manifest.h"          // Web app manifest

// ===== FUNCTION DECLARATIONS =====
// Web server handlers
void handleSetColor();
void handleSetEffect();
void handleGetState();
void handleGetSettings();
void handleSetSettings();
void handleGetGpioStates();
void handleUpdate();
void handleFactoryReset();

// Static file handlers
void handleStyles();
void handleScript();
void handleIro();
void handleSettingsStyles();
void handleSettingsScript();
void handleManifest();

// Settings management
void loadEssentialSettings();
void loadAllSettings();
void printLoadedSettings();
void parseAndApplySettings(String json);
void applyRuntimeSettings(String json, int (*extractInt)(const String&, const String&), bool (*extractBool)(const String&, const String&), String (*extractString)(const String&, const String&));
void applyToggleSettings(String json, bool (*extractBool)(const String&, const String&));
void applyColorSettings(String json, String (*extractString)(const String&, const String&));
void applyTimingSettings(String json, int (*extractInt)(const String&, const String&));
void applyNetworkSettings(String json, String (*extractString)(const String&, const String&));

// Network initialization
void initializeNetwork();
void setupWebServerRoutes();
void initializeOTA();

// Main loop functions
void runBootEffect();
void handleBackgroundLoading();
void handleDDP();
void updateInputStates();
void updateColorTransition();
void runSignalOnlyMode();
void runFullEffectsMode();

// Indicator system
void handleIndicatorsOverlay();
void runFlashMode(int leftStart, int leftEnd, int rightStart, int rightEnd, unsigned long now);
void runTrailMode(int leftStart, int leftEnd, int rightStart, int rightEnd, int leftLength, int rightLength, unsigned long now);

// Brake system
void updateBrakeLights();

// Effects
void runEffect();
void runRainbowEffect();
void runSolidEffect();
void runSparkleEffect();
void runPulseEffect();
void runChaseEffect();
void runHazardEffect();

// Status LED control
void updateStatusLed();

// ===== HARDWARE CONFIGURATION =====
// LED Strip Configuration
#define DATA_PIN        18     // LED data output pin

// ===== EFFECT ENUM SYSTEM =====
enum EffectType { RAINBOW, SOLID, SPARKLE, PULSE, CHASE, HAZARD, XLIGHTS };
EffectType currentEffect = RAINBOW;

// Conversion functions for web compatibility
String effectToString(EffectType effect) {
  switch(effect) {
    case RAINBOW: return "rainbow";
    case SOLID: return "solid";
    case SPARKLE: return "sparkle";
    case PULSE: return "pulse";
    case CHASE: return "chase";
    case HAZARD: return "Hazard";
    case XLIGHTS: return "X-lights";
    default: return "rainbow";
  }
}

EffectType stringToEffect(String str) {
  if (str.equalsIgnoreCase("rainbow")) return RAINBOW;
  if (str.equalsIgnoreCase("solid")) return SOLID;
  if (str.equalsIgnoreCase("sparkle")) return SPARKLE;
  if (str.equalsIgnoreCase("pulse")) return PULSE;
  if (str.equalsIgnoreCase("chase")) return CHASE;
  if (str.equalsIgnoreCase("Hazard")) return HAZARD;
  if (str.equalsIgnoreCase("X-lights")) return XLIGHTS;
  return RAINBOW; // default
}

// Runtime LED Configuration (will be loaded from preferences)
uint16_t NUM_LEDS = 174;           // Total LED count
uint16_t BRAKE_START_LED = 70;     // Start index of brake segment (inclusive)
uint16_t BRAKE_END_LED = 104;      // End index of brake segment (inclusive)
uint16_t MAIN_LEN;                 // Calculated main segment length

// GPIO Pin Configuration
#define LIGHT_TOGGLE_PIN 26    // Light toggle input pin (HIGH/LOW)
#define BRAKE_PIN       19     // Brake input pin (HIGH/LOW)
#define LEFT_PIN        23     // Left indicator input pin (HIGH/LOW)
#define RIGHT_PIN       33     // Right indicator input pin (HIGH/LOW)
#define STATUS_LED_PIN  17     // Status LED pin (built-in ESP32 LED)

// Network Configuration
#define DDP_PORT        4048   // DDP protocol port
#define DNS_PORT        53     // DNS server port

// ===== RUNTIME CONFIGURABLE SETTINGS =====
// Brightness Settings
uint8_t brakeIdleBrightness = 50;      // Brake LEDs brightness when idle (0-255)
uint8_t brakeActiveBrightness = 255;   // Brake LEDs brightness when active (0-255)
uint8_t backgroundDimLevel = 50;       // Dim level for background LEDs (0-255)

// Timing Settings
uint8_t dimTransitionSpeed = 20;       // Speed for dimming transition in ms
uint16_t indicatorFlashTime = 558;     // Indicator flash interval in ms
uint16_t trailUpdateInterval = 15;     // Interval between trail animation steps (ms)
uint16_t bootDurationMs = 2210;        // Total boot effect duration in ms

// Color Settings
CRGB bootColor = CRGB::White;          // Color for boot effect
CRGB indicatorColor = CRGB(255, 255, 0); // Indicator LED color (R,G,B)

// Color Transition Settings
CRGB targetUnderglowColor = CRGB::Blue;  // Target color for transitions
CRGB currentUnderglowColor = CRGB::Blue; // Current color during transitions
bool colorTransitionActive = false;      // Whether a color transition is in progress
unsigned long colorTransitionStart = 0;  // Start time of current transition
uint16_t colorTransitionDuration = 500;  // Duration of color transitions in ms

// Calculated Values
uint16_t bootSteps = NUM_LEDS / 2;     // Steps in boot effect
uint16_t bootStepDelay = bootDurationMs / bootSteps; // Delay per boot step

// ===== GLOBAL OBJECTS =====
CRGB leds[300];                        // LED array (max 300 LEDs)
WiFiUDP ddpUdp;                        // DDP UDP object
Preferences prefs;                     // Preferences object
DNSServer dnsServer;                   // DNS server
WebServer server(80);                  // Web server

// ===== SYSTEM STATE VARIABLES =====
// Input States
bool lightOverride = true;             // Force lights on regardless of toggle pin
bool lightToggle = false;              // Current light toggle state
bool braking = false;                  // Current brake state
bool leftIndicating = false;           // Current left indicator state
bool rightIndicating = false;          // Current right indicator state

// Boot Effect State
bool bootDone = false;                 // Set true to skip boot effect
unsigned long bootLastStepTime = 0;    // Last boot step time
int bootStep = 0;                      // Current boot step

// Background Loading State
bool backgroundLoadingComplete = false; // Whether background loading is done
bool backgroundLoadingStarted = false;  // Whether background loading has started
unsigned long backgroundLoadStartTime = 0; // When background loading started

// Status LED State
bool statusLedPulseActive = false;     // Whether status LED is in pulse mode
unsigned long statusLedLastUpdate = 0; // Last status LED update time
uint8_t statusLedBrightness = 100;     // Current status LED brightness (0-100)
int8_t statusLedDirection = 1;         // Direction of brightness change (1 = increasing, -1 = decreasing)

// Boot flash state
bool statusLedBootFlashActive = false; // Whether boot flash pattern is active
unsigned long statusLedBootFlashTime = 0; // Boot flash timing
uint8_t statusLedBootFlashCount = 0;   // Number of flashes in current group
bool statusLedBootFlashState = false;  // Current flash state (on/off)
uint8_t statusLedBootFlashGroup = 0;   // Current flash group (0 or 1)

// Background and Effect State
uint8_t backgroundBrightness = 255;    // Brightness for background LEDs
bool indicatorFlashState = false;      // Flashing state for indicators
bool isFirstFlash = false;             // Track if this is the first flash when starting to indicate
uint16_t firstFlashDuration = 1081; // Duration of the first flash in ms
String selectedEffect = "rainbow";     // Current LED effect
CRGB underglowColor = CRGB::Blue;      // Current underglow color

// ===== FPS CONTROL SYSTEM =====
#define FRAMES_PER_SECOND 120
const uint16_t frameInterval = 1000 / FRAMES_PER_SECOND;  // 8.33ms for 120 FPS
unsigned long lastFrameTime = 0;       // Last frame update time

// Individual effect timing for optimization
unsigned long lastIndicatorUpdate = 0;     // Indicators: 120 FPS (8ms)
unsigned long lastBackgroundUpdate = 0;    // Background: 30 FPS (33ms) 
unsigned long lastColorUpdate = 0;         // Color transitions: 60 FPS (16ms)

// Performance monitoring
unsigned long lastPerformanceLog = 0;      // Performance logging interval
unsigned long loopStartTime = 0;           // Loop timing measurement

// ===== INDICATOR SYSTEM VARIABLES =====
// Trail Mode Settings
bool indicatorTrailMode = false;       // false = flash mode, true = trail mode
int trailLength = 40;                  // Length of the indicator trail
unsigned long trailPosition = 0;                 // Current position for trail animation
bool leftTrailReverse = false;         // Reverse direction for left indicator trail
bool rightTrailReverse = true;         // Reverse direction for right indicator trail
bool reverseIndicatorSides = false;  // Reverse indicator sides


// Timing Variables
unsigned long lastTrailUpdate = 0;     // Last time trail updated
unsigned long lastIndicatorToggle = 0; // Last indicator toggle time
unsigned long lastDimUpdate = 0;       // Last dim update time
bool wasIndicating = false;             // Previous indicating state for immediate start detection

// ===== HAZARD EFFECT VARIABLES =====
unsigned long hazardLastFlash = 0;     // Last hazard flash time
unsigned long hazardPauseStart = 0;    // Hazard pause start time
int hazardFlashCount = 0;              // Flash count per side
bool hazardSideLeft = true;            // Current hazard side

// Hazard Timing Constants
const int hazardFlashInterval = 90;    // Time between each flash (ms)
const int hazardPauseInterval = 500;   // Pause time between left/right flashes (ms)
const int hazardFlashesPerSide = 4;    // Number of flashes per side

// ===== ANIMATION VARIABLES =====
uint16_t rainbowOffset = 0;            // Offset for moving rainbow effect
uint8_t pulseBrightness = 0;           // Current brightness for pulse effect
int pulseDirection = 5;                // Pulse brightness step
int chasePosition = 0;                 // Position for chase effect

// ===== NETWORK CONFIGURATION =====
const char* ssid = "Nami-LEDS";        // SSID name for network
const char* password = "Joeyrides123"; // Password for network

// ===== HELPER FUNCTIONS =====
bool isBrakeLED(int index) {
  return index >= BRAKE_START_LED && index <= BRAKE_END_LED && index < NUM_LEDS;
}

bool isMainSegmentLED(int index) {
  return (index < BRAKE_START_LED || index > BRAKE_END_LED) && index < NUM_LEDS;
}

// ===== WEB SERVER HANDLERS =====
void handleRoot() {
  server.send_P(200, "text/html", captivePortalHTML);
}

void handleSettings() {
  server.send_P(200, "text/html", settingsPageHTML);
}

// ===== SETUP FUNCTION =====
void setup() {
  Serial.begin(115200);
  Serial.println("==== Booting Up ====");

  // Initialize GPIO pins
  pinMode(RIGHT_PIN, INPUT_PULLUP);
  pinMode(LEFT_PIN, INPUT_PULLUP);
  pinMode(BRAKE_PIN, INPUT_PULLUP);
  pinMode(LIGHT_TOGGLE_PIN, INPUT_PULLUP);
  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, LOW); // Start with LED off

  // Initialize preferences
  prefs.begin("leds", false);
  
  // Load essential settings only (fast boot)
  loadEssentialSettings();
  
  // Initialize FastLED with loaded LED count
  FastLED.addLeds<WS2815, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  
  // Start boot effect immediately if enabled
  if (!bootDone) {
    Serial.println("Starting boot effect...");
    bootLastStepTime = millis();
  } else {
    Serial.println("Boot effect disabled, loading background...");
    backgroundLoadingStarted = true;
    backgroundLoadStartTime = millis();
  }
  
  Serial.println("Setup complete!");
}

// ===== SETTINGS MANAGEMENT =====
void loadEssentialSettings() {
  // Load only essential settings for fast boot
  NUM_LEDS = prefs.getInt("totalLedCount", 174);
  BRAKE_START_LED = prefs.getInt("brakeLedStart", 70);
  BRAKE_END_LED = prefs.getInt("brakeLedEnd", 104);
  MAIN_LEN = NUM_LEDS - (BRAKE_END_LED - BRAKE_START_LED + 1);
  
  // Load boot effect settings
  bootDone = prefs.getBool("bootDone", false);
  bootDurationMs = prefs.getInt("bootDuration", bootDurationMs);
  
  // Load boot color
  String savedBootColor = prefs.getString("bootColour", "");
  if (savedBootColor.length() == 7 && savedBootColor[0] == '#') {
    long number = strtol(savedBootColor.substring(1).c_str(), NULL, 16);
    bootColor = CRGB((number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF);
  }
  
  // Calculate boot effect parameters
  bootSteps = NUM_LEDS / 2;
  bootStepDelay = bootDurationMs / bootSteps;
  
  Serial.println("Loaded essential settings:");
  Serial.println("  NUM_LEDS: " + String(NUM_LEDS));
  Serial.println("  bootDone: " + String(bootDone));
  Serial.println("  bootDurationMs: " + String(bootDurationMs));
}

void loadAllSettings() {
  // Load saved color
  String savedColor = prefs.getString("color", "");
  if (savedColor.length() == 7 && savedColor[0] == '#') {
    long number = strtol(savedColor.substring(1).c_str(), NULL, 16);
    underglowColor = CRGB((number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF);
    
    // Initialize transition variables with the loaded color
    targetUnderglowColor = underglowColor;
    currentUnderglowColor = underglowColor;
    colorTransitionActive = false; // No transition needed on boot
    
    Serial.println("Loaded saved color: " + savedColor);
  } else {
    // No saved color, use default
    Serial.println("No saved color found, using default");
    targetUnderglowColor = underglowColor;
    currentUnderglowColor = underglowColor;
  }

  // Load saved effect
  String savedEffect = prefs.getString("effect", "rainbow");
  currentEffect = stringToEffect(savedEffect);
  selectedEffect = savedEffect; // Keep string for web compatibility
  Serial.println("Loaded saved effect: " + selectedEffect);

  // Load toggle settings
  trailLength = prefs.getInt("trailLength", 40);
  lightOverride = prefs.getBool("lightOverride", true);
  indicatorTrailMode = prefs.getBool("indicatorTrailMode", false);
  leftTrailReverse = prefs.getBool("leftTrailReverse", false);
  rightTrailReverse = prefs.getBool("rightTrailReverse", true);
  reverseIndicatorSides = prefs.getBool("reverseSides", false);
  
  // Load runtime configurable settings
  brakeActiveBrightness = prefs.getInt("fullBrakeBrightness", brakeActiveBrightness);
  brakeIdleBrightness = prefs.getInt("idleBrakeBrightness", brakeIdleBrightness);
  backgroundDimLevel = prefs.getInt("indicatorBackroundDim", backgroundDimLevel);
  dimTransitionSpeed = prefs.getInt("indicatorTransition", dimTransitionSpeed);
  colorTransitionDuration = prefs.getInt("colourTransition", 500);
  Serial.println("Loaded colorTransitionDuration: " + String(colorTransitionDuration));
  indicatorFlashTime = prefs.getInt("indicatorFlashTime", indicatorFlashTime);
  firstFlashDuration = prefs.getInt("firstFlashDuration", firstFlashDuration);
  trailUpdateInterval = prefs.getInt("trailFlashInterval", trailUpdateInterval);
  
  // Load saved indicator color
  String savedIndicatorColor = prefs.getString("indicatorColour", "");
  if (savedIndicatorColor.length() == 7 && savedIndicatorColor[0] == '#') {
    long number = strtol(savedIndicatorColor.substring(1).c_str(), NULL, 16);
    indicatorColor = CRGB((number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF);
  }
  
  printLoadedSettings();
}

void printLoadedSettings() {
  Serial.println("Loaded saved settings:");
  Serial.println("  NUM_LEDS: " + String(NUM_LEDS));
  Serial.println("  BRAKE_START_LED: " + String(BRAKE_START_LED));
  Serial.println("  BRAKE_END_LED: " + String(BRAKE_END_LED));
  Serial.println("  MAIN_LEN: " + String(MAIN_LEN));
  Serial.println("  trailLength: " + String(trailLength));
  Serial.println("  lightOverride: " + String(lightOverride));
  Serial.println("  bootDone: " + String(bootDone));
  Serial.println("  indicatorTrailMode: " + String(indicatorTrailMode));
  Serial.println("  leftTrailReverse: " + String(leftTrailReverse));
  Serial.println("  rightTrailReverse: " + String(rightTrailReverse));
  Serial.println("  reverseIndicatorSides: " + String(reverseIndicatorSides));
  Serial.println("  brakeActiveBrightness: " + String(brakeActiveBrightness));
  Serial.println("  brakeIdleBrightness: " + String(brakeIdleBrightness));
  Serial.println("  backgroundDimLevel: " + String(backgroundDimLevel));
  Serial.println("  indicatorFlashTime: " + String(indicatorFlashTime));
  Serial.println("  firstFlashDuration: " + String(firstFlashDuration));
  Serial.println("  trailUpdateInterval: " + String(trailUpdateInterval));
  Serial.println("  bootDurationMs: " + String(bootDurationMs));
}

// ===== NETWORK INITIALIZATION =====
void initializeNetwork() {
  // Set WiFi mode for faster startup
  WiFi.mode(WIFI_AP);
  
  // Configure WiFi for optimal iPhone connection
  WiFi.softAP(ssid, password, 6, 0, 4);  // Channel 6, max 4 connections
  Serial.print("Access Point IP: ");
  Serial.println(WiFi.softAPIP());

  // Start DNS server for captive portal
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  // Setup web server routes
  setupWebServerRoutes();
  
  // Add CORS headers for all responses
  server.enableCORS(true);
  server.enableCrossOrigin(true);
  
  // Configure server for large file uploads
  server.setContentLength(2 * 1024 * 1024); // 2MB max
  
  // Start web server
  server.begin();
}

void setupWebServerRoutes() {
  server.on("/", handleRoot);
  server.on("/settings", handleSettings);
  
  // Static file serving with proper headers
  server.on("/styles.css", HTTP_GET, handleStyles);
  server.on("/script.js", HTTP_GET, handleScript);
  server.on("/iro.min.js", HTTP_GET, handleIro);
  server.on("/settings-styles.css", HTTP_GET, handleSettingsStyles);
  server.on("/settings-script.js", HTTP_GET, handleSettingsScript);
  server.on("/manifest.json", HTTP_GET, handleManifest);
  
  // API endpoints
  server.on("/setColor", HTTP_GET, handleSetColor);
  server.on("/setEffect", HTTP_GET, handleSetEffect);
  server.on("/getState", HTTP_GET, handleGetState);
  server.on("/getSettings", HTTP_GET, handleGetSettings);
  server.on("/setSettings", HTTP_POST, handleSetSettings);
  server.on("/getGpioStates", HTTP_GET, handleGetGpioStates);
  
  // OTA update endpoint
  server.on("/update", HTTP_POST, []() {
    server.send(200, "text/plain", "Update successful");
  }, handleUpdate);
  
  // Factory reset endpoint
  server.on("/factoryReset", HTTP_POST, handleFactoryReset);
  
  // Redirect all other URLs to root (captive portal)
  server.onNotFound([]() {
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
  });
}

void initializeOTA() {
  ArduinoOTA.setHostname(ssid);
  ArduinoOTA.setPassword(password);

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
}

// ===== WEB SERVER API HANDLERS =====
void handleSetColor() {
  if (server.hasArg("color")) {
    String color = server.arg("color");
    Serial.println("Color set to: " + color);

    long number = strtol(color.substring(1).c_str(), NULL, 16);
    CRGB newColor = CRGB((number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF);
    
    // Use saved transition setting
    if (colorTransitionDuration > 0) {
      // Start color transition
      targetUnderglowColor = newColor;
      currentUnderglowColor = underglowColor;
      colorTransitionActive = true;
      colorTransitionStart = millis();
      Serial.println("Starting color transition: " + String(colorTransitionDuration) + "ms");
    } else {
      // No transition, set immediately
      underglowColor = newColor;
      currentUnderglowColor = newColor;
      targetUnderglowColor = newColor;
      colorTransitionActive = false;
    }

    prefs.putString("color", color);
  }
  server.send(200, "text/plain", "OK");
}

void handleSetEffect() {
  if (server.hasArg("effect")) {
    String effect = server.arg("effect");
    Serial.println("Effect set to: " + effect);
    currentEffect = stringToEffect(effect);
    selectedEffect = effect; // Keep string for web compatibility
    prefs.putString("effect", effect);
  }
  server.send(200, "text/plain", "OK");
}

void handleGetState() {
  String colorHex = "#";
  colorHex += String(underglowColor.r, HEX);
  colorHex += String(underglowColor.g, HEX);
  colorHex += String(underglowColor.b, HEX);

  // Pad hex digits if needed
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
}

void handleGetSettings() {
  String json = "{";
  
  // Get saved values from preferences, fallback to runtime variables
  int fullBrakeBrightness = prefs.getInt("fullBrakeBrightness", brakeActiveBrightness);
  int idleBrakeBrightness = prefs.getInt("idleBrakeBrightness", brakeIdleBrightness);
  int indicatorBackroundDim = prefs.getInt("indicatorBackroundDim", backgroundDimLevel);
  String bootColour = prefs.getString("bootColour", "#ffffff");
  String indicatorColour = prefs.getString("indicatorColour", "#ffff00");
  int indicatorTransition = prefs.getInt("indicatorTransition", dimTransitionSpeed);
  int colourTransition = prefs.getInt("colourTransition", 500); // Get color transition setting
  colorTransitionDuration = colourTransition; // Update runtime variable
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
  json += "\"colourTransition\":" + String(colourTransition) + ",";
  json += "\"indicatorFlashTime\":" + String(indicatorFlashTimeValue) + ",";
  json += "\"firstFlashDuration\":" + String(firstFlashDuration) + ",";
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
  json += "\"reverseIndicatorSides\":" + String(reverseIndicatorSides ? "true" : "false") + ",";
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
}

void handleSetSettings() {
  if (server.hasArg("plain")) {
    String json = server.arg("plain");
    Serial.println("Received settings: " + json);
    
    // Parse JSON and apply settings
    parseAndApplySettings(json);
    
    server.send(200, "text/plain", "Settings applied");
  } else {
    server.send(400, "text/plain", "No data received");
  }
}

void handleGetGpioStates() {
  String json = "{";
  json += "\"lightState\":" + String(!digitalRead(LIGHT_TOGGLE_PIN)) + ",";
  json += "\"brakeState\":" + String(!digitalRead(BRAKE_PIN)) + ",";
  json += "\"leftState\":" + String(!digitalRead(LEFT_PIN)) + ",";
  json += "\"rightState\":" + String(!digitalRead(RIGHT_PIN));
  json += "}";
  server.send(200, "application/json", json);
}

void handleUpdate() {
  // Handle file upload for OTA update
  HTTPUpload& upload = server.upload();
  
  static size_t totalBytes = 0;
  
  if (upload.status == UPLOAD_FILE_START) {
    totalBytes = 0;
    Serial.println("Starting firmware upload...");
    Serial.printf("Upload: %s, Size: %u\n", upload.filename.c_str(), upload.totalSize);
    
    // Validate file size (ESP32 typically has 4MB flash, limit to 2MB for safety)
    if (upload.totalSize > 2 * 1024 * 1024) {
      Serial.println("File too large");
      server.send(400, "text/plain", "File too large. Maximum size is 2MB.");
      return;
    }
    
    // Validate minimum file size (firmware should be at least 100KB)
    if (upload.totalSize < 100 * 1024) {
      Serial.println("File too small - likely not a valid firmware");
      server.send(400, "text/plain", "File too small - likely not a valid firmware file.");
      return;
    }
    
    // Start OTA update
    if (Update.begin(upload.totalSize)) {
      Serial.println("OTA update started successfully");
    } else {
      Serial.println("Failed to start OTA update");
      Serial.printf("Update error: %u\n", Update.getError());
      server.send(500, "text/plain", "Failed to start update");
      return;
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    // Write the uploaded data to OTA
    Serial.printf("Chunk size: %d\n", upload.currentSize);
    if (upload.currentSize > 0) {
      size_t bytesWritten = Update.write(upload.buf, upload.currentSize);
      if (bytesWritten != upload.currentSize) {
        Serial.printf("Failed to write to OTA. Expected: %d, Written: %d\n", upload.currentSize, bytesWritten);
        server.send(500, "text/plain", "Failed to write firmware");
        return;
      }
      totalBytes += bytesWritten;
      Serial.printf("Total written: %d\n", totalBytes);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    // End the OTA update
    Serial.printf("Upload completed. Total bytes written: %u\n", totalBytes);
    Serial.printf("Expected size: %u\n", upload.totalSize);
    
    if (Update.end()) {
      Serial.println("Firmware upload completed successfully");
      server.send(200, "text/plain", "Update successful");
      
      // Restart the device after a short delay
      delay(1000);
      ESP.restart();
    } else {
      Serial.println("Failed to end OTA update");
      Serial.printf("Update error: %u\n", Update.getError());
      server.send(500, "text/plain", "Failed to complete update");
      return;
    }
  }
}

void handleFactoryReset() {
  Serial.println("🔧 Factory reset requested!");
  
  // Send response first
  server.send(200, "text/plain", "Factory reset initiated. Device will restart in 3 seconds...");
  
  // Small delay to ensure response is sent
  delay(100);
  
  // Clear all preferences 
  prefs.clear();
  Serial.println("✅ All preferences cleared from memory");
  
  // Close preferences
  prefs.end();
  
  // Log restart
  Serial.println("🔄 Restarting ESP32 in 3 seconds...");
  Serial.flush(); // Ensure all serial output is sent
  
  // Give time for serial output and any web response
  delay(3000);
  
  // Restart the ESP32
  ESP.restart();
}

// ===== STATIC FILE HANDLERS =====
void handleStyles() {
  server.sendHeader("Content-Type", "text/css");
  server.sendHeader("Cache-Control", "public, max-age=86400"); // Cache for 24 hours
  server.send(200, "text/css", stylesCSS);
}

void handleScript() {
  server.sendHeader("Content-Type", "application/javascript");
  server.sendHeader("Cache-Control", "public, max-age=86400"); // Cache for 24 hours
  server.send(200, "application/javascript", scriptJS);
}

void handleIro() {
  server.sendHeader("Content-Type", "application/javascript");
  server.sendHeader("Cache-Control", "public, max-age=86400"); // Cache for 24 hours
  server.send(200, "application/javascript", iroJS);
}

void handleSettingsStyles() {
  server.sendHeader("Content-Type", "text/css");
  server.sendHeader("Cache-Control", "public, max-age=86400"); // Cache for 24 hours
  server.send(200, "text/css", settingsStylesCSS);
}

void handleSettingsScript() {
  server.sendHeader("Content-Type", "application/javascript");
  server.sendHeader("Cache-Control", "public, max-age=86400"); // Cache for 24 hours
  server.send(200, "application/javascript", settingsScriptJS);
}

void handleManifest() {
  server.sendHeader("Content-Type", "application/manifest+json");
  server.sendHeader("Cache-Control", "public, max-age=31536000"); // 1 year cache
  server.send(200, "application/manifest+json", manifestJSON);
}

// ===== SETTINGS PARSING =====
void parseAndApplySettings(String json) {
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
  applyRuntimeSettings(json, extractInt, extractBool, extractString);
  
  // Apply toggle settings
  applyToggleSettings(json, extractBool);
  
  // Apply color settings
  applyColorSettings(json, extractString);
  
  // Apply timing settings
  applyTimingSettings(json, extractInt);
  
  // Apply network settings
  applyNetworkSettings(json, extractString);
}

void applyRuntimeSettings(String json, int (*extractInt)(const String&, const String&), bool (*extractBool)(const String&, const String&), String (*extractString)(const String&, const String&)) {
  int trailLengthValue = extractInt(json, "indicatorTrailLength");
  if (trailLengthValue != -1) {
    trailLength = trailLengthValue;
    prefs.putInt("trailLength", trailLength);
    Serial.println("Updated trailLength: " + String(trailLength));
  }
  
  // Handle LED count settings
  int totalLedCount = extractInt(json, "totalLedCount");
  if (totalLedCount != -1 && totalLedCount > 0 && totalLedCount <= 300) {
    NUM_LEDS = totalLedCount;
    MAIN_LEN = NUM_LEDS - (BRAKE_END_LED - BRAKE_START_LED + 1);
    prefs.putInt("totalLedCount", NUM_LEDS);
    Serial.println("Updated NUM_LEDS: " + String(NUM_LEDS));
    Serial.println("Updated MAIN_LEN: " + String(MAIN_LEN));
  }
  
  int brakeLedStart = extractInt(json, "brakeLedStart");
  if (brakeLedStart != -1 && brakeLedStart >= 0 && brakeLedStart < NUM_LEDS) {
    BRAKE_START_LED = brakeLedStart;
    MAIN_LEN = NUM_LEDS - (BRAKE_END_LED - BRAKE_START_LED + 1);
    prefs.putInt("brakeLedStart", BRAKE_START_LED);
    Serial.println("Updated BRAKE_START_LED: " + String(BRAKE_START_LED));
    Serial.println("Updated MAIN_LEN: " + String(MAIN_LEN));
  }
  
  int brakeLedEnd = extractInt(json, "brakeLedEnd");
  if (brakeLedEnd != -1 && brakeLedEnd >= BRAKE_START_LED && brakeLedEnd < NUM_LEDS) {
    BRAKE_END_LED = brakeLedEnd;
    MAIN_LEN = NUM_LEDS - (BRAKE_END_LED - BRAKE_START_LED + 1);
    prefs.putInt("brakeLedEnd", BRAKE_END_LED);
    Serial.println("Updated BRAKE_END_LED: " + String(BRAKE_END_LED));
    Serial.println("Updated MAIN_LEN: " + String(MAIN_LEN));
  }
  
  bool xlightsToggle = extractBool(json, "xlightsToggle");
  if (xlightsToggle) {
    currentEffect = XLIGHTS;
    selectedEffect = "X-lights";
    prefs.putString("effect", selectedEffect);
    Serial.println("Updated selectedEffect: " + selectedEffect);
  }
}

void applyToggleSettings(String json, bool (*extractBool)(const String&, const String&)) {
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

  bool reverseSides = extractBool(json, "reverseIndicatorSides");
  reverseIndicatorSides = reverseSides;
  prefs.putBool("reverseSides", reverseSides);
  Serial.println("Updated reverseIndicatorSides: " + String(reverseSides));
}

void applyColorSettings(String json, String (*extractString)(const String&, const String&)) {
  String bootColour = extractString(json, "bootColour");
  if (bootColour.length() > 0) {
    long number = strtol(bootColour.substring(1).c_str(), NULL, 16);
    bootColor = CRGB((number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF);
    prefs.putString("bootColour", bootColour);
    Serial.println("Updated bootColor: " + bootColour);
  }
  
  String indicatorColour = extractString(json, "indicatorColour");
  if (indicatorColour.length() > 0) {
    long number = strtol(indicatorColour.substring(1).c_str(), NULL, 16);
    indicatorColor = CRGB((number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF);
    prefs.putString("indicatorColour", indicatorColour);
    Serial.println("Updated indicatorColor: " + indicatorColour);
  }
}

void applyTimingSettings(String json, int (*extractInt)(const String&, const String&)) {
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
  
  int firstFlashDurationValue = extractInt(json, "firstFlashDuration");
  if (firstFlashDurationValue != -1) {
    firstFlashDuration = firstFlashDurationValue;
    prefs.putInt("firstFlashDuration", firstFlashDurationValue);
    Serial.println("Updated firstFlashDuration: " + String(firstFlashDuration));
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
  
  int colourTransition = extractInt(json, "colourTransition");
  if (colourTransition != -1) {
    colorTransitionDuration = colourTransition;
    prefs.putInt("colourTransition", colourTransition);
    Serial.println("Updated colorTransitionDuration: " + String(colorTransitionDuration));
    Serial.println("Saved colourTransition to preferences: " + String(colourTransition));
  }
}

void applyNetworkSettings(String json, String (*extractString)(const String&, const String&)) {
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
}

// ===== MAIN LOOP =====
void loop() {
  unsigned long now = millis();
  loopStartTime = micros();  // Start timing measurement
  
  // High-priority non-blocking tasks (always run)
  dnsServer.processNextRequest();
  server.handleClient();
  ArduinoOTA.handle();
  updateStatusLed(); // Update status LED

  // Frame rate controlled LED updates
  if (now - lastFrameTime >= frameInterval) {
    lastFrameTime = now;
    
    if (!bootDone) {
      runBootEffect();
      // Handle background loading during boot effect
      handleBackgroundLoading();
      return;  // Skip rest of loop until boot effect done
    }
    
    // Handle background loading if boot effect was disabled
    if (!backgroundLoadingComplete) {
      handleBackgroundLoading();
      return;  // Skip rest of loop until background loading done
    }

    if (currentEffect == XLIGHTS) {
      handleDDP();
      FastLED.show();
      return;  // skip everything else when Xlights active
    }

    // Read input states
    updateInputStates();

    // Update color transitions
    updateColorTransition();

    // Run appropriate mode based on light toggle
    if (!lightToggle) {
      runSignalOnlyMode();
    } else {
      runFullEffectsMode();
    }

    FastLED.show();
    
    // Performance monitoring (logs every 5 seconds)
    if (now - lastPerformanceLog >= 5000) {
      unsigned long loopTime = micros() - loopStartTime;
      Serial.printf("Performance: Loop time: %luμs, FPS: %d, Free heap: %d bytes\n", 
                   loopTime, FRAMES_PER_SECOND, ESP.getFreeHeap());
      lastPerformanceLog = now;
    }
  }
}

// ===== INPUT PROCESSING =====
void updateInputStates() {
  lightToggle = lightOverride || (!digitalRead(LIGHT_TOGGLE_PIN));
  braking = !digitalRead(BRAKE_PIN);

  bool rawLeft = !digitalRead(LEFT_PIN);
  bool rawRight = !digitalRead(RIGHT_PIN);

  if (reverseIndicatorSides) {
    leftIndicating = rawRight;
    rightIndicating = rawLeft;
  } else {
    leftIndicating = rawLeft;
    rightIndicating = rawRight;
  }
}

// ===== DDP PROTOCOL HANDLING =====
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

// ===== BACKGROUND LOADING =====
void handleBackgroundLoading() {
  if (backgroundLoadingComplete) return;
  
  if (!backgroundLoadingStarted) {
    backgroundLoadingStarted = true;
    backgroundLoadStartTime = millis();
    Serial.println("Starting background loading...");
  }
  
  // Load all settings
  loadAllSettings();
  
  // Optimize WiFi for iPhone connections
  WiFi.setSleep(false);  // Disable WiFi sleep for faster response
  
  // Initialize WiFi and web server
  initializeNetwork();
  
  // Initialize DDP (only if X-lights mode is selected)
  if (selectedEffect.equalsIgnoreCase("X-lights")) {
    ddpUdp.begin(DDP_PORT);
    Serial.println("DDP listener started on port 4048");
  }
  
  // Initialize OTA
  initializeOTA();
  
  backgroundLoadingComplete = true;
  unsigned long loadTime = millis() - backgroundLoadStartTime;
  Serial.println("Background loading complete in " + String(loadTime) + "ms");
}

// ===== BOOT EFFECT =====
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
    if (bootStep < NUM_LEDS) {
      leds[bootStep] = bootColor;
    }
    if (NUM_LEDS - 1 - bootStep >= 0 && NUM_LEDS - 1 - bootStep < NUM_LEDS) {
      leds[NUM_LEDS - 1 - bootStep] = bootColor;
    }
    FastLED.show();

    bootStep++;
    bootLastStepTime = now;
  }
}

// ===== LIGHTING MODES =====
void runSignalOnlyMode() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  backgroundBrightness = 0;

  handleIndicatorsOverlay();
  updateBrakeLights();
}

void runFullEffectsMode() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  runEffect();
  handleIndicatorsOverlay();
  updateBrakeLights();
}

// ===== INDICATOR SYSTEM =====
void handleIndicatorsOverlay() {
  unsigned long now = millis();

  bool indicating = leftIndicating || rightIndicating;

  // Check for immediate start when indicating begins
  if (indicating && !wasIndicating) {
    indicatorFlashState = true;      // Start flashing immediately
    isFirstFlash = true;             // Mark this as the first flash
    lastIndicatorToggle = now;       // Reset timer for synchronized flashing
  }
  // Reset first flash flag when indicators are turned off
  if (!indicating && wasIndicating) {
    isFirstFlash = false;            // Reset first flash flag when stopping indication
  }
  wasIndicating = indicating;        // Update previous state

  // Smooth dimming for background brightness
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
    runFlashMode(leftSegmentStart, leftSegmentEnd, rightSegmentStart, rightSegmentEnd, now);
  } else {
    runTrailMode(leftSegmentStart, leftSegmentEnd, rightSegmentStart, rightSegmentEnd, 
                 leftSegmentLength, rightSegmentLength, now);
  }
}

void runFlashMode(int leftStart, int leftEnd, int rightStart, int rightEnd, unsigned long now) {
  // Use longer duration for the first flash, normal duration for subsequent flashes
  uint16_t currentFlashTime = isFirstFlash ? firstFlashDuration : indicatorFlashTime;
  
  if (now - lastIndicatorToggle >= currentFlashTime) {
    indicatorFlashState = !indicatorFlashState;
    lastIndicatorToggle = now;
    
    // Clear first flash flag after the first flash completes (turns off)
    if (isFirstFlash && !indicatorFlashState) {
      isFirstFlash = false;
    }
  }

  if (indicatorFlashState) {
    if (leftIndicating) {
      for (int i = leftStart; i <= leftEnd && i < NUM_LEDS; i++) {
        leds[i] = indicatorColor;
      }
    }

    if (rightIndicating) {
      for (int i = rightStart; i <= rightEnd && i < NUM_LEDS; i++) {
        leds[i] = indicatorColor;
      }
    }
  }
}

void runTrailMode(int leftStart, int leftEnd, int rightStart, int rightEnd, 
                  int leftLength, int rightLength, unsigned long now) {
  if (now - lastTrailUpdate >= trailUpdateInterval) {
    trailPosition++; // Increment without modulus
    lastTrailUpdate = now;
  }

  if (leftIndicating) {
    for (int i = 0; i < trailLength; i++) {
      int pos;
      if (leftTrailReverse) {
        // Reversed: Trail moves from end to start
        pos = (leftLength - 1) - ((trailPosition + i) % leftLength);
      } else {
        // Normal: Trail moves from start to end
        pos = (trailPosition + i) % leftLength;
      }
      int ledIndex = leftStart + pos;
      if (ledIndex >= leftStart && ledIndex <= leftEnd) {
        leds[ledIndex] = indicatorColor;
      }
    }
  }

  if (rightIndicating) {
    for (int i = 0; i < trailLength; i++) {
      int pos;
      if (rightTrailReverse) {
        // Reversed: Trail moves from end to start
        pos = (rightLength - 1) - ((trailPosition + i) % rightLength);
      } else {
        // Normal: Trail moves from start to end
        pos = (trailPosition + i) % rightLength;
      }
      int ledIndex = rightStart + pos;
      if (ledIndex >= rightStart && ledIndex <= rightEnd) {
        leds[ledIndex] = indicatorColor;
      }
    }
  }
}

// ===== BRAKE LIGHT SYSTEM =====
void updateBrakeLights() {
  uint8_t brightness;

  if (braking) {
    brightness = brakeActiveBrightness;
  } else {
    brightness = lightToggle ? brakeIdleBrightness : 0;
  }

  for (int i = BRAKE_START_LED; i <= BRAKE_END_LED && i < NUM_LEDS; i++) {
    leds[i] = CRGB(brightness, 0, 0);
  }
}

// ===== COLOR TRANSITION HANDLING =====
// Color transitions work for all background effects (solid, sparkle, pulse, chase, hazard)
// Rainbow effect uses HSV colors and is not affected by color transitions
// Indicators, brake lights, and boot effects use their own colors
void updateColorTransition() {
  if (!colorTransitionActive) return;
  
  unsigned long now = millis();
  unsigned long elapsed = now - colorTransitionStart;
  
  if (elapsed >= colorTransitionDuration) {
    // Transition complete
    underglowColor = targetUnderglowColor;
    currentUnderglowColor = targetUnderglowColor;
    colorTransitionActive = false;
    Serial.println("Color transition complete");
  } else {
    // Calculate progress (0.0 to 1.0)
    float progress = (float)elapsed / colorTransitionDuration;
    
    // Interpolate between current and target colors
    currentUnderglowColor.r = currentUnderglowColor.r + (targetUnderglowColor.r - currentUnderglowColor.r) * progress;
    currentUnderglowColor.g = currentUnderglowColor.g + (targetUnderglowColor.g - currentUnderglowColor.g) * progress;
    currentUnderglowColor.b = currentUnderglowColor.b + (targetUnderglowColor.b - currentUnderglowColor.b) * progress;
    
    // Update the main color variable
    underglowColor = currentUnderglowColor;
  }
}

// ===== LED EFFECTS =====
void runEffect() {
  rainbowOffset++;

  switch (currentEffect) {
    case RAINBOW:
      runRainbowEffect();
      break;
    case SOLID:
      runSolidEffect();
      break;
    case SPARKLE:
      runSparkleEffect();
      break;
    case PULSE:
      runPulseEffect();
      break;
    case CHASE:
      runChaseEffect();
      break;
    case HAZARD:
      runHazardEffect();
      break;
    default:
      fill_solid(leds, MAIN_LEN, CRGB::Black);
      break;
  }
}

void runRainbowEffect() {
  // Rainbow uses HSV colors - not affected by color transitions
  // Rainbow cycles through all hues automatically
  for (int i = 0; i < NUM_LEDS; i++) {
    if (isBrakeLED(i)) continue;
    uint8_t hue = (i * 255 / MAIN_LEN + rainbowOffset) % 255;
    leds[i] = CHSV(hue, 255, backgroundBrightness);
  }
}

void runSolidEffect() {
  CRGB scaledColor = currentUnderglowColor;  // Use transitioning color
  scaledColor.nscale8_video(backgroundBrightness);
  for (int i = 0; i < NUM_LEDS; i++) {
    if (isBrakeLED(i)) continue;
    leds[i] = scaledColor;
  }
}

void runSparkleEffect() {
  CRGB baseColor = currentUnderglowColor;  // Use transitioning color
  baseColor.nscale8_video(backgroundBrightness / 2);
  for (int i = 0; i < NUM_LEDS; i++) {
    if (isBrakeLED(i)) continue;
    leds[i] = baseColor;
  }

  int sparkleIndex = random(MAIN_LEN);
  if (sparkleIndex >= 0 && sparkleIndex < NUM_LEDS) {
    leds[sparkleIndex] = CRGB::White;  // Sparkle white pixel
  }
}

void runPulseEffect() {
  pulseBrightness += pulseDirection;
  if (pulseBrightness <= 50 || pulseBrightness >= backgroundBrightness) {
    pulseDirection = -pulseDirection;  // reverse direction
    pulseBrightness = constrain(pulseBrightness, 50, backgroundBrightness);
  }
  CRGB scaledColor = currentUnderglowColor;  // Use transitioning color
  scaledColor.nscale8_video(pulseBrightness);
  for (int i = 0; i < NUM_LEDS; i++) {
    if (isBrakeLED(i)) continue;
    leds[i] = scaledColor;
  }
}

void runChaseEffect() {
  int chaseLength = 40;       // Number of LEDs lit at once
  int fadeAmount = 150;      // Trail fade speed (0-255)

  // Fade all LEDs to create trailing effect
  for (int i = 0; i < NUM_LEDS; i++) {
    if (isBrakeLED(i)) continue;
    leds[i].nscale8(fadeAmount);
  }

  // Light up chaseLength LEDs starting at chasePosition
  for (int i = 0; i < chaseLength; i++) {
    int ledIndex = (chasePosition + i) % MAIN_LEN;
    if (ledIndex >= 0 && ledIndex < NUM_LEDS) {
      leds[ledIndex] = currentUnderglowColor;  // Use transitioning color
    }
  }

  // Move the chase forward
  chasePosition = (chasePosition + 1) % MAIN_LEN;
}

void runHazardEffect() {
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
        for (int i = leftSegmentStart; i <= leftSegmentEnd && i < NUM_LEDS; i++) {
          leds[i] = currentUnderglowColor;  // Use transitioning color
        }
      } else {
        // Right side flash
        for (int i = rightSegmentStart; i <= rightSegmentEnd && i < NUM_LEDS; i++) {
          leds[i] = currentUnderglowColor;  // Use transitioning color
        }
      }
    }
  }
}

// ===== STATUS LED CONTROL =====
void updateStatusLed() {
  unsigned long now = millis();
  
  // If boot effect is still running, do flash pattern
  if (!bootDone) {
    // Initialize boot flash pattern
    if (!statusLedBootFlashActive) {
      statusLedBootFlashActive = true;
      statusLedBootFlashTime = now;
      statusLedBootFlashCount = 0;
      statusLedBootFlashState = false;
      statusLedBootFlashGroup = 0;
      digitalWrite(STATUS_LED_PIN, LOW); // Start with LED off
    }
    
    // Boot flash pattern: 2 quick flashes, pause, repeat
    // Each flash: 200ms on, 200ms off, then 1000ms pause
    unsigned long cycleTime = now - statusLedBootFlashTime;
    unsigned long groupTime = cycleTime % 1600; // 1600ms total cycle (800ms flash + 800ms pause)
    
    if (groupTime < 800) { // First 800ms: flash twice
      unsigned long flashTime = groupTime % 400; // 400ms per flash (200ms on + 200ms off)
      bool shouldBeOn = (flashTime < 200); // First 200ms on, second 200ms off
      
      if (shouldBeOn != statusLedBootFlashState) {
        statusLedBootFlashState = shouldBeOn;
        digitalWrite(STATUS_LED_PIN, shouldBeOn ? HIGH : LOW);
      }
    } else { // Second 400ms: pause
      if (statusLedBootFlashState) {
        statusLedBootFlashState = false;
        digitalWrite(STATUS_LED_PIN, LOW);
      }
    }
    
    return;
  }
  
  // After boot, start pulsing
  if (!statusLedPulseActive) {
    statusLedPulseActive = true;
    statusLedBrightness = 100;
    statusLedDirection = -1; // Start by decreasing
  }
  
  // Update every 50ms for optimized pulsing
  if (now - statusLedLastUpdate >= 50) {
    statusLedLastUpdate = now;
    
    // Update brightness
    statusLedBrightness += statusLedDirection;
    
    // Change direction at boundaries
    if (statusLedBrightness >= 100) {
      statusLedBrightness = 100;
      statusLedDirection = -1;
    } else if (statusLedBrightness <= 0) {
      statusLedBrightness = 0;
      statusLedDirection = 1;
    }
    
    // Apply brightness using PWM (0-100 to 0-255)
    uint8_t pwmValue = map(statusLedBrightness, 0, 100, 0, 255);
    analogWrite(STATUS_LED_PIN, pwmValue);
  }
}
