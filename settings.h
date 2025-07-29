#ifndef Settings_H
#define Settings_H

const char* settingsPageHTML = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Settings - Scooter LEDs</title>
<style>
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
  box-shadow: -10px -10px 10px #6359f8, 0px -10px 10px 0px #9c32e2,
    10px -10px 10px #f36896, 10px 0 10px #ff0b0b, 10px 10px 10px 0px#ff5500,
    0 10px 10px 0px #ff9500, -10px 10px 10px 0px #ffb700;
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

/* Collapsible sections */
.collapsible-section {
  border: 1px solid #2a2a2a;
  border-radius: 8px;
  margin-bottom: 1rem;
  overflow: hidden;
}

.section-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 1rem 1.5rem;
  background: #1a1a1a;
  cursor: pointer;
  user-select: none;
  transition: background-color 0.2s ease;
}

.section-header:hover {
  background: #222;
}

.section-header.has-errors {
  background: #3a2222;
  border-left: 4px solid #a33;
}

.section-header.has-errors:hover {
  background: #4a2a2a;
}

.section-header h2 {
  margin: 0;
  font-size: 1.1rem;
  font-weight: 600;
  color: #f1f1f1;
}

.section-arrow {
  font-size: 0.8rem;
  color: #888;
  transition: transform 0.3s ease;
}

.collapsible-section.collapsed .section-arrow {
  transform: rotate(-90deg);
}

.section-content {
  padding: 1.5rem 2rem;
  background: #121212;
  display: flex;
  flex-direction: column;
  gap: 1.4rem;
  max-height: 1000px;
  opacity: 1;
  transition: all 0.3s ease;
}

.collapsible-section.collapsed .section-content {
  max-height: 0;
  opacity: 0;
  padding-top: 0;
  padding-bottom: 0;
  overflow: hidden;
}

/* Legacy section styling for non-collapsible sections */
section:not(.collapsible-section) {
  display: flex;
  flex-direction: column;
  gap: 1.4rem;
  padding-bottom: 1.6rem;
  border-bottom: 1px solid #2a2a2a;
}

section:not(.collapsible-section):last-of-type {
  border-bottom: none;
  padding-bottom: 0;
}

header {
  padding: 1.4rem 0;
  text-align: center;
  border-bottom: 1px solid #2a2a2a;
  font-weight: 600;
  font-size: 1.7rem;
}

main {
  padding: 1.8rem 2rem 3rem 2rem;
  max-width: 420px;
  margin: 0 auto;
  display: flex;
  flex-direction: column;
  gap: 1.6rem;
}

.changes-indicator {
  background: #3a2a1a;
  border: 1px solid #ff6b35;
  border-radius: 6px;
  padding: 0.8rem 1rem;
  text-align: center;
  font-weight: 500;
  color: #ff6b35;
  font-size: 0.9rem;
  transition: opacity 0.3s ease;
}

.changes-indicator.hidden {
  opacity: 0;
  pointer-events: none;
}

.section-controls {
  display: flex;
  gap: 0.5rem;
  justify-content: center;
  margin-bottom: 1rem;
}

.btn.small {
  padding: 0.3rem 0.8rem;
  font-size: 0.85rem;
}

form {
  display: flex;
  flex-direction: column;
  gap: 1.4rem;
}

h2 {
  font-size: 1.2rem;
  margin: 1rem 0 0.5rem 0;
}

label {
  display: block;
  margin-bottom: 0.3rem;
  font-weight: 500;
  font-size: 1rem;
  color: #b3b3b3;
  text-transform: none;
}

input[type="range"] {
  appearance: none;
  -webkit-appearance: none;
  width: 100%;
  height: 8px;
  border-radius: 6px;
  background: #2a2a2a;
  cursor: pointer;
}

input[type="range"]::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 18px;
  height: 18px;
  border-radius: 50%;
  background: #4db8ff;
  cursor: pointer;
  border: none;
  margin-top: 0px;
}

input[type="range"]::-moz-range-thumb {
  width: 18px;
  height: 18px;
  border-radius: 50%;
  background: #4db8ff;
  cursor: pointer;
  border: none;
}

input[type="number"] {
  width: 100%;
  padding: 0.5rem 0.8rem;
  background: #2a2a2a;
  border: none;
  border-radius: 6px;
  color: #f1f1f1;
  font-size: 1rem;
  font-weight: 500;
  cursor: text;
  box-sizing: border-box;
}

input[type="number"]:focus {
  outline: 2px solid #4db8ff;
}


input[type="text"],
input[type="password"] {
  width: 100%;
  padding: 0.5rem 0.7rem;
  background: #2a2a2a;
  border: none;
  border-radius: 6px;
  color: #f1f1f1;
  font-size: 1rem;
  font-weight: 500;
  cursor: text;
}

input[type="text"]:focus,
input[type="password"]:focus {
  outline: 2px solid #4db8ff;
}

.toggle-password {
  position: absolute;
  right: 0.75rem;
  /* Or even 0.5rem depending how tight you want */
  top: 50%;
  transform: translateY(-50%);
  color: #888;
  cursor: pointer;
  font-size: 0.9rem;
  user-select: none;
  padding: 0 0.25rem;
  /* optional, keeps it from hugging edge too hard */
}


.settings-link {
  margin-top: 2rem;
  text-align: center;
  display: flex;
  flex-direction: column;
  gap: 1rem;
}

.button-row {
  display: flex;
  gap: 1rem;
  justify-content: center;
}

.settings-link .btn {
  background: transparent;
  border: 1.5px solid #4db8ff;
  border-radius: 6px;
  padding: 0.5rem 1.2rem;
  color: #4db8ff;
  text-decoration: none;
  font-weight: 600;
  cursor: pointer;
  transition: background 0.3s;
}

.settings-link .btn:hover {
  background: #4db8ff;
  color: #121212;
}

.settings-link .btn.secondary {
  background: transparent;
  border: 1.5px solid #666;
  color: #666;
  margin-left: 1rem;
}

