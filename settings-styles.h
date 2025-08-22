#ifndef SETTINGS_STYLES_H
#define SETTINGS_STYLES_H

const char* settingsStylesCSS = R"raw_string(
/* Loader */
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

.loader.hidden {
  display: none;
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

/* Base styles */
* {
  box-sizing: border-box;
}

/* Accessibility */
.visually-hidden {
  position: absolute !important;
  width: 1px !important;
  height: 1px !important;
  padding: 0 !important;
  margin: -1px !important;
  overflow: hidden !important;
  clip: rect(0, 0, 0, 0) !important;
  white-space: nowrap !important;
  border: 0 !important;
}

/* Utility Classes */
.backdrop-blur {
  backdrop-filter: blur(10px) saturate(120%);
  -webkit-backdrop-filter: blur(10px) saturate(120%);
}

html {
  background: #0a0a0a;
  height: 100%;
  width: 100%;
}

body {
  margin: 0;
  padding: 0;
  background: linear-gradient(135deg, #0a0a0a, #1a1a1a, #0f0f0f);
  color: #f1f1f1;
  font-family: -apple-system, BlinkMacSystemFont, "SF Pro Display", "Segoe UI",
    "Roboto", "Helvetica Neue", "Arial", sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  font-size: 16px;
  line-height: 1.5;
  font-weight: 400;
  letter-spacing: -0.01em;
  min-height: 100vh;
  width: 100%;
  position: relative;
}

/* Extended gradient coverage for overscroll areas */
body::before {
  content: '';
  position: fixed;
  top: -50%;
  left: -50%;
  width: 200%;
  height: 200%;
  background: linear-gradient(135deg, #0a0a0a, #1a1a1a, #0f0f0f);
  z-index: -1;
}

main {
  padding: 1.8rem 2rem 3rem;
  max-width: 420px;
  margin: 0 auto;
  display: flex;
  flex-direction: column;
  gap: 1.6rem;
}

/* Responsive typography */
@media (max-width: 480px) {
  body {
    font-size: 15px;
  }

  label {
    font-size: 0.9rem;
  }

  .dropdown-toggle {
    font-size: 0.85rem;
    padding: 0.8rem 1rem;
  }

  .quick-actions h3 {
    font-size: 1rem;
  }

  .action-text {
    font-size: 0.8rem;
  }

  .notification {
    font-size: 0.85rem;
    padding: 12px 18px;
  }

  .switch {
    padding: 0 0.3em;
    font-size: 0.9rem;
  }
}

@media (max-width: 360px) {
  body {
    font-size: 14px;
  }

  main {
    padding: 1.5rem 1.5rem 3rem;
  }

  .nav-title {
    font-size: 0.9rem;
  }

  .popupContent {
    padding: 0.8rem 1rem;
    max-width: 280px;
  }

  .popupContent h2 {
    font-size: 0.9rem;
    margin: 0 0 0.4rem;
  }

  .popup-icon {
    width: 1.3em;
    height: 1em;
  }

  .popupContent button {
    padding: 5px 10px;
    font-size: 0.7rem;
    margin-top: 0.6rem;
  }
}

/* Collapsible sections */
.collapsible-section {
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-radius: 10px;
  margin-bottom: 1rem;
  overflow: hidden;
  background: rgba(20, 20, 20, 0.85);
  box-shadow: 0 0 15px rgba(0, 0, 0, 0.4);
}

.section-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 0.9rem 1.1rem;
  background: rgba(20, 20, 20, 0.85);
  cursor: pointer;
  user-select: none;
  transition: all 0.3s ease;
  border-bottom: 1px solid rgba(255, 255, 255, 0.1);
  color: #f8f8f8;
  font-weight: 600;
  font-size: 0.9rem;
  letter-spacing: -0.02em;
  line-height: 1.3;
}

.section-header:hover {
  background: #222;
}

.section-header.has-errors {
  background: rgba(220, 53, 69, 0.1);
  border-left: 4px solid #dc3545;
}

.section-header.has-errors:hover {
  background: rgba(220, 53, 69, 0.15);
}

.section-header h2 {
  margin: 0;
  font-size: 0.9rem;
  font-weight: 600;
  color: #f8f8f8;
  letter-spacing: -0.02em;
  line-height: 1.3;
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
  background: rgba(20, 20, 20, 0.85);
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
  border-bottom: 1px solid rgba(255, 255, 255, 0.1);
}

section:not(.collapsible-section):last-of-type {
  border-bottom: none;
  padding-bottom: 0;
}

header {
  padding: 1.4rem 0;
  text-align: center;
  border-bottom: 1px solid rgba(255, 255, 255, 0.1);
  font-weight: 700;
  font-size: 1.7rem;
  color: #f8f8f8;
  letter-spacing: -0.03em;
}

.changes-indicator {
  background: rgba(255, 107, 53, 0.1);
  border: 1px solid #ff6b35;
  border-radius: 10px;
  padding: 0.8rem 1rem;
  text-align: center;
  font-weight: 600;
  color: #ff6b35;
  font-size: 0.9rem;
  transition: all 0.3s ease;
  box-shadow: 0 0 15px rgba(0, 0, 0, 0.4);
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
  padding: 0.7rem 1.2rem;
  font-size: 0.9rem;
  font-weight: 600;
  background: rgba(20, 20, 20, 0.85);
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-radius: 25px;
  color: #f8f8f8;
  cursor: pointer;
  transition: all 0.3s ease;
  box-shadow: 0 0 15px rgba(0, 0, 0, 0.4);
  letter-spacing: -0.02em;
  line-height: 1.3;
  font-family: inherit;
}

.btn.small:hover {
  background: rgba(30, 30, 30, 0.9);
  border-color: #4db8ff;
  transform: translateY(-2px);
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
}

.btn.small:active {
  transform: translateY(0);
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}

form {
  display: flex;
  flex-direction: column;
  gap: 1.4rem;
}

h2 {
  font-size: 1.2rem;
  margin: 1rem 0 0.5rem 0;
  font-weight: 700;
  color: #f8f8f8;
  letter-spacing: -0.03em;
  line-height: 1.3;
}

label {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  margin-bottom: 0.5rem;
  font-weight: 600;
  font-size: 0.95rem;
  color: #e0e0e0;
  text-transform: none;
  letter-spacing: -0.02em;
  line-height: 1.4;
}

.label-icon {
  width: 16px;
  height: 16px;
  flex-shrink: 0;
  opacity: 0.8;
}

input[type="range"] {
  appearance: none;
  -webkit-appearance: none;
  width: 100%;
  height: 8px;
  border-radius: 6px;
  background: rgba(20, 20, 20, 0.85);
  border: 1px solid rgba(255, 255, 255, 0.1);
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
  margin-top: 0;
}

input[type="range"]::-moz-range-thumb {
  width: 18px;
  height: 18px;
  border-radius: 50%;
  background: #4db8ff;
  cursor: pointer;
  border: none;
}

input[type="number"],
input[type="text"],
input[type="password"] {
  width: 100%;
  padding: 0.9rem 1.1rem;
  background: rgba(20, 20, 20, 0.85);
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-radius: 10px;
  color: #f8f8f8;
  font-size: 0.9rem;
  font-weight: 600;
  cursor: text;
  transition: all 0.3s ease;
  box-shadow: 0 0 15px rgba(0, 0, 0, 0.4);
}

input[type="number"] {
  box-sizing: border-box;
}

input[type="number"]:focus,
input[type="text"]:focus,
input[type="password"]:focus {
  outline: none;
  border-color: #4db8ff;
  background: rgba(30, 30, 30, 0.9);
  transform: translateY(-1px);
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
}

.toggle-password {
  position: absolute;
  right: 0.75rem;
  top: 50%;
  transform: translateY(-50%);
  color: #888;
  cursor: pointer;
  font-size: 0.9rem;
  user-select: none;
  padding: 0 0.25rem;
  transition: color 0.2s ease;
  z-index: 2;
}

.toggle-password:hover {
  color: #4db8ff;
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
  width: 100%;
  padding: 0.9rem 1.1rem;
  background: #4296f4;
  color: white;
  border: none;
  border-radius: 10px;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.3s ease;
  font-size: 0.9rem;
  box-shadow: 0 0 15px rgba(0, 0, 0, 0.4);
  text-decoration: none;
  font-family: inherit;
}

.settings-link .btn:hover:not(:disabled) {
  background: #3a7bc8;
  transform: translateY(-1px);
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
}

.settings-link .btn:disabled {
  background: #555;
  color: #888;
  cursor: not-allowed;
  transform: none;
}

.settings-link .btn.secondary {
  background: #666;
  margin-left: 1rem;
}

.settings-link .btn.secondary:hover:not(:disabled) {
  background: #555;
  transform: translateY(-1px);
}

.settings-link .btn.warning {
  background: #d32f2f;
  margin-left: 1rem;
}

.settings-link .btn.warning:hover:not(:disabled) {
  background: #b71c1c;
  transform: translateY(-1px);
}

.btn {
  display: inline-block;
  font-weight: 600;
  font-size: 1rem;
  text-transform: none;
  font-family: inherit;
}

.notification {
  position: fixed;
  bottom: 20px;
  left: 20px;
  background: rgba(20, 20, 20, 0.85);
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-radius: 15px;
  color: #f8f8f8;
  font-weight: 600;
  padding: 14px 22px;
  box-shadow: 0 0 15px rgba(0, 0, 0, 0.4);
  visibility: hidden;
  opacity: 0;
  pointer-events: none;
  transition: all 0.4s ease;
  font-size: 0.9rem;
  max-width: 300px;
  z-index: 10000;
  user-select: none;
  box-sizing: border-box;
  letter-spacing: -0.01em;
  line-height: 1.4;
  display: flex;
  align-items: center;
  gap: 0.6rem;
  transform: translateY(20px);
  touch-action: pan-y;
}

.notification.error {
  background: rgba(220, 53, 69, 0.9);
  border: 1px solid rgba(255, 150, 150, 0.3);
  box-shadow: 0 0 15px rgba(220, 53, 69, 0.3);
}

.notification-icon {
  width: 1.2em;
  height: 1.2em;
  flex-shrink: 0;
  opacity: 0.9;
}

.notification-text {
  flex: 1;
  font-weight: 500;
  font-size: 0.8rem;
  color: #f0f0f0;
  line-height: 1.3;
}

.notification-content {
  flex: 1;
  display: flex;
  flex-direction: column;
  gap: 0.2rem;
}

.notification-title {
  font-weight: 700;
  font-size: 0.85rem;
  color: #ffffff;
  line-height: 1.2;
}

.notification.show {
  visibility: visible;
  opacity: 1;
  pointer-events: auto;
  transform: translateY(0);
}

#toggles {
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
  width: 3.5em;
  height: 2em;
  background: rgba(20, 20, 20, 0.85);
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-radius: 1em;
  position: relative;
  transition: all 0.3s ease-in-out;
  box-shadow: 0 0 15px rgba(0, 0, 0, 0.4);
}

#toggles input[type="checkbox"]+.slider::after {
  content: "";
  display: inline-block;
  width: 1.5em;
  height: 1.5em;
  background: #fff;
  border-radius: 50%;
  position: absolute;
  top: 50%;
  left: 0.25em;
  transform: translateY(-50%);
  box-shadow: 10px 0 40px rgba(0, 0, 0, 0.1);
  transition: transform 0.3s ease, background-color 0.3s ease;
}

