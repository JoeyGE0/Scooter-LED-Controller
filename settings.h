#ifndef SETTINGS_H
#define SETTINGS_H

const char* settingsPageHTML = R"raw_string(<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Settings - Scooter LEDs</title>
  <link rel="stylesheet" href="settings-styles.css" />
</head>

<body>
  <header>
    Settings
  </header>
  <div id="loader" class="loader"></div>

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
          <input type="range" id="full-brake-brightness" name="fullBrakeBrightness" min="0" max="255" value="0"
            step="1" />

          <label for="idle-brake-brightness">Idle Brake Brightness: <span
              id="idle-brake-brightness-value">—</span></label>
          <input type="range" id="idle-brake-brightness" name="idleBrakeBrightness" min="0" max="255" value="0"
            step="1" />

          <label for="indicator-backround-dim">Indicator Backround Dim Level: <span
              id="indicator-backround-dim-value">—</span></label>
          <input type="range" id="indicator-backround-dim" name="indicatorBackroundDim" min="0" max="255" value="0"
            step="1" />
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
              max="4000" value="0" />
            <span class="input-suffix">MS</span>
          </div>

          <label for="colour-transition">Colour Transition</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="colour-transition" class="input-with-suffix" name="colourTransition" min="0"
              max="4000" value="0" />
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
            <input type="number" id="indicator-flash-time" class="input-with-suffix" min="0" max="1000" value="0" />
            <span class="input-suffix">MS</span>
          </div>

          <label for="first-flash-duration">First Flash Duration</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="first-flash-duration" class="input-with-suffix" min="100" max="3000" value="0" />
            <span class="input-suffix">MS</span>
          </div>

          <label for="trail-flash-interval">Trail Update Interval</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="trail-flash-interval" class="input-with-suffix" min="0" max="1000" value="0" />
            <span class="input-suffix">MS</span>
          </div>

          <label for="boot-duration">Boot Duration</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="boot-duration" class="input-with-suffix" min="0" max="10000" value="0" />
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
              value="0" />
            <span class="input-suffix">LEDs</span>
          </div>

          <label for="brake-led-start">Brake Segment Start</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="brake-led-start" class="input-with-suffix" name="brakeLedStart" min="1" max="1000"
              value="0" />
            <span class="input-suffix">Start LED</span>
          </div>

          <label for="brake-led-end">Brake Segment End</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="brake-led-end" class="input-with-suffix" name="brakeLedEnd" min="1" max="1000"
              value="0" />
            <span class="input-suffix">End LED</span>
          </div>

          <label for="indicator-trail-length">Indicator Trail Length</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="indicator-trail-length" class="input-with-suffix" name="indicatorTrailLength"
              min="1" max="1000" value="0" />
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

          <label for="reverse-indicator-sides" class="switch">
            Reverse Indicator Sides
            <input type="checkbox" id="reverse-indicator-sides" name="reverseIndicatorSides" />
            <span class="slider"></span>
          </label>

          <label for="xlights-toggle" class="switch">
            X-Lights (DDP)
            <input type="checkbox" id="xlights-toggle" name="xlightsToggle" />
            <span class="slider"></span>
          </label>
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
            <input type="text" id="captive-ssid" name="captiveSsid" maxlength="32" placeholder="—" />
            <span class="input-suffix">Required</span>
          </div>

          <label for="captive-password">Captive Password</label>
          <div class="input-suffix-wrapper">
            <input type="password" id="captive-password" name="captivePassword" maxlength="64" placeholder="—" />
            <span class="toggle-password" onclick="togglePassword()">👁</span>
            <span class="input-suffix">8+</span>
          </div>
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
              value="0" />
            <span class="input-suffix">Pin</span>
          </div>

          <label for="brake-signal-gpio">Brake Signal GPIO</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="brake-signal-gpio" class="input-with-suffix" name="brakeSignalGpio" min="0"
              max="39" value="0" />
            <span class="input-suffix">Pin</span>
          </div>

          <label for="light-signal-gpio">Light Signal GPIO</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="light-signal-gpio" class="input-with-suffix" name="lightSignalGpio" min="0"
              max="39" value="0" />
            <span class="input-suffix">Pin</span>
          </div>

          <label for="left-signal-gpio">Left Indicator Signal GPIO</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="left-signal-gpio" class="input-with-suffix" name="leftSignalGpio" min="0" max="39"
              value="0" />
            <span class="input-suffix">Pin</span>
          </div>

          <label for="right-signal-gpio">Right Indicator Signal GPIO</label>
          <div class="input-suffix-wrapper">
            <input type="number" id="right-signal-gpio" class="input-with-suffix" name="rightSignalGpio" min="0"
              max="39" value="0" />
            <span class="input-suffix">Pin</span>
          </div>
        </div>
      </section>

      <section id="gpio-states" class="collapsible-section collapsed">
        <div class="section-header" onclick="toggleSection('gpio-states')">
          <h2>GPIO States</h2>
          <span class="section-arrow">▼</span>
        </div>
        <div class="section-content">
          <div class="gpio-dashboard">
            <div class="gpio-card" data-gpio="power">
              <div class="gpio-icon">
                <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" viewBox="0 0 16 16">
                  <path
                    d="M11.251.068a.5.5 0 0 1 .227.58L9.677 6.5H13a.5.5 0 0 1 .364.843l-8 8.5a.5.5 0 0 1-.842-.49L6.323 9.5H3a.5.5 0 0 1-.364-.843l8-8.5a.5.5 0 0 1 .615-.09z" />
                </svg>
              </div>
              <div class="gpio-info">
                <span class="gpio-label">Light</span>
              </div>
            </div>

            <div class="gpio-card" data-gpio="brake">
              <div class="gpio-icon">
                <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" viewBox="0 0 16 16">
                  <path fill-rule="evenodd"
                    d="M6 0a2 2 0 0 0-2 2H2c.167.5.8 1.6 2 2v2H2c.167.5.8 1.6 2 2v2H2c.167.5.8 1.6 2 2v1a2 2 0 0 0 2 2h4a2 2 0 0 0 2-2v-1c1.2-.4 1.833-1.5 2-2h-2V8c1.2-.4 1.833-1.5 2-2h-2V4c1.2-.4 1.833-1.5 2-2h-2a2 2 0 0 0-2-2zm3.5 3.5a1.5 1.5 0 1 1-3 0 1.5 1.5 0 0 1 3 0m0 4a1.5 1.5 0 1 1-3 0 1.5 1.5 0 0 1 3 0M8 13a1.5 1.5 0 1 1 0-3 1.5 1.5 0 0 1 0 3" />
                </svg>
              </div>
              <div class="gpio-info">
                <span class="gpio-label">Brake</span>
              </div>
            </div>

            <div class="gpio-card" data-gpio="left">
              <div class="gpio-icon">
                <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" viewBox="0 0 16 16">
                  <path fill-rule="evenodd"
                    d="M1.146 4.854a.5.5 0 0 1 0-.708l4-4a.5.5 0 1 1 .708.708L2.707 4H12.5A2.5 2.5 0 0 1 15 6.5v8a.5.5 0 0 1-1 0v-8A1.5 1.5 0 0 0 12.5 5H2.707l3.147 3.146a.5.5 0 1 1-.708.708z" />
                </svg>
              </div>
              <div class="gpio-info">
                <span class="gpio-label">Left</span>
              </div>
            </div>

            <div class="gpio-card" data-gpio="right">
              <div class="gpio-icon">
                <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" viewBox="0 0 16 16">
                  <path fill-rule="evenodd"
                    d="M14.854 4.854a.5.5 0 0 0 0-.708l-4-4a.5.5 0 0 0-.708.708L13.293 4H3.5A2.5 2.5 0 0 0 1 6.5v8a.5.5 0 0 0 1 0v-8A1.5 1.5 0 0 1 3.5 5h9.793l-3.147 3.146a.5.5 0 0 0 .708.708z" />
                </svg>
              </div>
              <div class="gpio-info">
                <span class="gpio-label">Right</span>
              </div>
            </div>
          </div>
        </div>
      </section>

      <section id="updates-section" class="collapsible-section collapsed">
        <div class="section-header" onclick="toggleSection('updates-section')">
          <h2>Firmware Updates</h2>
          <span class="section-arrow">▼</span>
        </div>
        <div class="section-content">
          <div class="update-info">
            <p>Upload a firmware file (.bin) to update the ESP32. The device will restart after a successful update.</p>
            <div class="file-upload-area">
              <input type="file" id="firmware-file" accept=".bin" style="display: none;" />
              <button type="button" class="btn" onclick="document.getElementById('firmware-file').click()">
                <svg class="upload-icon" xmlns="http://www.w3.org/2000/svg" fill="currentColor" viewBox="0 0 16 16">
                  <path
                    d="M.5 9.9a.5.5 0 0 1 .5.5v2.5a1 1 0 0 0 1 1h12a1 1 0 0 0 1-1v-2.5a.5.5 0 0 1 1 0v2.5a2 2 0 0 1-2 2H2a2 2 0 0 1-2-2v-2.5a.5.5 0 0 1 .5-.5z" />
                  <path
                    d="M7.646 11.854a.5.5 0 0 0 .708 0l3-3a.5.5 0 0 0-.708-.708L8.5 10.293V1.5a.5.5 0 0 0-1 0v8.793L5.354 8.146a.5.5 0 1 0-.708.708l3 3z" />
                </svg>
                Select Firmware File
              </button>
              <div id="file-info" class="file-info hidden">
                <span id="file-name"></span>
                <span id="file-size"></span>
              </div>
            </div>
            <button type="button" id="upload-btn" class="btn warning" onclick="uploadFirmware()" disabled>
              Upload & Flash Firmware
            </button>
          </div>
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
  <script src="settings-script.js"></script>
</body>

</html>)raw_string";

#endif