.settings-link .btn.secondary:hover {
  background: #666;
  color: #121212;
}

.settings-link .btn.warning {
  background: transparent;
  border: 1.5px solid #ff6b35;
  color: #ff6b35;
  margin-left: 1rem;
}

.settings-link .btn.warning:hover {
  background: #ff6b35;
  color: #121212;
}

.btn {
  display: inline-block;
  font-weight: 600;
  font-size: 1rem;
  text-transform: none;
}

.notification {
  position: fixed;
  bottom: 20px;
  left: 20px;
  background: #222;
  color: #eee;
  font-weight: 500;
  padding: 12px 20px;
  border-radius: 6px;
  box-shadow: 0 3px 8px rgba(0, 0, 0, 0.4);
  opacity: 0;
  pointer-events: none;
  transition: opacity 0.4s ease;
  font-size: 14px;
  max-width: 400px;
  max-height: 300px;
  overflow-y: auto;
  z-index: 10000;
  user-select: none;
  box-sizing: border-box;
  line-height: 1.4;
}

.notification.show {
  opacity: 1;
  pointer-events: auto;
}

#toggles {
  --button-width: 3.5em;
  --button-height: 2em;
  --toggle-diameter: 1.5em;
  --button-toggle-offset: calc((var(--button-height) - var(--toggle-diameter)) / 2);
  --toggle-shadow-offset: 10px;
  --toggle-wider: 3em;
  --color-grey: #2a2a2a;
  --color-green: #4296f4;
  margin-top: 1rem;
}

.switch {
  display: flex;
  justify-content: space-between;
  align-items: center;
  cursor: pointer;
  user-select: none;
  padding: 0 0.5em;
  margin-bottom: 1rem;
}

#toggles input[type="checkbox"] {
  display: none;
}

#toggles input[type="checkbox"]+.slider {
  display: inline-block;
  width: var(--button-width);
  height: var(--button-height);
  background-color: var(--color-grey);
  border-radius: calc(var(--button-height) / 2);
  position: relative;
  transition: background-color 0.3s ease-in-out;
}

#toggles input[type="checkbox"]+.slider::after {
  content: "";
  display: inline-block;
  width: var(--toggle-diameter);
  height: var(--toggle-diameter);
  background-color: #fff;
  border-radius: 50%;
  position: absolute;
  top: 50%;
  left: var(--button-toggle-offset);
  transform: translateY(-50%) translateX(0);
  box-shadow: var(--toggle-shadow-offset) 0 calc(var(--toggle-shadow-offset) * 4) rgba(0, 0, 0, 0.1);
  transition: transform 0.3s ease, background-color 0.3s ease;
}

#toggles input[type="checkbox"]:checked+.slider {
  background-color: var(--color-green);
}

#toggles input[type="checkbox"]:checked+.slider::after {
  left: auto;
  right: var(--button-toggle-offset);
  transform: translateY(-50%) translateX(0);
  box-shadow: calc(var(--toggle-shadow-offset) * -1) 0 calc(var(--toggle-shadow-offset) * 4) rgba(0, 0, 0, 0.1);
}

#toggles input[type="checkbox"]:active+.slider::after,
#toggles input[type="checkbox"]:checked:active+.slider::after {
  transform: translateY(-50%) translateX(0) scaleX(1.1);
}

@media (max-width: 480px) {
  .switch {
    padding: 0 0.3em;
    font-size: 0.9rem;
  }
}

.input-suffix-wrapper {
  position: relative;
  display: inline-block;
  width: 100%;
}

.input-with-suffix {
  padding-right: 2.5em;
  /* Slightly reduced padding */
  width: 100%;
}

.input-suffix {
  position: absolute;
  right: 1.2em;
  /* Nudged left a bit */
  top: 50%;
  transform: translateY(-50%);
  pointer-events: none;
  color: #b3b3b3;
  font-size: 0.75em;
  /* Smaller size */
  letter-spacing: 0.5px;
}

/* Handle multiple suffixes (like password field with eye icon) */
.input-suffix-wrapper .input-suffix:last-of-type {
  right: 2.5em;
  /* Move further left to make room for eye icon */
}




#colour-section label.toggle-label {
  display: flex;
  justify-content: space-between;
  align-items: center;
  cursor: pointer;
  margin-bottom: 1rem;
  color: #b3b3b3;
  font-weight: 500;
}

/* Make the colour picker a tidy square */
#colour-section input[type="color"] {
  width: 2.4rem;
  height: 2.4rem;
  border: none;
  padding: 0;
  margin-left: 0.5rem;
  cursor: pointer;
  border-radius: 4px;
  background: none;
  transition: transform 0.2s ease;
}

#colour-section input[type="color"]:hover {
  transform: scale(1.1);
}

/* Webkit fix */
#colour-section input[type="color"]::-webkit-color-swatch-wrapper {
  padding: 0;
}

#colour-section input[type="color"]::-webkit-color-swatch {
  border-radius: 50%;
  border: none;
}

/* GPIO States styling */
#gpio-states span {
  font-weight: 600;
  padding: 0.2rem 0.5rem;
  border-radius: 4px;
  font-size: 0.9rem;
  background: #666;
  color: #fff;
}

.gpio-on {
  background: #4a3 !important;
}

.gpio-off {
  background: #a33 !important;
}

/* Invalid field highlighting */
.invalid-field {
  border: 2px solid #a33 !important;
  background: #3a2222 !important;
  animation: shake 0.5s ease-in-out;
}

@keyframes shake {

  0%,
  100% {
    transform: translateX(0);
  }

  25% {
    transform: translateX(-5px);
  }

  75% {
    transform: translateX(5px);
  }
}

/* Custom Popup Styles */
.popupBox {
  position: fixed;
  top: 0;
  left: 0;
  width: 100vw;
  height: 100vh;
  background: rgba(0, 0, 0, 0.5);
  backdrop-filter: blur(3px);
  -webkit-backdrop-filter: blur(3px);
  display: flex;
  justify-content: center;
  align-items: center;
  z-index: 20000;
  opacity: 0;
  pointer-events: none;
  transition: opacity 0.3s ease;
}