#toggles input[type="checkbox"]:checked+.slider {
  background: #4db8ff;
  border-color: #4db8ff;
}

#toggles input[type="checkbox"]:checked+.slider::after {
  left: auto;
  right: 0.25em;
  box-shadow: -10px 0 40px rgba(0, 0, 0, 0.1);
}

#toggles input[type="checkbox"]:active+.slider::after,
#toggles input[type="checkbox"]:checked:active+.slider::after {
  transform: translateY(-50%) scaleX(1.1);
}



.input-suffix-wrapper {
  position: relative;
  display: inline-block;
  width: 100%;
}

.input-with-suffix {
  padding-right: 4.5em;
  width: 100%;
}

/* Extra padding for password fields to accommodate eye icon */
input[type="password"].input-with-suffix {
  padding-right: 6.5em;
}

.input-suffix {
  position: absolute;
  right: 1.2em;
  top: 50%;
  transform: translateY(-50%);
  pointer-events: none;
  color: #b3b3b3;
  font-size: 0.75em;
  letter-spacing: 0.5px;
}

.input-suffix-wrapper .input-suffix:last-of-type {
  right: 1.2em;
}

/* Special positioning for password field suffix */
.input-suffix-wrapper .toggle-password+.input-suffix {
  right: 4em;
  font-size: 0.7em;
}

