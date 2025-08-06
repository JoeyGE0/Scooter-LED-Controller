# Scooter-LED-Controller

A **fully addressable scooter lighting system** powered by an ESP32 and FastLED.  
In my setup, it uses the scooter’s 12V signals for turn indicators, brake lights, and the light button.  
> ⚠️ **Note:** These 12V inputs **must** be stepped down to a safe voltage for the ESP32 (OPTO isolator recommended).

## 🚧 Status

**Still in development** — currently functional but has a few bugs. You can use it now, but I recommend waiting for a stable release.

## ✨ Features

- Web-based UI for control and settings  
- Fully addressable underglow effects via FastLED  
- Turn indicators and brake light logic  
- xLights/DDP support  
- OTA updates for easy firmware changes

---

<p align="left">
  <a href="https://joeyge0.github.io/Scooter-LED-Controller/" target="_blank" rel="noopener noreferrer">
    <img src="DEV%20UI.png" alt="DEV UI" width="340" />
  </a>
</p>

<p align="left">
  <a href="https://joeyge0.github.io/Scooter-LED-Controller/" target="_blank" rel="noopener noreferrer">
    Example Frontend UI
  </a>
</p>

---

## 🤝 Contributing

Any help is hugely appreciated! I’m still learning to code, so I’m building this with a mix of my own work, ChatGPT, and Cursor. If you’ve got ideas, fixes, or optimizations, feel free to open a pull request.