.popupBox.visible {
  opacity: 1;
  pointer-events: auto;
}

.popupContent {
  background: #2a2a2a;
  padding: 2rem;
  border-radius: 8px;
  max-width: 350px;
  text-align: center;
  color: #f1f1f1;
  box-shadow: 0 0 15px #121212;
}

.popupContent h2 {
  margin: 0 0 1rem 0;
  font-size: 1.3rem;
  color: #f1f1f1;
}

.popupContent p {
  margin: 0 0 1.5rem 0;
  line-height: 1.5;
  color: #b3b3b3;
}

.popup-buttons {
  display: flex;
  gap: 0.8rem;
  justify-content: center;
}

.popup-buttons .btn {
  padding: 0.6rem 1.2rem;
  font-size: 0.9rem;
  min-width: 80px;
  background: transparent;
  border: 1.5px solid;
  border-radius: 6px;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.3s ease;
  text-decoration: none;
  display: inline-block;
}

.popup-buttons .btn.secondary {
  border-color: #888;
  color: #f1f1f1;
}

.popup-buttons .btn.secondary:hover {
  background: #888;
  color: #121212;
}

.popup-buttons .btn.secondary:active {
  transform: scale(0.95);
}

.popup-buttons .btn.warning {
  border-color: #ff6b35;
  color: #ff6b35;
}

.popup-buttons .btn.warning:hover {
  background: #ff6b35;
  color: #121212;
}

.popup-buttons .btn.warning:active {
  transform: scale(0.95);
}


</style>
</head>
<body>

<header>Settings</header>

<div id="loader" class="loader">
  <div class="spinner">
    <div class="spinnerin"></div>
  </div>