/* Hide number input arrows/spinners */
input[type="number"]::-webkit-outer-spin-button,
input[type="number"]::-webkit-inner-spin-button {
  -webkit-appearance: none;
  margin: 0;
}

input[type="number"] {
  -moz-appearance: textfield;
}



#colour-section label.toggle-label {
  display: flex;
  justify-content: space-between;
  align-items: center;
  cursor: pointer;
  margin-bottom: 1rem;
  color: #e0e0e0;
  font-weight: 600;
  font-size: 0.95rem;
  letter-spacing: -0.02em;
}

#colour-section input[type="color"] {
  width: 2.4rem;
  height: 2.4rem;
  border: none;
  padding: 0;
  margin-left: 0.5rem;
  cursor: pointer;
  border-radius: 50%;
  background: none;
  transition: transform 0.2s ease;
  box-shadow: 0 0 15px rgba(0, 0, 0, 0.4);
}

#colour-section input[type="color"]:hover {
  transform: scale(1.1);
}

#colour-section input[type="color"]::-webkit-color-swatch-wrapper {
  padding: 0;
}

#colour-section input[type="color"]::-webkit-color-swatch {
  border-radius: 50%;
  border: none;
}



/* GPIO Dashboard Styles */
.gpio-dashboard {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 1rem;
}

