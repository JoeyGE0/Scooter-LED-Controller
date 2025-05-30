#include <FastLED.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>

// ===== CONFIG =====
#define NUM_LEDS        169
#define DATA_PIN        16

#define BRAKE_LEN       40
#define MAIN_LEN        (NUM_LEDS - BRAKE_LEN)

#define RIGHT_PIN       25
#define LEFT_PIN        27
#define BRAKE_PIN       32
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
const char* ssid = "NamiLEDS";
const char* password = "Joeyscoots123";

const byte DNS_PORT = 53;
DNSServer dnsServer;

WebServer server(80);

// Serve your custom captive portal HTML here
const char* captivePortalHTML = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Scooter LED Control</title>
  <style>
    body {
      margin: 0;
      padding: 0;
      background: #121212;
      color: #f1f1f1;
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Oxygen,
        Ubuntu, Cantarell, "Open Sans", "Helvetica Neue", sans-serif;
      -webkit-font-smoothing: antialiased;
      -moz-osx-font-smoothing: grayscale;
    }

    header {
      padding: 1.4rem 0;
      text-align: center;
      border-bottom: 1px solid #2a2a2a;
      font-weight: 600;
      font-size: 1.7rem;
    }

    main {
      padding: 1.8rem 2rem 5rem;
    }

    .loader {
      position: fixed;
      top: 0;
      left: 0;
      width: 100vw;
      height: 100vh;
      background: rgba(0, 0, 0, 0.6);
      display: flex;
      justify-content: center;
      align-items: center;
      z-index: 9999;
    }

    .spinner {
      width: 3em;
      height: 3em;
      cursor: not-allowed;
      border-radius: 50%;
      border: 2px solid #444;
      box-shadow:
        -10px -10px 10px #6359f8,
        0px -10px 10px #9c32e2,
        10px -10px 10px #f36896,
        10px 0 10px #ff0b0b,
        10px 10px 10px #ff5500,
        0 10px 10px #ff9500,
        -10px 10px 10px #ffb700;
      animation: rot55 0.7s linear infinite;
    }

    .spinnerin {
      border: 2px solid #444;
      width: 1.5em;
      height: 1.5em;
      border-radius: 50%;
      position: absolute;
      top: 50%;
      left: 50%;
      transform: translate(-50%, -50%);
    }

    @keyframes rot55 {
      to {
        transform: rotate(360deg);
      }
    }

    .loader.hidden {
      display: none;
    }

    .bottom-nav {
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 1rem;
      background: #1e1e1e;
      border-top: 1px solid #333;
      position: fixed;
      bottom: 0;
      width: 100%;
    }

    .nav-btn {
      background: none;
      border: none;
      color: #fff;
      font-size: 1.5rem;
      cursor: pointer;
    }

    .nav-title {
      font-size: 1.2rem;
      font-weight: bold;
    }

    .color-section label {
      display: block;
      margin-bottom: 0.5rem;
    }

    .swatchGrid {
      display: flex;
      flex-wrap: wrap;
      gap: 0.5rem;
    }

    .swatch {
      width: 40px;
      height: 40px;
      border-radius: 4px;
      cursor: pointer;
      border: 2px solid #333;
    }

    .effects-section h2 {
      margin-top: 2rem;
    }

    .effects-bar {
      margin-top: 1rem;
    }

    .effect-dropdown {
      position: relative;
      display: inline-block;
    }

    .dropdown-toggle {
      background: #333;
      color: white;
      padding: 0.6rem 1rem;
      border: none;
      border-radius: 4px;
      cursor: pointer;
    }

    .dropdown-list {
      display: none;
      position: absolute;
      background: #222;
      box-shadow: 0 8px 16px rgba(0,0,0,0.2);
      border-radius: 4px;
      overflow: hidden;
      z-index: 1000;
    }

    .effect-dropdown:hover .dropdown-list {
      display: block;
    }

    .effect-btn {
      background: none;
      border: none;
      color: white;
      padding: 0.8rem 1.2rem;
      text-align: left;
      width: 100%;
      cursor: pointer;
    }

    #effect-speed {
      margin-top: 1rem;
      width: 100%;
    }
  </style>
</head>
<body>

  <div id="loader" class="loader">
    <div class="spinner">
      <div class="spinnerin"></div>
    </div>
  </div>

  <nav class="bottom-nav">
    <button class="nav-btn power-btn" title="Power Toggle">⏻</button>
    <div class="nav-title">Main</div>
    <button class="nav-btn settings-btn" title="Settings" onclick="window.location.href='settings.html'">⚙</button>
  </nav>

  <main>
    <section class="color-section">
      <label for="color-picker">LED Colour</label>
      <div id="color-picker-ui"></div>
      <br>
      <div class="swatchGrid" id="swatch-grid">
        <div class="swatch" data-color="#ff0000" style="background:#ff0000;"></div>
        <div class="swatch" data-color="#00FF00" style="background:#00FF00;"></div>
        <div class="swatch" data-color="#0000FF" style="background:#0000FF;"></div>
        <div class="swatch" data-color="#00FFFF" style="background:#00FFFF;"></div>
        <div class="swatch" data-color="#FF00FF" style="background:#FF00FF;"></div>
        <div class="swatch" data-color="#ffffff" style="background:#ffffff;"></div>
      </div>
    </section>

    <section class="effects-section">
      <h2>Effects</h2>
      <div class="effects-bar">
        <div class="effect-dropdown">
          <button class="dropdown-toggle">Select Effect ▼</button>
          <div class="dropdown-list">
            <button class="effect-btn" data-effect="static">Static</button>
            <button class="effect-btn" data-effect="breathing">Breathing</button>
            <button class="effect-btn" data-effect="rainbow">Rainbow</button>
            <button class="effect-btn" data-effect="chase">Chase</button>
            <button class="effect-btn" data-effect="wave">Wave</button>
            <button class="effect-btn" data-effect="off">Off</button>
          </div>
        </div>
        <input type="range" id="effect-speed" min="1" max="100" value="50">
      </div>
    </section>
  </main>

</body>
</html>

)rawliteral";

void handleRoot() {
  server.send(200, "text/html", captivePortalHTML);
}

void setup() {
  Serial.begin(115200);

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
