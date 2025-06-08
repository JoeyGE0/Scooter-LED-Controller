#ifndef Settings_H
#define Settings_H

const char* settingsPageHTML = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<title>Settings - Scooter LEDs</title>
<style>
  /* Loader styles */
  .loader {
    position: fixed;
    top: 0; left: 0;
    width: 100vw; height: 100vh;
    background: rgba(0,0,0,0.6);
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
      0px -10px 10px 0px #9c32e2,
      10px -10px 10px #f36896,
      10px 0 10px #ff0b0b,
      10px 10px 10px 0px #ff5500,
      0 10px 10px 0px #ff9500,
      -10px 10px 10px 0px #ffb700;
    animation: rot55 0.7s linear infinite;
  }
  .spinnerin {
    border: 2px solid #444;
    width: 1.5em;
    height: 1.5em;
    border-radius: 50%;
    position: absolute;
    top: 50%; left: 50%;
    transform: translate(-50%, -50%);
  }
  @keyframes rot55 {
    to { transform: rotate(360deg); }
  }
  .loader.hidden {
    display: none;
  }

  /* Body & typography */
  body {
    margin: 0; padding: 0;
    background: #121212;
    color: #f1f1f1;
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Oxygen,
      Ubuntu, Cantarell, "Open Sans", "Helvetica Neue", sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
  }

  /* Layout */
  section {
    display: flex;
    flex-direction: column;
    gap: 1.4rem;
    padding-bottom: 1.6rem;
    border-bottom: 1px solid #2a2a2a;
  }
  section:last-of-type {
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
    padding: 0.5rem 0.7rem;
    background: #2a2a2a;
    border: none;
    border-radius: 6px;
    color: #f1f1f1;
    font-size: 1rem;
    font-weight: 500;
    cursor: text;
  }
  input[type="number"]:focus {
    outline: 2px solid #4db8ff;
  }
  .settings-link {
    margin-top: 2rem;
    text-align: center;
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
  .btn {
    display: inline-block;
    font-weight: 600;
    font-size: 1rem;
    text-transform: none;
  }

  /* Notification popup */
  .notification {
    position: fixed;
    bottom: 20px;
    left: 20px;
    background: #222;
    color: #eee;
    font-weight: 500;
    padding: 12px 20px;
    border-radius: 6px;
    box-shadow: 0 3px 8px rgba(0,0,0,0.4);
    opacity: 0;
    pointer-events: none;
    transition: opacity 0.4s ease;
    font-size: 14px;
    max-width: 300px;
    z-index: 10000;
    user-select: none;
    box-sizing: border-box;
  }
  .notification.show {
    opacity: 1;
    pointer-events: auto;
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
  <form id="settings-form" autocomplete="off" onsubmit="return false;">
    <section id="brightness-settings">
      <h2>Brightness:</h2>

      <label for="full-brake-brightness">Full Brake Brightness: <span id="full-brake-brightness-value">100</span></label>
      <input type="range" id="full-brake-brightness" name="fullBrakeBrightness" min="0" max="100" value="100" step="1" data-key="fullBrakeBrightness" />

      <label for="idle-brake-brightness">Idle Brake Brightness</label>
      <input type="range" id="idle-brake-brightness" name="idleBrakeBrightness" min="0" max="100" value="20" step="1" data-key="idleBrakeBrightness" />

      <label for="indicator-backround-dim">Indicator Backround Dim Level</label>
      <input type="range" id="indicator-backround-dim" name="indicatorBackroundDim" min="0" max="100" value="20" step="1" data-key="indicatorBackroundDim" />
    </section>

    <section id="transition-settings">
      <h2>Transitions:</h2>

      <label for="indicator-transition">Indicator Transition</label>
      <input type="number" id="indicator-transition" name="indicatorTransition" min="0" max="4000" value="100" data-key="indicatorTransition" />

      <label for="colour-transition">Indicator Stop Transition</label>
      <input type="number" id="colour-transition" name="colourTransition" min="0" max="4000" value="200" data-key="colourTransition" />
    </section>

    <section id="led-count">
      <h2>LED Count:</h2>

      <label for="total-led-count">Total LED Count</label>
      <input type="number" id="total-led-count" name="totalLedCount" min="0" max="1000" value="100" data-key="totalLedCount" />

      <label for="brake-led-count">Brake LED Count</label>
      <input type="number" id="brake-led-count" name="brakeLedCount" min="0" max="50" value="20" data-key="brakeLedCount" />
    </section>

    <section id="gpio-pins">
      <h2>GPIO:</h2>

      <label for="led-data-gpio">LED Data GPIO</label>
      <input type="number" id="led-data-gpio" name="ledDataGpio" min="0" max="39" value="1" data-key="ledDataGpio" />

      <label for="brake-signal-gpio">Brake Signal GPIO</label>
      <input type="number" id="brake-signal-gpio" name="brakeSignalGpio" min="0" max="39" value="2" data-key="brakeSignalGpio" />

      <label for="light-signal-gpio">Light Signal GPIO</label>
      <input type="number" id="light-signal-gpio" name="lightSignalGpio" min="0" max="39" value="3" data-key="lightSignalGpio" />

      <label for="left-signal-gpio">Left Indicator Signal GPIO</label>
      <input type="number" id="left-signal-gpio" name="leftSignalGpio" min="0" max="39" value="4" data-key="leftSignalGpio" />

      <label for="right-signal-gpio">Right Indicator Signal GPIO</label>
      <input type="number" id="right-signal-gpio" name="rightSignalGpio" min="0" max="39" value="5" data-key="rightSignalGpio" />
    </section>

    <section id="gpio-states">
      <h2>GPIO States:</h2>
      <label>Light Signal:</label> <span id="light-state">—</span><br />
      <label>Brake Signal:</label> <span id="brake-state">—</span><br />
      <label>Left Indicator Signal:</label> <span id="left-state">—</span><br />
      <label>Right Indicator Signal:</label> <span id="right-state">—</span>
    </section>

    <div id="notification" class="notification"></div>

    <div class="settings-link">
      <a href="index.html" class="btn">Save Changes</a>
    </div>
  </form>
</main>

<script>
  const originalValues = {};

  // Update GPIO states spans from state data
  function updateGPIOStates(state) {
    if (!state) return;
    document.getElementById('light-state').textContent = state.lightSignal ?? '—';
    document.getElementById('brake-state').textContent = state.brakeSignal ?? '—';
    document.getElementById('left-state').textContent = state.leftIndicatorSignal ?? '—';
    document.getElementById('right-state').textContent = state.rightIndicatorSignal ?? '—';
  }

  // Show notification popup bottom-left
  function showNotification(message, isError = false) {
    const notif = document.getElementById('notification');
    notif.textContent = message;
    notif.style.background = isError ? '#a33' : '#292'; // red for error, green for success
    notif.classList.add('show');

    if (notif.timeoutId) clearTimeout(notif.timeoutId);

    notif.timeoutId = setTimeout(() => {
      notif.classList.remove('show');
    }, 3500);
  }

  window.addEventListener('DOMContentLoaded', () => {
    // 1. Fetch settings and fill form
    fetch('/getSettings')
      .then(res => {
        if (!res.ok) throw new Error(`Server error: ${res.status}`);
        return res.json();
      })
      .then(settings => {
        Object.entries(settings).forEach(([key, value]) => {
          const el = document.querySelector(`[data-key="\${key}"]`);
          if (!el) return; // no matching input found
          if (el.type === 'checkbox') {
            el.checked = value;
            originalValues[key] = el.checked;
          } else {
            el.value = value;
            originalValues[key] = el.value;
          }
        });
        showNotification('Settings loaded successfully.');
      })
      .catch(err => {
        console.error('Failed to load settings:', err);
        showNotification(`Failed to load settings: \${err.message}`, true);
      });

    // 2. Fetch GPIO states ASAP and update UI
    function fetchGPIOState() {
      fetch('/getState')
        .then(res => res.ok ? res.json() : Promise.reject(res.status))
        .then(state => updateGPIOStates(state))
        .catch(err => {
          console.error('Failed to fetch GPIO states:', err);
          showNotification(`Failed to fetch GPIO states: \${err}`, true);
        });
    }
    fetchGPIOState();

    // Optional: poll GPIO states every 5 seconds (uncomment if want)
    // setInterval(fetchGPIOState, 5000);

    // 3. Sliders send setting on input event (live)
    document.querySelectorAll('input[type="range"][data-key]').forEach(input => {
      input.addEventListener('input', () => {
        const key = input.dataset.key;
        if (!key) return;
        const val = input.value;
        if (originalValues[key] != val) {
          sendSetting(key, val);
        }
      });
    });

    // 4. Text/number send setting on blur (unfocus)
    document.querySelectorAll('input[type="text"][data-key], input[type="number"][data-key]').forEach(input => {
      input.addEventListener('blur', () => {
        const key = input.dataset.key;
        if (!key) return;
        const val = input.value;
        if (originalValues[key] != val) {
          sendSetting(key, val);
        }
      });
    });
  });

  // Send changed setting to ESP
  function sendSetting(key, value) {
    fetch(`/setSetting?key=\${encodeURIComponent(key)}&value=\${encodeURIComponent(value)}`)
      .then(res => {
        if (!res.ok) throw new Error(`Server error: \${res.status}`);
        console.log(`Saved \${key}: \${value}`);
        originalValues[key] = value;
        showNotification(`Saved "\${key}" successfully.`);
      })
      .catch(err => {
        console.error(`Error sending \${key}:`, err);
        showNotification(`Error sending "\${key}": \${err.message}`, true);
      });
  }

  // Loader fade on window load
  window.addEventListener('load', () => {
    const loader = document.getElementById('loader');
    if (loader) {
      loader.style.opacity = 0;
      loader.style.transition = 'opacity 0.5s ease';
      setTimeout(() => {
        loader.style.display = 'none';
        console.log('Loader faded out');
      }, 500);
    }
  });
</script>

</body>
</html>

)rawliteral";

#endif