.gpio-card {
  position: relative;
  display: flex;
  align-items: center;
  padding: 1rem;
  background: rgba(20, 20, 20, 0.85);
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-radius: 12px;
  transition: all 0.3s ease;
  overflow: hidden;
  min-height: 4rem;
}



.gpio-icon {
  display: flex;
  align-items: center;
  justify-content: center;
  width: 2.5rem;
  height: 2.5rem;
  margin-right: 0.8rem;
  background: rgba(102, 102, 102, 0.2);
  border-radius: 8px;
  color: #666;
  transition: all 0.3s ease;
  flex-shrink: 0;
}

.gpio-icon svg {
  width: 1.2rem;
  height: 1.2rem;
  display: block;
}



.gpio-info {
  flex: 1;
  display: flex;
  align-items: center;
}

.gpio-label {
  font-weight: 600;
  color: #e0e0e0;
  font-size: 0.9rem;
  letter-spacing: -0.02em;
}

/* ON State - Green */
.gpio-card[data-state="on"] .gpio-icon {
  background: rgba(74, 170, 51, 0.2);
  color: #4a3;
}

/* OFF State - Red */
.gpio-card[data-state="off"] .gpio-icon {
  background: rgba(170, 51, 51, 0.2);
  color: #a33;
}







/* Invalid field highlighting */
.invalid-field {
  border: 2px solid #dc3545 !important;
  background: rgba(220, 53, 69, 0.1) !important;
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

/* Popup Styles */
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
  background: rgba(20, 20, 20, 0.85);
  border: 1px solid rgba(255, 255, 255, 0.1);
  padding: 1rem 1.2rem;
  border-radius: 10px;
  max-width: 320px;
  width: 85%;
  text-align: center;
  color: #f1f1f1;
  box-shadow: 0 0 15px rgba(0, 0, 0, 0.4);
}

.popupContent button,
.popup-buttons .btn {
  background: #4db8ff;
  border: none;
  padding: 0.6rem 1.2rem;
  font-weight: 600;
  color: white;
  border-radius: 8px;
  cursor: pointer;
  transition: all 0.3s ease;
  font-size: 0.85rem;
  box-shadow: 0 0 15px rgba(0, 0, 0, 0.4);
  font-family: inherit;
}

.popupContent button {
  margin-top: 0.8rem;
}

.popupContent button:hover:not(:disabled),
.popup-buttons .btn:hover:not(:disabled) {
  background: #4296f4;
  transform: translateY(-1px);
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
}

.popupContent button:active,
.popup-buttons .btn:active {
  transform: translateY(0);
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}

.popupContent h2 {
  margin: 0 0 0.5rem;
  font-size: 1rem;
  font-weight: 600;
  color: #f8f8f8;
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 0.4rem;
  justify-content: center;
}