</div>

  <main>
    <div id="changes-indicator" class="changes-indicator hidden">
      <span>⚠️ Unsaved changes</span>
    </div>
    <div class="section-controls">
      <button type="button" class="btn small" onclick="expandAllSections()">Expand All</button>
      <button type="button" class="btn small secondary" onclick="collapseAllSections()">Collapse All</button>
    </div>
    <form id="settings-form" autocomplete="off" onsubmit="return false;">
      <section id="brightness-settings" class="collapsible-section collapsed">
        <div class="section-header" onclick="toggleSection('brightness-settings')">
          <h2>Brightness</h2>
          <span class="section-arrow">▼</span>
        </div>
        <div class="section-content">
          <label for="full-brake-brightness">Full Brake Brightness: <span
              id="full-brake-brightness-value">—</span></label>
          <input type="range" id="full-brake-brightness" name="fullBrakeBrightness" min="0" max="255" value="0" step="1"
            data-key="fullBrakeBrightness" />

          <label for="idle-brake-brightness">Idle Brake Brightness: <span
              id="idle-brake-brightness-value">—</span></label>
          <input type="range" id="idle-brake-brightness" name="idleBrakeBrightness" min="0" max="255" value="0" step="1"
            data-key="idleBrakeBrightness" />

          <label for="indicator-backround-dim">Indicator Backround Dim Level: <span
              id="indicator-backround-dim-value">—</span></label>
          <input type="range" id="indicator-backround-dim" name="indicatorBackroundDim" min="0" max="255" value="0"
            step="1" data-key="indicatorBackroundDim" />
        </div>
      </section>

      <section id="colour-section" class="collapsible-section collapsed">
        <div class="section-header" onclick="toggleSection('colour-section')">
          <h2>Default Colours</h2>
          <span class="section-arrow">▼</span>
        </div>
        <div class="section-content">
          <label for="boot-colour" class="toggle-label">
            Boot Colour
            <input type="color" id="boot-colour" name="bootColour" value="#000000" />
          </label>

          <label for="indicator-colour" class="toggle-label">
            Indicator Colour
            <input type="color" id="indicator-colour" name="indicatorColour" value="#000000" />
          </label>
        </div>
      </section>

      <section id="transitions-section" class="collapsible-section collapsed">
        <div class="section-header" onclick="toggleSection('transitions-section')">
          <h2>Transitions</h2>
          <span class="section-arrow">▼</span>
        </div>
        <div class="section-content">
          <label for="indicator-transition">Indicator Backround Transition</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="indicator-transition" class="input-with-suffix" name="indicatorTransition" min="0"
              max="4000" value="0" data-key="indicatorTransition" />
            <span class="input-suffix">MS</span>
          </div>

          <label for="colour-transition">Colour Transition (NULL UNUSED)</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="colour-transition" class="input-with-suffix" name="colourTransition" min="0"
              max="4000" value="0" data-key="colourTransition" />
            <span class="input-suffix">MS</span>
          </div>
        </div>
      </section>

      <section id="timing-settings" class="collapsible-section collapsed">
        <div class="section-header" onclick="toggleSection('timing-settings')">
          <h2>Timing</h2>
          <span class="section-arrow">▼</span>
        </div>
        <div class="section-content">
          <label for="indicator-flash-time">Indicator Flash Time</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="indicator-flash-time" class="input-with-suffix" min="0" max="1000" value="0"
              data-key="indicatorFlashTime" />
            <span class="input-suffix">MS</span>
          </div>

          <label for="trail-flash-interval">Trail Update Interval</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="trail-flash-interval" class="input-with-suffix" min="0" max="1000" value="0"
              data-key="trailFlashInterval" />
            <span class="input-suffix">MS</span>
          </div>

          <label for="boot-duration">Boot Duration</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="boot-duration" class="input-with-suffix" min="0" max="10000" value="0"
              data-key="bootDuration" />
            <span class="input-suffix">MS</span>
          </div>
        </div>
      </section>

      <section id="led-count" class="collapsible-section collapsed">
        <div class="section-header" onclick="toggleSection('led-count')">
          <h2>LED Count</h2>
          <span class="section-arrow">▼</span>
        </div>
        <div class="section-content">
          <label for="total-led-count">Total LED Count</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="total-led-count" class="input-with-suffix" name="totalLedCount" min="1" max="1000"
              value="0" data-key="totalLedCount" />
            <span class="input-suffix">LEDs</span>
          </div>

          <label for="brake-led-start">Brake Segment Start</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="brake-led-start" class="input-with-suffix" name="brakeLedStart" min="1" max="1000"
              value="0" data-key="brakeLedStart" />
            <span class="input-suffix">Start LED</span>
          </div>

          <label for="brake-led-count">Brake Segment End</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="brake-led-end" class="input-with-suffix" name="brakeLedEnd" min="1" max="1000"
              value="0" data-key="brakeLedEnd" />
            <span class="input-suffix">End LED</span>
          </div>

          <label for="total-led-count">Indicator Trail Length</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="indicator-trail-length" class="input-with-suffix" name="indicatorTrailLength"
              min="1" max="1000" value="0" data-key="indicatorTrailLength" />
            <span class="input-suffix">LEDs</span>
          </div>
        </div>
      </section>

      <section id="toggles" class="collapsible-section collapsed">
        <div class="section-header" onclick="toggleSection('toggles')">
          <h2>Toggles</h2>
          <span class="section-arrow">▼</span>
        </div>
        <div class="section-content">
          <label for="default-light-mode" class="switch">
            Default light Mode
            <input type="checkbox" id="default-light-mode" name="defaultLightMode" />
            <span class="slider"></span>
          </label>

          <label for="boot-effect-toggle" class="switch">
            Boot Effect
            <input type="checkbox" id="boot-effect-toggle" name="bootEffectToggle" />
            <span class="slider"></span>
          </label>

          <label for="indicator-effect-toggle" class="switch">
            Indicator Trail Effect
            <input type="checkbox" id="indicator-effect-toggle" name="indicatorEffectToggle" />
            <span class="slider"></span>
          </label>

          <label for="left-trail-reverse" class="switch">
            Left Trail Reverse
            <input type="checkbox" id="left-trail-reverse" name="leftTrailReverse" />
            <span class="slider"></span>
          </label>

          <label for="right-trail-reverse" class="switch">
            Right Trail Reverse
            <input type="checkbox" id="right-trail-reverse" name="rightTrailReverse" />
            <span class="slider"></span>
          </label>

          <label for="xlights-toggle" class="switch">
            X-Lights (DDP)
            <input type="checkbox" id="xlights-toggle" name="xlightsToggle" />
            <span class="slider"></span>
          </label>
        </div>
      </section>


      <section id="gpio-pins" class="collapsible-section collapsed">
        <div class="section-header" onclick="toggleSection('gpio-pins')">
          <h2>GPIO</h2>
          <span class="section-arrow">▼</span>
        </div>
        <div class="section-content">
          <label for="led-data-gpio">LED Data GPIO</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="led-data-gpio" class="input-with-suffix" name="ledDataGpio" min="0" max="39"
              value="0" data-key="ledDataGpio" />
            <span class="input-suffix">Pin</span>
          </div>

          <label for="brake-signal-gpio">Brake Signal GPIO</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="brake-signal-gpio" class="input-with-suffix" name="brakeSignalGpio" min="0"
              max="39" value="0" data-key="brakeSignalGpio" />
            <span class="input-suffix">Pin</span>
          </div>

          <label for="light-signal-gpio">Light Signal GPIO</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="light-signal-gpio" class="input-with-suffix" name="lightSignalGpio" min="0"
              max="39" value="0" data-key="lightSignalGpio" />
            <span class="input-suffix">Pin</span>
          </div>

          <label for="left-signal-gpio">Left Indicator Signal GPIO</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="left-signal-gpio" class="input-with-suffix" name="leftSignalGpio" min="0" max="39"
              value="0" data-key="leftSignalGpio" />
            <span class="input-suffix">Pin</span>
          </div>

          <label for="right-signal-gpio">Right Indicator Signal GPIO</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="right-signal-gpio" class="input-with-suffix" name="rightSignalGpio" min="0"
              max="39" value="0" data-key="rightSignalGpio" />
            <span class="input-suffix">Pin</span>
          </div>
        </div>
      </section>

      <section id="network-auth" class="collapsible-section collapsed">
        <div class="section-header" onclick="toggleSection('network-auth')">
          <h2>Network/Auth</h2>
          <span class="section-arrow">▼</span>
        </div>
        <div class="section-content">
          <label for="captive-ssid">Captive SSID</label>
          <div class="input-suffix-wrapper">
            <input type="text" id="captive-ssid" name="captiveSsid" data-key="captiveSsid" maxlength="32"
              autocomplete="off" placeholder="—" />
            <span class="input-suffix">Required</span>
          </div>

          <label for="captive-password">Captive Password</label>
          <div class="input-suffix-wrapper">
            <input type="password" id="captive-password" name="captivePassword" data-key="captivePassword"
              maxlength="64" placeholder="—" />
            <span class="toggle-password" onclick="togglePassword()">👁</span>
            <span class="input-suffix">8+ chars if set</span>
          </div>
        </div>
      </section>

      <section id="gpio-states" class="collapsible-section collapsed">
        <div class="section-header" onclick="toggleSection('gpio-states')">
          <h2>GPIO States</h2>
          <span class="section-arrow">▼</span>
        </div>
        <div class="section-content">
          <label>Light Signal:</label> <span id="light-state">—</span><br />
          <label>Brake Signal:</label> <span id="brake-state">—</span><br />
          <label>Left Indicator Signal:</label> <span id="left-state">—</span><br />
          <label>Right Indicator Signal:</label> <span id="right-state">—</span>
        </div>
      </section>
      <div id="notification" class="notification"></div>

      <!-- Custom Popup for Confirmations -->
      <div id="popupBox" class="popupBox" aria-hidden="true" role="dialog" aria-modal="true"
        aria-labelledby="popupTitle" aria-describedby="popupDesc">
        <div class="popupContent">
          <h2 id="popupTitle"></h2>
          <p id="popupDesc"></p>
          <div class="popup-buttons">
            <button id="popupCancelBtn" class="btn secondary">Cancel</button>
            <button id="popupConfirmBtn" class="btn warning">Confirm</button>
          </div>
        </div>
      </div>

      <div class="settings-link">
        <div class="button-row">
          <button type="button" class="btn" onclick="saveAllSettings()">Save Changes</button>
          <button type="button" class="btn warning" onclick="resetToDefaults()">Reset to Defaults</button>
        </div>
        <div class="button-row">
          <a href="/" class="btn secondary">Back to Main</a>
        </div>
      </div>
    </form>
  </main>

