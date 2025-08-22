# Scooter-LED-Controller

A **fully addressable scooter lighting system** powered by an ESP32 and FastLED.  
In my setup, it uses the scooter's 12V signals for turn indicators, brake lights, and the light button.

> ⚠️ **Note:** These 12V inputs **must** be stepped down to a safe voltage for the ESP32 (OPTO isolator recommended).

## 🚧 Status

**Beta 3** — Major architectural overhaul with PlatformIO migration, performance improvements, and new features. Currently functional with some UI limitations.

## ✨ Features

- **Web-based UI** for control and settings (PWA support)
- **Fully addressable underglow effects** via FastLED
- **Turn indicators and brake light logic** with enhanced timing
- **xLights/DDP support** for advanced lighting control
- **OTA updates** for easy firmware changes
- **Color transitions** with smooth animations
- **Status LED** with boot patterns and monitoring
- **Performance optimization** with 120 FPS frame control
- **Modular architecture** with PlatformIO development

## 🔧 Requirements

- **ESP32 development board** (tested on ESP32-S3, compatible with other ESP32 chips)
- **WS2815 LED strip** (up to 300 LEDs)
- **PlatformIO development environment**
- **FastLED library**

## 📦 Installation

1. Clone this repository
2. Open in PlatformIO
3. Configure your LED count and GPIO pins in `src/main.cpp`
4. Build and upload to your ESP32
5. Connect to the "Nami-LEDS" WiFi network
6. Access the web interface at the ESP32's IP address

---

<p align="left">
  <a href="https://joeyge0.github.io/Scooter-LED-Controller/" target="_blank" rel="noopener noreferrer">
    <img src="Images/DEV%20UI.png" alt="DEV UI" width="340" />
  </a>
</p>

<p align="left">
  <a href="https://joeyge0.github.io/Scooter-LED-Controller/" target="_blank" rel="noopener noreferrer">
    Example Frontend UI
  </a>
</p>

---

## 🚀 Recent Updates (Beta 3)

- **PlatformIO Migration** - Modern development environment
- **Modular Architecture** - Separated HTML, CSS, and JavaScript
- **Performance Improvements** - 120 FPS frame control, optimized loops
- **New Features** - Color transitions, status LED, enhanced indicators
- **PWA Support** - Can be installed as web app
- **Better Error Handling** - Comprehensive validation and feedback

## 🤝 Contributing

Any help is hugely appreciated! I'm still learning to code, so I'm building this with a mix of my own work, ChatGPT, and Cursor. If you've got ideas, fixes, or optimizations, feel free to open a pull request.
