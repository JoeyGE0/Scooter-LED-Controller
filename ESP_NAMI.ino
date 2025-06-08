#include <FastLED.h> // For LED Control
#include <WiFi.h> // Needed for captive portal
#include <DNSServer.h> // Needed for captive portal
#include <WebServer.h> // Needed for captive portal
#include <ArduinoOTA.h> // For OTA updates
#include <Preferences.h> // saves settings and sates in non-volatile storage
#include "UI.h" // Needed for main UI
#include "Settings.h" // Needed for settings page
Preferences prefs;


// ===== DEFINES =====
// Hardware and LED config
#define NUM_LEDS        168    // Total LED count
#define DATA_PIN        16     // LED data output pin

#define BRAKE_LEN       38     // Number of LEDs reserved for brake segment
#define MAIN_LEN        (NUM_LEDS - BRAKE_LEN)

#define RIGHT_PIN       26     // Right indicator input pin (HIGH/LOW)
#define LEFT_PIN        27     // Left indicator input pin (HIGH/LOW)
#define BRAKE_PIN       13     // Brake input pin (HIGH/LOW)
#define LIGHT_TOGGLE_PIN 17    // Light toggle input pin (HIGH/LOW)

// Brightness and timing constants
#define BRAKE_IDLE_BRIGHTNESS    50    // Brake LEDs brightness when idle (0-255)
#define BRAKE_ACTIVE_BRIGHTNESS  255   // Brake LEDs brightness when active (0-255)

#define INDICATOR_COLOR          CRGB(255, 80, 0) // Indicator LED colour (R,G,B)
#define INDICATOR_FLASH_TIME     300   // Indicator flash interval in ms
#define BACKGROUND_DIM_LEVEL     50    // Dim level for background LEDs (0-255)
#define DIM_TRANSITION_SPEED     20    // Speed for dimming transition in ms

#define BOOT_COLOR               CRGB::White // Colour for boot effect
#define BOOT_DURATION_MS         2210        // Total boot effect duration in ms
#define BOOT_STEPS               (MAIN_LEN / 2)   // Steps in boot effect
#define BOOT_STEP_DELAY          (BOOT_DURATION_MS / BOOT_STEPS) // Delay per boot step

#define TRAIL_UPDATE_INTERVAL    15    // Interval between trail animation steps (ms)


// ===== GLOBAL OBJECTS =====
CRGB leds[NUM_LEDS];  // LED array


// ===== STATE VARIABLES =====
// Toggle states
bool lightToggle = false;
bool braking = false;
bool leftIndicating = false;
bool rightIndicating = false;

// Boot effect state
bool bootDone = false; // set true to skip boot effect
unsigned long bootLastStepTime = 0;
int bootStep = 0;

// Indicator and underglow state
uint8_t backgroundBrightness = 255; // brightness for background LEDs
bool indicatorFlashState = false;   // flashing state for indicators

String selectedEffect = "rainbow";  // Default LED effect
CRGB underglowColor = CRGB::Blue;   // Default underglow colour

// Indicator trail mode and animation
bool indicatorTrailMode = false;    // false = flash mode, true = trail mode
int trailLength = 40;               // Length of the indicator trail
int trailPosition = 0;              // Current position for trail animation
bool leftTrailReverse = false;      // Reverse direction for left indicator trail
bool rightTrailReverse = true;      // Reverse direction for right indicator trail
unsigned long lastTrailUpdate = 0;  // Last time trail updated

// Timing helpers for indicators and dimming
unsigned long lastIndicatorToggle = 0;
unsigned long lastDimUpdate = 0;

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

  bool indicating = leftIndicating || rightIndicating;

  // Smooth dimming for background brightness (same for both modes)
  if (now - lastDimUpdate >= DIM_TRANSITION_SPEED) {
    if (indicating && backgroundBrightness > BACKGROUND_DIM_LEVEL) {
      backgroundBrightness -= 5;
    } else if (!indicating && backgroundBrightness < 255) {
      backgroundBrightness += 5;
    }
    backgroundBrightness = constrain(backgroundBrightness, BACKGROUND_DIM_LEVEL, 255);
    lastDimUpdate = now;
  }

  int mid = MAIN_LEN / 2;

  if (!indicatorTrailMode) {
    // Classic flash mode
    if (now - lastIndicatorToggle >= INDICATOR_FLASH_TIME) {
      indicatorFlashState = !indicatorFlashState;
      lastIndicatorToggle = now;
    }

    if (indicatorFlashState) {
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
  } else {
    // Trail mode — NO fadeToBlackBy anymore!

    if (now - lastTrailUpdate >= TRAIL_UPDATE_INTERVAL) {
      trailPosition = (trailPosition + 1) % mid; // note: use mid for wrap on left side, for right side handle separately
      lastTrailUpdate = now;
    }

    if (leftIndicating) {
      // No fadeToBlackBy, just overlay trail on left side
      for (int i = 0; i < mid; i++) {
        // Could optionally clear or set background with scaled brightness here
        // but don't fade to black
      }

      for (int i = 0; i < trailLength; i++) {
        int pos = (trailPosition + i) % mid;
        if (leftTrailReverse) {
          pos = mid - 1 - pos;
        }
        leds[pos] = INDICATOR_COLOR;
      }
    }

    if (rightIndicating) {
      // No fadeToBlackBy, just overlay trail on right side
      for (int i = mid; i < MAIN_LEN; i++) {
        // same as left, no fadeToBlackBy
      }

      for (int i = 0; i < trailLength; i++) {
        int pos = (trailPosition + i) % (MAIN_LEN - mid);
        if (rightTrailReverse) {
          pos = (MAIN_LEN - 1) - pos;
        } else {
          pos = mid + pos;
        }
        leds[pos] = INDICATOR_COLOR;
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
    CRGB baseColor = underglowColor;
    baseColor.nscale8_video(backgroundBrightness / 2);
    for (int i = 0; i < MAIN_LEN; i++) {
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
    for (int i = 0; i < MAIN_LEN; i++) {
      leds[i] = scaledColor;
    }
  } else if (selectedEffect.equalsIgnoreCase("chase")) {
    int chaseLength = 8;       // Number of LEDs lit at once — just change this number for bigger or smaller
    int fadeAmount = 150;      // Trail fade speed (0-255)

    // Fade all LEDs to create trailing effect
    for (int i = 0; i < MAIN_LEN; i++) {
      leds[i].nscale8(fadeAmount);
    }

    // Light up chaseLength LEDs starting at chasePosition
    for (int i = 0; i < chaseLength; i++) {
      int ledIndex = (chasePosition + i) % MAIN_LEN;
      leds[ledIndex] = underglowColor;
    }

    // Move the chase forward
    chasePosition = (chasePosition + 1) % MAIN_LEN;
  }

  else {
    fill_solid(leds, MAIN_LEN, CRGB::Black);
  }
}