<script>

document.addEventListener('DOMContentLoaded', () => {
  // Fetch current settings on load
  fetch('/getSettings')
    .then(res => {
      if (!res.ok) throw new Error(res.status);
      return res.json();
    })
    .then(data => {
      console.log('Loaded settings:', data);
      
      // Store original settings as defaults
      originalSettings = { ...data };
      
      // Apply settings to UI elements
      applySettingsToUI(data);
    })
    .catch(err => {
      const code = err.message;
      console.error(`Failed to fetch initial settings: ${code}`);
      showNotification(`Failed to fetch settings: ${code}`, true);
    });

  // Save button functionality is handled by onclick in HTML

  // Add real-time updates for range inputs
  const rangeInputs = document.querySelectorAll('input[type="range"]');
  rangeInputs.forEach(input => {
    const valueDisplay = document.getElementById(input.id + '-value');
    if (valueDisplay) {
      input.addEventListener('input', () => {
        valueDisplay.textContent = input.value;
        markAsChanged();
      });
    }
  });

  // Add real-time updates for number inputs
  const numberInputs = document.querySelectorAll('input[type="number"]');
  numberInputs.forEach(input => {
    input.addEventListener('input', () => {
      // Validate min/max values
      const min = parseInt(input.min);
      const max = parseInt(input.max);
      const value = parseInt(input.value);
      
      if (value < min) input.value = min;
      if (value > max) input.value = max;
      
      // Clear invalid field highlighting when user starts typing
      input.classList.remove('invalid-field');
      markAsChanged();
    });
  });

  // Clear invalid field highlighting for text inputs
  const textInputs = document.querySelectorAll('input[type="text"], input[type="password"]');
  textInputs.forEach(input => {
    input.addEventListener('input', () => {
      input.classList.remove('invalid-field');
      markAsChanged();
    });
  });

  // Clear invalid field highlighting for color inputs
  const colorInputs = document.querySelectorAll('input[type="color"]');
  colorInputs.forEach(input => {
    input.addEventListener('change', () => {
      input.classList.remove('invalid-field');
      markAsChanged();
    });
  });

  // Add change tracking for checkboxes
  const checkboxInputs = document.querySelectorAll('input[type="checkbox"]');
  checkboxInputs.forEach(input => {
    input.addEventListener('change', () => {
      markAsChanged();
    });
  });

  // Start GPIO state monitoring
  updateGpioStates();
  setInterval(updateGpioStates, 1000); // Update every second
});

function applySettingsToUI(settings) {
  // Brightness settings
  if (settings.fullBrakeBrightness !== undefined) {
    const input = document.getElementById('full-brake-brightness');
    if (input) {
      input.value = settings.fullBrakeBrightness;
      const valueDisplay = document.getElementById('full-brake-brightness-value');
      if (valueDisplay) valueDisplay.textContent = settings.fullBrakeBrightness;
    }
  }

  if (settings.idleBrakeBrightness !== undefined) {
    const input = document.getElementById('idle-brake-brightness');
    if (input) input.value = settings.idleBrakeBrightness;
  }

  if (settings.indicatorBackroundDim !== undefined) {
    const input = document.getElementById('indicator-backround-dim');
    if (input) input.value = settings.indicatorBackroundDim;
  }

  // Color settings
  if (settings.bootColour) {
    const input = document.getElementById('boot-colour');
    if (input) input.value = settings.bootColour;
  }

  if (settings.indicatorColour) {
    const input = document.getElementById('indicator-colour');
    if (input) input.value = settings.indicatorColour;
  }

  // Transition settings
  if (settings.indicatorTransition !== undefined) {
    const input = document.getElementById('indicator-transition');
    if (input) input.value = settings.indicatorTransition;
  }

  if (settings.colourTransition !== undefined) {
    const input = document.getElementById('colour-transition');
    if (input) input.value = settings.colourTransition;
  }

  // Timing settings
  if (settings.indicatorFlashTime !== undefined) {
    const input = document.getElementById('indicator-flash-time');
    if (input) input.value = settings.indicatorFlashTime;
  }

  if (settings.trailFlashInterval !== undefined) {
    const input = document.getElementById('trail-flash-interval');
    if (input) input.value = settings.trailFlashInterval;
  }

  if (settings.bootDuration !== undefined) {
    const input = document.getElementById('boot-duration');
    if (input) input.value = settings.bootDuration;
  }

  // LED count settings
  if (settings.totalLedCount !== undefined) {
    const input = document.getElementById('total-led-count');
    if (input) input.value = settings.totalLedCount;
  }

  if (settings.brakeLedStart !== undefined) {
    const input = document.getElementById('brake-led-start');
    if (input) input.value = settings.brakeLedStart;
  }

  if (settings.brakeLedEnd !== undefined) {
    const input = document.getElementById('brake-led-end');
    if (input) input.value = settings.brakeLedEnd;
  }

  if (settings.indicatorTrailLength !== undefined) {
    const input = document.getElementById('indicator-trail-length');
    if (input) input.value = settings.indicatorTrailLength;
  }

  // Toggle settings
  if (settings.defaultLightMode !== undefined) {
    const input = document.getElementById('default-light-mode');
    if (input) input.checked = settings.defaultLightMode;
  }

  if (settings.bootEffectToggle !== undefined) {
    const input = document.getElementById('boot-effect-toggle');
    if (input) input.checked = settings.bootEffectToggle;
  }

  if (settings.indicatorEffectToggle !== undefined) {
    const input = document.getElementById('indicator-effect-toggle');
    if (input) input.checked = settings.indicatorEffectToggle;
  }

  if (settings.leftTrailReverse !== undefined) {
    const input = document.getElementById('left-trail-reverse');
    if (input) input.checked = settings.leftTrailReverse;
  }

  if (settings.rightTrailReverse !== undefined) {
    const input = document.getElementById('right-trail-reverse');
    if (input) input.checked = settings.rightTrailReverse;
  }

  if (settings.xlightsToggle !== undefined) {
    const input = document.getElementById('xlights-toggle');
    if (input) input.checked = settings.xlightsToggle;
  }

  // GPIO settings
  if (settings.ledDataGpio !== undefined) {
    const input = document.getElementById('led-data-gpio');
    if (input) input.value = settings.ledDataGpio;
  }

  if (settings.brakeSignalGpio !== undefined) {
    const input = document.getElementById('brake-signal-gpio');
    if (input) input.value = settings.brakeSignalGpio;
  }

  if (settings.lightSignalGpio !== undefined) {
    const input = document.getElementById('light-signal-gpio');
    if (input) input.value = settings.lightSignalGpio;
  }

  if (settings.leftSignalGpio !== undefined) {
    const input = document.getElementById('left-signal-gpio');
    if (input) input.value = settings.leftSignalGpio;
  }

  if (settings.rightSignalGpio !== undefined) {
    const input = document.getElementById('right-signal-gpio');
    if (input) input.value = settings.rightSignalGpio;
  }

  // Network settings
  if (settings.captiveSsid) {
    const input = document.getElementById('captive-ssid');
    if (input) input.value = settings.captiveSsid;
  }

  if (settings.captivePassword) {
    const input = document.getElementById('captive-password');
    if (input) input.value = settings.captivePassword;
  }
}