.popup-icon {
  width: 1.5em;
  height: 1.2em;
  flex-shrink: 0;
  opacity: 0.9;
  transform: scale(1.25);
}

.popup-buttons {
  display: flex;
  gap: 0.8rem;
  justify-content: center;
}

.popup-buttons .btn {
  min-width: 80px;
  text-decoration: none;
  display: inline-block;
}

.popup-buttons .btn.secondary {
  background: #666;
}

.popup-buttons .btn.secondary:hover:not(:disabled) {
  background: #555;
}

.popup-buttons .btn.warning {
  background: #d32f2f;
}

.popup-buttons .btn.warning:hover:not(:disabled) {
  background: #b71c1c;
}

/* Update section styles */
.update-info {
  padding: 1rem 0;
}

.update-info p {
  margin: 0 0 1rem 0;
  color: #e0e0e0;
  font-size: 0.9rem;
  line-height: 1.4;
  letter-spacing: -0.02em;
}

.file-upload-area {
  margin-bottom: 1rem;
}

.file-upload-area .btn {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  padding: 0.9rem 1.1rem;
  background: rgba(20, 20, 20, 0.85);
  border: 2px dashed rgba(255, 255, 255, 0.2);
  border-radius: 10px;
  color: #f1f1f1;
  cursor: pointer;
  transition: all 0.3s ease;
  width: 100%;
  justify-content: center;
  font-weight: 600;
  box-shadow: 0 0 15px rgba(0, 0, 0, 0.4);
}

.file-upload-area .btn:hover {
  background: rgba(30, 30, 30, 0.9);
  border-color: #4db8ff;
  transform: translateY(-1px);
}

.upload-icon {
  width: 1.2rem;
  height: 1.2rem;
}

.file-info {
  margin-top: 0.75rem;
  padding: 0.75rem;
  background: rgba(20, 20, 20, 0.85);
  border-radius: 10px;
  border: 1px solid rgba(255, 255, 255, 0.1);
  display: flex;
  flex-direction: column;
  gap: 0.25rem;
  box-shadow: 0 0 15px rgba(0, 0, 0, 0.4);
}

.file-info.hidden {
  display: none;
}

.file-info #file-name {
  font-weight: 600;
  color: #f8f8f8;
  letter-spacing: -0.02em;
}

.file-info #file-size {
  font-size: 0.85rem;
  color: #888;
}

#upload-btn {
  width: 100%;
  padding: 0.9rem 1.1rem;
  background: #d32f2f;
  color: white;
  border: none;
  border-radius: 10px;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.3s ease;
  font-size: 0.9rem;
  box-shadow: 0 0 15px rgba(0, 0, 0, 0.4);
  font-family: inherit;
}

#upload-btn:hover:not(:disabled) {
  background: #b71c1c;
  transform: translateY(-1px);
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
}

#upload-btn:disabled {
  background: #555;
  color: #888;
  cursor: not-allowed;
  transform: none;
}

/* Progress bar for upload */
.upload-progress {
  margin-top: 1rem;
  background: rgba(20, 20, 20, 0.85);
  border-radius: 6px;
  overflow: hidden;
  height: 8px;
  border: 1px solid rgba(255, 255, 255, 0.1);
}

.upload-progress-bar {
  height: 100%;
  background: linear-gradient(90deg, #4caf50, #8bc34a);
  width: 0%;
  transition: width 0.3s ease;
}

/* Upload status messages */
.upload-status {
  margin-top: 0.75rem;
  padding: 0.75rem;
  border-radius: 10px;
  font-size: 0.9rem;
  border: 1px solid rgba(255, 255, 255, 0.1);
  box-shadow: 0 0 15px rgba(0, 0, 0, 0.4);
}

.upload-status.success {
  background: rgba(27, 94, 32, 0.1);
  color: #c8e6c9;
  border-color: #2e7d32;
}

.upload-status.error {
  background: rgba(183, 28, 28, 0.1);
  color: #ffcdd2;
  border-color: #c62828;
}

.upload-status.info {
  background: rgba(21, 101, 192, 0.1);
  color: #bbdefb;
  border-color: #1976d2;
}
)raw_string";

#endif
