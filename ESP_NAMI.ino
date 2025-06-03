#include <FastLED.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include "UI.h"


// ===== CONFIG =====
#define NUM_LEDS        141
#define DATA_PIN        16

#define BRAKE_LEN       40
#define MAIN_LEN        (NUM_LEDS - BRAKE_LEN)

#define RIGHT_PIN       26
#define LEFT_PIN        27
#define BRAKE_PIN       13
#define LIGHT_TOGGLE_PIN 17

#define BRAKE_IDLE_BRIGHTNESS    50
#define BRAKE_ACTIVE_BRIGHTNESS  255

#define INDICATOR_FLASH_TIME     300
#define BACKGROUND_DIM_LEVEL     50
#define DIM_TRANSITION_SPEED     20

CRGB leds[NUM_LEDS];

// ===== STATE =====
bool lightToggle = false;
bool braking = false;
bool leftIndicating = false;
bool rightIndicating = false;

uint8_t backgroundBrightness = 255;
bool indicatorFlashState = false;

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

  // Start captive portal AP
  WiFi.softAP(ssid, password);
  Serial.print("Access Point started. IP: ");
  Serial.println(WiFi.softAPIP());

  // Start DNS server to catch all requests and redirect to captive portal IP
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  // Setup web server routes
  server.on("/", handleRoot);
  server.onNotFound([]() {
    // Redirect any other URL to root (captive portal)
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
  });

  server.begin();
}

void loop() {
  // Handle captive portal stuff first
  dnsServer.processNextRequest();
  server.handleClient();

  // Your existing input reads
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

  runRainbowUnderglow();
  handleIndicatorsOverlay();
  updateBrakeLights();
}

// ===== RAINBOW UNDERGLOW =====
void runRainbowUnderglow() {
  rainbowOffset++;

  for (int i = 0; i < MAIN_LEN; i++) {
    uint8_t hue = (i * 255 / MAIN_LEN + rainbowOffset) % 255;
    leds[i] = CHSV(hue, 255, backgroundBrightness);
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
        leds[i] = CRGB(255, 165, 0);  // Orange
      }
    }

    if (rightIndicating) {
      for (int i = mid; i < MAIN_LEN; i++) {
        leds[i] = CRGB(255, 165, 0);  // Orange
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