function validateSettings(settings) {
  const errors = [];

  // Validate SSID
  if (!settings.captiveSsid || settings.captiveSsid.trim() === '') {
    errors.push('SSID is required');
  } else if (!/^[a-zA-Z0-9_-]+$/.test(settings.captiveSsid)) {
    errors.push('SSID can only contain letters, numbers, underscores, and hyphens');
  } else if (settings.captiveSsid.length > 32) {
    errors.push('SSID must be 32 characters or less');
  }

  // Validate Password (optional)
  if (settings.captivePassword && settings.captivePassword.trim() !== '') {
    if (settings.captivePassword.length < 8) {
      errors.push('Password must be at least 8 characters (or leave empty for no password)');
    } else if (settings.captivePassword.length > 64) {
      errors.push('Password must be 64 characters or less');
    } else if (!/^[a-zA-Z0-9!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?]+$/.test(settings.captivePassword)) {
      errors.push('Password contains invalid characters');
    }
  }

  // Validate LED counts
  if (settings.totalLedCount < 1 || settings.totalLedCount > 1000) {
    errors.push('Total LED count must be between 1 and 1000');
  }

  if (settings.brakeLedStart < 0 || settings.brakeLedStart >= settings.totalLedCount) {
    errors.push('Brake LED start must be between 0 and total LED count');
  }

  if (settings.brakeLedEnd < settings.brakeLedStart || settings.brakeLedEnd >= settings.totalLedCount) {
    errors.push('Brake LED end must be after start and less than total LED count');
  }

  if (settings.indicatorTrailLength < 1 || settings.indicatorTrailLength > 100) {
    errors.push('Indicator trail length must be between 1 and 100');
  }

  // Validate brightness values
  if (settings.fullBrakeBrightness < 0 || settings.fullBrakeBrightness > 255) {
    errors.push('Full brake brightness must be between 0 and 255');
  }

  if (settings.idleBrakeBrightness < 0 || settings.idleBrakeBrightness > 255) {
    errors.push('Idle brake brightness must be between 0 and 255');
  }

  if (settings.indicatorBackroundDim < 0 || settings.indicatorBackroundDim > 255) {
    errors.push('Indicator background dim must be between 0 and 255');
  }

  // Validate timing values
  if (settings.indicatorTransition < 0 || settings.indicatorTransition > 4000) {
    errors.push('Indicator transition must be between 0 and 4000ms');
  }

  if (settings.indicatorFlashTime < 0 || settings.indicatorFlashTime > 1000) {
    errors.push('Indicator flash time must be between 0 and 1000ms');
  }

  if (settings.trailFlashInterval < 0 || settings.trailFlashInterval > 1000) {
    errors.push('Trail flash interval must be between 0 and 1000ms');
  }

  if (settings.bootDuration < 0 || settings.bootDuration > 10000) {
    errors.push('Boot duration must be between 0 and 10000ms');
  }

  // Validate GPIO pins
  const gpioPins = [settings.ledDataGpio, settings.brakeSignalGpio, settings.lightSignalGpio, 
                   settings.leftSignalGpio, settings.rightSignalGpio];
  const uniquePins = new Set(gpioPins);
  
  if (uniquePins.size !== gpioPins.length) {
    errors.push('All GPIO pins must be unique');
  }

  for (let pin of gpioPins) {
    if (pin < 0 || pin > 39) {
      errors.push('GPIO pins must be between 0 and 39');
      break;
    }
  }

  // Validate colors
  const colorRegex = /^#[0-9A-Fa-f]{6}$/;
  if (!colorRegex.test(settings.bootColour)) {
    errors.push('Boot color must be a valid hex color (e.g., #FF0000)');
  }

  if (!colorRegex.test(settings.indicatorColour)) {
    errors.push('Indicator color must be a valid hex color (e.g., #FF0000)');
  }

  return errors;
}

