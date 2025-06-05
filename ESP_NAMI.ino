#include <FastLED.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include "UI.h"
#include <ArduinoOTA.h>
#include <Preferences.h>
Preferences prefs;


// ===== CONFIG =====
#define NUM_LEDS        168 // Total LED count
#define DATA_PIN        16 // LED data output

#define BRAKE_LEN       38 // Brake segment LED count
#define MAIN_LEN        (NUM_LEDS - BRAKE_LEN)

#define RIGHT_PIN       26 // Right indicator input pin (HIGH/LOW)
#define LEFT_PIN        27 // Left indicator input pin (HIGH/LOW)
#define BRAKE_PIN       13 // Brake input pin (HIGH/LOW)
#define LIGHT_TOGGLE_PIN 17 // Light toggle input pin (HIGH/LOW)

#define BRAKE_IDLE_BRIGHTNESS    50 // (0-255)
#define BRAKE_ACTIVE_BRIGHTNESS  255 // (0-255)

#define INDICATOR_COLOR CRGB(255, 80, 0)
#define INDICATOR_FLASH_TIME     300 // (MS)
#define BACKGROUND_DIM_LEVEL     50 // (0-255)
#define DIM_TRANSITION_SPEED     20 // (MS)

CRGB leds[NUM_LEDS];

#define BOOT_COLOR      CRGB::White //colour of boot effect
#define BOOT_DURATION_MS 2210 // Duration and speed of the boot effect
#define BOOT_STEPS      (MAIN_LEN / 2)
#define BOOT_STEP_DELAY (BOOT_DURATION_MS / BOOT_STEPS)


// ===== STATE =====
bool lightToggle = false; // True or False
bool braking = false; // True or False
bool leftIndicating = false; // True or False
bool rightIndicating = false; // True or False

bool bootDone = false; // make this true to disable boot effect
unsigned long bootLastStepTime = 0;
int bootStep = 0;

uint8_t backgroundBrightness = 255; // (0-255)
bool indicatorFlashState = false; // True or False
String selectedEffect = "rainbow";  // default effect
CRGB underglowColor = CRGB::Blue;   // default underglow colour




unsigned long lastIndicatorToggle = 0;
unsigned long lastDimUpdate = 0;

uint16_t rainbowOffset = 0;  // For moving rainbow

// ===== WIFI CAPTIVE PORTAL SETUP =====
const char* ssid = "Nami-LEDS";
const char* password = "Joeyrides123";

const byte DNS_PORT = 53;
DNSServer dnsServer;

WebServer server(80);

// Sends the UI 
void handleRoot() {
  server.send_P(200, "text/html", captivePortalHTML);

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
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
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



  // Start DNS server to redirect to captive portal IP
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  // Setup web server routes
  server.on("/", handleRoot);
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



  server.begin();
}

void runBootEffect() {
  unsigned long now = millis();
  if (bootStep >= BOOT_STEPS) {
    bootDone = true;
    // Leave LEDs fully lit white after boot
    for (int i = 0; i < MAIN_LEN; i++) {
      leds[i] = BOOT_COLOR;
    }
    FastLED.show();
    return;
  }

  if (now - bootLastStepTime >= BOOT_STEP_DELAY) {
    leds[bootStep] = BOOT_COLOR;
    leds[MAIN_LEN - 1 - bootStep] = BOOT_COLOR;
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

  lightToggle     = !digitalRead(LIGHT_TOGGLE_PIN);
  braking         = !digitalRead(BRAKE_PIN);
  leftIndicating  = !digitalRead(LEFT_PIN);
  rightIndicating = !digitalRead(RIGHT_PIN);

  if (!lightToggle) {
    runSignalOnlyMode();
  } else {
    runFullEffectsMode();
  }

  FastLED.show();
  delay(10);
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

void runUnderglow() {
  rainbowOffset++;

  if (selectedEffect == "rainbow") {
    for (int i = 0; i < MAIN_LEN; i++) {
      uint8_t hue = (i * 255 / MAIN_LEN + rainbowOffset) % 255;
      leds[i] = CHSV(hue, 255, backgroundBrightness);
    }
  } else if (selectedEffect == "solid") {
    CRGB scaledColor = underglowColor;
    scaledColor.nscale8_video(backgroundBrightness);
    for (int i = 0; i < MAIN_LEN; i++) {
      leds[i] = scaledColor;
    }
  } else {
    // fallback to off
    fill_solid(leds, MAIN_LEN, CRGB::Black);
  }
}


// ===== INDICATOR OVERLAY =====
void handleIndicatorsOverlay() {
  unsigned long now = millis();

  // Flash toggle
  if (now - lastIndicatorToggle >= INDICATOR_FLASH_TIME) {
    indicatorFlashState = !indicatorFlashState;
    lastIndicatorToggle = now;
  }

  bool indicating = leftIndicating || rightIndicating;

  // Smooth dimming
  if (now - lastDimUpdate >= DIM_TRANSITION_SPEED) {
    if (indicating && backgroundBrightness > BACKGROUND_DIM_LEVEL) {
      backgroundBrightness -= 5;
    } else if (!indicating && backgroundBrightness < 255) {
      backgroundBrightness += 5;
    }
    backgroundBrightness = constrain(backgroundBrightness, BACKGROUND_DIM_LEVEL, 255);
    lastDimUpdate = now;
  }

  // Draw indicators
  if (indicatorFlashState) {
    int mid = MAIN_LEN / 2;

    if (leftIndicating) {
      for (int i = 0; i < mid; i++) {
        leds[i] = INDICATOR_COLOR;
      }
    }

    if (rightIndicating) {
      for (int i = mid; i < MAIN_LEN; i++) {
        leds[i] = INDICATOR_COLOR;
      }
    }
  }
}

// ===== BRAKE LIGHT SEGMENT =====
void updateBrakeLights() {
  uint8_t brightness;

  if (braking) {
    brightness = BRAKE_ACTIVE_BRIGHTNESS;
  } else {
    brightness = lightToggle ? BRAKE_IDLE_BRIGHTNESS : 0;
  }

  for (int i = MAIN_LEN; i < NUM_LEDS; i++) {
    leds[i] = CRGB(brightness, 0, 0);
  }
}

void runEffect() {
  rainbowOffset++;

  if (selectedEffect.equalsIgnoreCase("rainbow")) {
    for (int i = 0; i < MAIN_LEN; i++) {
      uint8_t hue = (i * 255 / MAIN_LEN + rainbowOffset) % 255;
      leds[i] = CHSV(hue, 255, backgroundBrightness);
    }
  } else if (selectedEffect.equalsIgnoreCase("solid")) {
    CRGB scaledColor = underglowColor;
    scaledColor.nscale8_video(backgroundBrightness);
    for (int i = 0; i < MAIN_LEN; i++) {
      leds[i] = scaledColor;
    }
  } else if (selectedEffect.equalsIgnoreCase("sparkle")) {
    // Simple sparkle effect on top of solid background
    CRGB baseColor = underglowColor;
    baseColor.nscale8_video(backgroundBrightness / 2);
    for (int i = 0; i < MAIN_LEN; i++) {
      leds[i] = baseColor;
    }

    int sparkleIndex = random(MAIN_LEN);
    leds[sparkleIndex] = CRGB::White;  // Sparkle white pixel
  } else {
    fill_solid(leds, MAIN_LEN, CRGB::Black);
  }
}