function highlightInvalidFields(errors) {
  // Clear previous highlights
  document.querySelectorAll('.invalid-field').forEach(el => {
    el.classList.remove('invalid-field');
  });
  document.querySelectorAll('.section-header').forEach(el => {
    el.classList.remove('has-errors');
  });

  // Map error messages to field IDs and their sections
  const errorFieldMap = {
    'SSID is required': { field: 'captive-ssid', section: 'network-auth' },
    'SSID can only contain letters, numbers, underscores, and hyphens': { field: 'captive-ssid', section: 'network-auth' },
    'SSID must be 32 characters or less': { field: 'captive-ssid', section: 'network-auth' },
    'Password is required': { field: 'captive-password', section: 'network-auth' },
    'Password must be at least 8 characters': { field: 'captive-password', section: 'network-auth' },
    'Password must be 64 characters or less': { field: 'captive-password', section: 'network-auth' },
    'Password contains invalid characters': { field: 'captive-password', section: 'network-auth' },
    'Total LED count must be between 1 and 1000': { field: 'total-led-count', section: 'led-count' },
    'Brake LED start must be between 0 and total LED count': { field: 'brake-led-start', section: 'led-count' },
    'Brake LED end must be after start and less than total LED count': { field: 'brake-led-end', section: 'led-count' },
    'Indicator trail length must be between 1 and 100': { field: 'indicator-trail-length', section: 'led-count' },
    'Full brake brightness must be between 0 and 255': { field: 'full-brake-brightness', section: 'brightness-settings' },
    'Idle brake brightness must be between 0 and 255': { field: 'idle-brake-brightness', section: 'brightness-settings' },
    'Indicator background dim must be between 0 and 255': { field: 'indicator-backround-dim', section: 'brightness-settings' },
    'Indicator transition must be between 0 and 4000ms': { field: 'indicator-transition', section: 'transitions-section' },
    'Indicator flash time must be between 0 and 1000ms': { field: 'indicator-flash-time', section: 'timing-settings' },
    'Trail flash interval must be between 0 and 1000ms': { field: 'trail-flash-interval', section: 'timing-settings' },
    'Boot duration must be between 0 and 10000ms': { field: 'boot-duration', section: 'timing-settings' },
    'All GPIO pins must be unique': { field: 'led-data-gpio', section: 'gpio-pins' },
    'GPIO pins must be between 0 and 39': { field: 'led-data-gpio', section: 'gpio-pins' },
    'Boot color must be a valid hex color (e.g., #FF0000)': { field: 'boot-colour', section: 'colour-section' },
    'Indicator color must be a valid hex color (e.g., #FF0000)': { field: 'indicator-colour', section: 'colour-section' }
  };

  // Track which sections have errors
  const sectionsWithErrors = new Set();

  // Highlight fields with errors and track sections
  errors.forEach(error => {
    const errorInfo = errorFieldMap[error];
    if (errorInfo) {
      const field = document.getElementById(errorInfo.field);
      if (field) {
        field.classList.add('invalid-field');
        sectionsWithErrors.add(errorInfo.section);
      }
    }
  });

  // Highlight section headers that contain errors
  sectionsWithErrors.forEach(sectionId => {
    const section = document.getElementById(sectionId);
    if (section) {
      const header = section.querySelector('.section-header');
      if (header) {
        header.classList.add('has-errors');
        // Auto-expand sections with errors
        section.classList.remove('collapsed');
      }
    }
  });

  // Scroll to first error field
  const firstError = errors[0];
  if (firstError && errorFieldMap[firstError]) {
    const field = document.getElementById(errorFieldMap[firstError].field);
    if (field) {
      field.scrollIntoView({ behavior: 'smooth', block: 'center' });
    }
  }
}

function saveAllSettings() {
  // Collect all form data
  const settings = {
    fullBrakeBrightness: parseInt(document.getElementById('full-brake-brightness').value),
    idleBrakeBrightness: parseInt(document.getElementById('idle-brake-brightness').value),
    indicatorBackroundDim: parseInt(document.getElementById('indicator-backround-dim').value),
    bootColour: document.getElementById('boot-colour').value,
    indicatorColour: document.getElementById('indicator-colour').value,
    indicatorTransition: parseInt(document.getElementById('indicator-transition').value),
    colourTransition: parseInt(document.getElementById('colour-transition').value),
    indicatorFlashTime: parseInt(document.getElementById('indicator-flash-time').value),
    trailFlashInterval: parseInt(document.getElementById('trail-flash-interval').value),
    bootDuration: parseInt(document.getElementById('boot-duration').value),
    totalLedCount: parseInt(document.getElementById('total-led-count').value),
    brakeLedStart: parseInt(document.getElementById('brake-led-start').value),
    brakeLedEnd: parseInt(document.getElementById('brake-led-end').value),
    indicatorTrailLength: parseInt(document.getElementById('indicator-trail-length').value),
    defaultLightMode: document.getElementById('default-light-mode').checked,
    bootEffectToggle: document.getElementById('boot-effect-toggle').checked,
    indicatorEffectToggle: document.getElementById('indicator-effect-toggle').checked,
    leftTrailReverse: document.getElementById('left-trail-reverse').checked,
    rightTrailReverse: document.getElementById('right-trail-reverse').checked,
    xlightsToggle: document.getElementById('xlights-toggle').checked,
    ledDataGpio: parseInt(document.getElementById('led-data-gpio').value),
    brakeSignalGpio: parseInt(document.getElementById('brake-signal-gpio').value),
    lightSignalGpio: parseInt(document.getElementById('light-signal-gpio').value),
    leftSignalGpio: parseInt(document.getElementById('left-signal-gpio').value),
    rightSignalGpio: parseInt(document.getElementById('right-signal-gpio').value),
    captiveSsid: document.getElementById('captive-ssid').value.trim(),
    captivePassword: document.getElementById('captive-password').value
  };

  // Validate settings before sending
  const errors = validateSettings(settings);
  if (errors.length > 0) {
    const errorMessage = 'Validation errors:\n' + errors.join('\n');
    console.error('Validation failed:', errors);
    showNotification(errorMessage, true);
    
    // Highlight problematic fields
    highlightInvalidFields(errors);
    return;
  }

  console.log('Saving settings:', settings);

  // Send settings to ESP
  fetch('/setSettings', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify(settings)
  })
      .then(res => {
        if (!res.ok) throw new Error(res.status);
    console.log('Settings saved successfully');
    showNotification('Settings saved successfully!', false);
    markAsSaved();
      })
      .catch(err => {
        const code = err.message;
    console.error(`Settings save failed: ${code}`);
    showNotification(`Settings save failed: ${code}`, true);
  });
}

// GPIO state monitoring
function updateGpioStates() {
  fetch('/getGpioStates')
    .then(res => {
      if (!res.ok) throw new Error(res.status);
      return res.json();
    })
    .then(data => {
      const lightState = document.getElementById('light-state');
      const brakeState = document.getElementById('brake-state');
      const leftState = document.getElementById('left-state');
      const rightState = document.getElementById('right-state');
      
      if (lightState) {
        lightState.textContent = data.lightState ? 'ON' : 'OFF';
        lightState.className = data.lightState ? 'gpio-on' : 'gpio-off';
      }
      if (brakeState) {
        brakeState.textContent = data.brakeState ? 'ON' : 'OFF';
        brakeState.className = data.brakeState ? 'gpio-on' : 'gpio-off';
      }
      if (leftState) {
        leftState.textContent = data.leftState ? 'ON' : 'OFF';
        leftState.className = data.leftState ? 'gpio-on' : 'gpio-off';
      }
      if (rightState) {
        rightState.textContent = data.rightState ? 'ON' : 'OFF';
        rightState.className = data.rightState ? 'gpio-on' : 'gpio-off';
      }
    })
    .catch(err => {
      console.error('Failed to fetch GPIO states:', err);
    });
}

// Section toggle functionality
function toggleSection(sectionId) {
  const section = document.getElementById(sectionId);
  if (section) {
    section.classList.toggle('collapsed');
  }
}

// Password toggle functionality
function togglePassword() {
  const passwordInput = document.getElementById('captive-password');
  if (passwordInput.type === 'password') {
    passwordInput.type = 'text';
  } else {
    passwordInput.type = 'password';
  }
}

window.addEventListener("load", () => {
  const loader = document.getElementById("loader");
  if (loader) {
  loader.style.opacity = 0;
  loader.style.transition = "opacity 0.5s ease";
  setTimeout(() => {
    loader.style.display = "none";
  }, 500);
  }
});

function showNotification(message, isError = false) {
  const notif = document.getElementById('notification');
  if (!notif) return;
  
  // Handle multi-line messages
  if (message.includes('\n')) {
    notif.innerHTML = message.replace(/\n/g, '<br>');
  } else {
  notif.textContent = message;
  }
  
  notif.style.background = isError ? '#a33' : '#3a3';
  notif.classList.add('show');

  if (notif.timeoutId) clearTimeout(notif.timeoutId);
  notif.timeoutId = setTimeout(() => {
    notif.classList.remove('show');
  }, isError ? 8000 : 3500); // Show errors longer
}

// Track changes to show indicator
let hasUnsavedChanges = false;
let originalSettings = {};

function markAsChanged() {
  if (!hasUnsavedChanges) {
    hasUnsavedChanges = true;
    const indicator = document.getElementById('changes-indicator');
    if (indicator) indicator.classList.remove('hidden');
  }
}

function markAsSaved() {
  hasUnsavedChanges = false;
  const indicator = document.getElementById('changes-indicator');
  if (indicator) indicator.classList.add('hidden');
}

function expandAllSections() {
  document.querySelectorAll('.collapsible-section').forEach(section => {
    section.classList.remove('collapsed');
  });
}

function collapseAllSections() {
  document.querySelectorAll('.collapsible-section').forEach(section => {
    section.classList.add('collapsed');
  });
}

// Popup functionality
let popupCallback = null;

function openPopup(title, desc, onConfirm) {
  const popupBox = document.getElementById('popupBox');
  const popupTitle = document.getElementById('popupTitle');
  const popupDesc = document.getElementById('popupDesc');
  
  popupTitle.textContent = title;
  popupDesc.textContent = desc;
  popupCallback = onConfirm;
  
  popupBox.classList.add('visible');
  popupBox.setAttribute('aria-hidden', 'false');
}

function closePopup() {
  const popupBox = document.getElementById('popupBox');
  popupBox.classList.remove('visible');
  popupBox.setAttribute('aria-hidden', 'true');
  popupCallback = null;
}

function confirmPopup() {
  if (popupCallback) {
    popupCallback();
  }
  closePopup();
}

// Initialize popup event listeners
document.addEventListener('DOMContentLoaded', function() {
  const popupCancelBtn = document.getElementById('popupCancelBtn');
  const popupConfirmBtn = document.getElementById('popupConfirmBtn');
  
  if (popupCancelBtn) {
    popupCancelBtn.addEventListener('click', closePopup);
  }
  
  if (popupConfirmBtn) {
    popupConfirmBtn.addEventListener('click', confirmPopup);
  }
  
  // Close popup when clicking outside
  const popupBox = document.getElementById('popupBox');
  if (popupBox) {
    popupBox.addEventListener('click', (e) => {
      if (e.target === popupBox) {
        closePopup();
      }
    });
  }
});

function resetToDefaults() {
  openPopup(
    'Reset to Defaults',
    'Are you sure you want to reset all settings to their original values? This action cannot be undone.',
    () => {
      // Reset to original loaded settings
      if (Object.keys(originalSettings).length > 0) {
        applySettingsToUI(originalSettings);
        markAsChanged();
        showNotification('Settings reset to original values. Click "Save Changes" to apply.');
      } else {
        showNotification('No original settings available. Please refresh the page.', true);
      }
    }
  );
}

</script>

</body>
</html>

)rawliteral";

#endif
