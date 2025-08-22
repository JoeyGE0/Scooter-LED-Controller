#ifndef SETTINGS_SCRIPT_H
#define SETTINGS_SCRIPT_H

const char* settingsScriptJS = R"raw_string(
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

  // Setup event listeners for all input types
  const inputTypes = {
    'input[type="range"]': (input) => {
      const valueDisplay = document.getElementById(input.id + '-value');
      if (valueDisplay) {
        input.addEventListener('input', () => {
          valueDisplay.textContent = input.value;
          markAsChanged();
        });
      }
    },
        'input[type="number"]': (input) => {
      input.addEventListener('input', () => {
        // Remove any non-numeric characters except minus sign
        let value = input.value.replace(/[^0-9-]/g, '');
        
        // Handle minus sign (only allow at start)
        if (value.startsWith('-') && value.length > 1) {
          value = value.replace(/-/g, '');
        }
        
        // Update input value
        input.value = value;
        
        // Validate min/max if value is not empty
        if (value !== '' && value !== '-') {
          const numValue = parseInt(value);
          const min = parseInt(input.min);
          const max = parseInt(input.max);
          
          if (!isNaN(numValue)) {
            if (numValue < min) input.value = min;
            if (numValue > max) input.value = max;
          }
        }

        input.classList.remove('invalid-field');
        markAsChanged();
      });
    },
    'input[type="text"], input[type="password"]': (input) => {
      input.addEventListener('input', () => {
        input.classList.remove('invalid-field');
        markAsChanged();
      });
    },
    'input[type="color"]': (input) => {
      input.addEventListener('change', () => {
        input.classList.remove('invalid-field');
        markAsChanged();
      });
    },
    'input[type="checkbox"]': (input) => {
      input.addEventListener('change', () => {
        markAsChanged();
      });
    }
  };

  // Apply event listeners to all input types
  Object.entries(inputTypes).forEach(([selector, handler]) => {
    document.querySelectorAll(selector).forEach(handler);
  });

  // Start GPIO state monitoring
  updateGpioStates();
  setInterval(updateGpioStates, 1000); // Update every second

  // Setup firmware upload functionality
  setupFirmwareUpload();
});

function applySettingsToUI(settings) {
  const settingMap = {
    // Brightness settings
    fullBrakeBrightness: { id: 'full-brake-brightness', hasValueDisplay: true },
    idleBrakeBrightness: { id: 'idle-brake-brightness' },
    indicatorBackroundDim: { id: 'indicator-backround-dim' },
    
    // Color settings
    bootColour: { id: 'boot-colour' },
    indicatorColour: { id: 'indicator-colour' },
    
    // Transition settings
    indicatorTransition: { id: 'indicator-transition' },
    colourTransition: { id: 'colour-transition' },
    
    // Timing settings
    indicatorFlashTime: { id: 'indicator-flash-time' },
    firstFlashDuration: { id: 'first-flash-duration' },
    trailFlashInterval: { id: 'trail-flash-interval' },
    bootDuration: { id: 'boot-duration' },
    
    // LED count settings
    totalLedCount: { id: 'total-led-count' },
    brakeLedStart: { id: 'brake-led-start' },
    brakeLedEnd: { id: 'brake-led-end' },
    indicatorTrailLength: { id: 'indicator-trail-length' },
    
    // Toggle settings
    defaultLightMode: { id: 'default-light-mode', type: 'checkbox' },
    bootEffectToggle: { id: 'boot-effect-toggle', type: 'checkbox' },
    indicatorEffectToggle: { id: 'indicator-effect-toggle', type: 'checkbox' },
    leftTrailReverse: { id: 'left-trail-reverse', type: 'checkbox' },
    rightTrailReverse: { id: 'right-trail-reverse', type: 'checkbox' },
    reverseIndicatorSides: { id: 'reverse-indicator-sides', type: 'checkbox' },
    xlightsToggle: { id: 'xlights-toggle', type: 'checkbox' },
    
        // GPIO settings
    ledDataGpio: { id: 'led-data-gpio' },
    brakeSignalGpio: { id: 'brake-signal-gpio' },
    lightSignalGpio: { id: 'light-signal-gpio' },
    leftSignalGpio: { id: 'left-signal-gpio' },
    rightSignalGpio: { id: 'right-signal-gpio' },
    
    // Network settings
    captiveSsid: { id: 'captive-ssid' },
    captivePassword: { id: 'captive-password' }
  };

  // Apply settings using the map
  Object.entries(settingMap).forEach(([key, config]) => {
    if (settings[key] !== undefined) {
      const input = document.getElementById(config.id);
      if (input) {
        if (config.type === 'checkbox') {
          input.checked = settings[key];
        } else {
          input.value = settings[key];
          // Handle value displays for range inputs
          if (config.hasValueDisplay) {
            const valueDisplay = document.getElementById(config.id + '-value');
            if (valueDisplay) valueDisplay.textContent = settings[key];
          }
        }
      }
    }
  });
}

function validateSettings(settings) {
  const errors = [];
  const colorRegex = /^#[0-9A-Fa-f]{6}$/;

  // Validation rules configuration
  const validationRules = {
    // Network validation
    captiveSsid: [
      { test: !settings.captiveSsid || settings.captiveSsid.trim() === '', message: 'SSID is required' },
      { test: settings.captiveSsid && !/^[a-zA-Z0-9_-]+$/.test(settings.captiveSsid), message: 'SSID can only contain letters, numbers, underscores, and hyphens' },
      { test: settings.captiveSsid && settings.captiveSsid.length > 32, message: 'SSID must be 32 characters or less' }
    ],
    captivePassword: [
      { test: settings.captivePassword && settings.captivePassword.trim() !== '' && settings.captivePassword.length < 8, message: 'Password must be at least 8 characters (or leave empty for no password)' },
      { test: settings.captivePassword && settings.captivePassword.length > 64, message: 'Password must be 64 characters or less' },
      { test: settings.captivePassword && !/^[a-zA-Z0-9!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?]+$/.test(settings.captivePassword), message: 'Password contains invalid characters' }
    ],
    
    // LED count validation
    totalLedCount: [{ test: settings.totalLedCount < 1 || settings.totalLedCount > 1000, message: 'Total LED count must be between 1 and 1000' }],
    brakeLedStart: [{ test: settings.brakeLedStart < 0 || settings.brakeLedStart >= settings.totalLedCount, message: 'Brake LED start must be between 0 and total LED count' }],
    brakeLedEnd: [{ test: settings.brakeLedEnd < settings.brakeLedStart || settings.brakeLedEnd >= settings.totalLedCount, message: 'Brake LED end must be after start and less than total LED count' }],
    indicatorTrailLength: [{ test: settings.indicatorTrailLength < 1 || settings.indicatorTrailLength > 100, message: 'Indicator trail length must be between 1 and 100' }],
    
    // Brightness validation
    fullBrakeBrightness: [{ test: settings.fullBrakeBrightness < 0 || settings.fullBrakeBrightness > 255, message: 'Full brake brightness must be between 0 and 255' }],
    idleBrakeBrightness: [{ test: settings.idleBrakeBrightness < 0 || settings.idleBrakeBrightness > 255, message: 'Idle brake brightness must be between 0 and 255' }],
    indicatorBackroundDim: [{ test: settings.indicatorBackroundDim < 0 || settings.indicatorBackroundDim > 255, message: 'Indicator background dim must be between 0 and 255' }],
    
    // Timing validation
    indicatorTransition: [{ test: settings.indicatorTransition < 0 || settings.indicatorTransition > 4000, message: 'Indicator transition must be between 0 and 4000ms' }],
    indicatorFlashTime: [{ test: settings.indicatorFlashTime < 0 || settings.indicatorFlashTime > 1000, message: 'Indicator flash time must be between 0 and 1000ms' }],
    trailFlashInterval: [{ test: settings.trailFlashInterval < 0 || settings.trailFlashInterval > 1000, message: 'Trail flash interval must be between 0 and 1000ms' }],
    bootDuration: [{ test: settings.bootDuration < 0 || settings.bootDuration > 10000, message: 'Boot duration must be between 0 and 10000ms' }],
    
    // Color validation
    bootColour: [{ test: !colorRegex.test(settings.bootColour), message: 'Boot color must be a valid hex color (e.g., #FF0000)' }],
    indicatorColour: [{ test: !colorRegex.test(settings.indicatorColour), message: 'Indicator color must be a valid hex color (e.g., #FF0000)' }]
  };

  // Run validation rules
  Object.entries(validationRules).forEach(([field, rules]) => {
    rules.forEach(rule => {
      if (rule.test) errors.push(rule.message);
    });
  });

  // GPIO validation (special case)
  const gpioPins = [settings.ledDataGpio, settings.brakeSignalGpio, settings.lightSignalGpio, settings.leftSignalGpio, settings.rightSignalGpio];
  if (new Set(gpioPins).size !== gpioPins.length) {
    errors.push('All GPIO pins must be unique');
  }
  if (gpioPins.some(pin => pin < 0 || pin > 39)) {
    errors.push('GPIO pins must be between 0 and 39');
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
    'Password must be at least 8 characters (or leave empty for no password)': { field: 'captive-password', section: 'network-auth' },
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
    firstFlashDuration: parseInt(document.getElementById('first-flash-duration').value),
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
    reverseIndicatorSides: document.getElementById('reverse-indicator-sides').checked,
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
      const statusMap = {
        'power': data.lightState,
        'brake': data.brakeState,
        'left': data.leftState,
        'right': data.rightState
      };

      Object.entries(statusMap).forEach(([gpio, state]) => {
        const card = document.querySelector(`[data-gpio="${gpio}"]`);

        if (card) {
          // Update card state (this will change icon and dot colors)
          card.setAttribute('data-state', state ? 'on' : 'off');
        }
      });
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
    'Factory Reset',
    'Are you sure you want to COMPLETELY RESET all settings to factory defaults and restart the device? This will clear ALL saved settings and cannot be undone.',
    () => {
      // Show loading state
      showNotification('Performing factory reset...', false);
      
      // Call factory reset endpoint
      fetch('/factoryReset', {
        method: 'POST',
        headers: {
          'Content-Type': 'text/plain',
        },
        body: ''
      })
      .then(response => {
        if (response.ok) {
          showNotification('Factory reset complete! Device is restarting...', false);
          // Redirect to main page after restart
          setTimeout(() => {
            window.location.href = '/';
          }, 5000);
        } else {
          throw new Error('Factory reset failed');
        }
      })
      .catch(error => {
        console.error('Factory reset error:', error);
        showNotification('Factory reset failed. Please try again.', true);
      });
    }
  );
}

// Firmware upload functionality
let selectedFile = null;

function setupFirmwareUpload() {
  const fileInput = document.getElementById('firmware-file');
  const uploadBtn = document.getElementById('upload-btn');
  
  if (fileInput) {
    fileInput.addEventListener('change', handleFileSelect);
  }
  
  if (uploadBtn) {
    uploadBtn.addEventListener('click', uploadFirmware);
  }
}

function handleFileSelect(event) {
  const file = event.target.files[0];
  const fileInfo = document.getElementById('file-info');
  const fileName = document.getElementById('file-name');
  const fileSize = document.getElementById('file-size');
  const uploadBtn = document.getElementById('upload-btn');
  
  console.log('File select event triggered');
  console.log('Selected file:', file);
  
  if (file) {
    // Validate file type
    if (!file.name.toLowerCase().endsWith('.bin')) {
      showNotification('Please select a valid .bin firmware file.', true);
      event.target.value = '';
      return;
    }
    
    // Validate file size (ESP32 typically has 4MB flash, so limit to 2MB for safety)
    const maxSize = 2 * 1024 * 1024; // 2MB
    if (file.size > maxSize) {
      showNotification('File size too large. Please select a file smaller than 2MB.', true);
      event.target.value = '';
      return;
    }
    
    selectedFile = file;
    
    // Debug: Log file details
    console.log('File selected:', file);
    console.log('File size:', file.size);
    console.log('File name:', file.name);
    console.log('File type:', file.type);
    
    // Update UI
    fileName.textContent = file.name;
    fileSize.textContent = formatFileSize(file.size);
    fileInfo.classList.remove('hidden');
    uploadBtn.disabled = false;
  } else {
    selectedFile = null;
    fileInfo.classList.add('hidden');
    uploadBtn.disabled = true;
  }
}

function formatFileSize(bytes) {
  if (bytes === 0) return '0 Bytes';
  const k = 1024;
  const sizes = ['Bytes', 'KB', 'MB', 'GB'];
  const i = Math.floor(Math.log(bytes) / Math.log(k));
  return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i];
}

function uploadFirmware() {
  if (!selectedFile) {
    showNotification('Please select a firmware file first.', true);
    return;
  }
  
  // Show confirmation popup
  openPopup(
    'Upload Firmware',
    `Are you sure you want to upload and flash "${selectedFile.name}"? The device will restart after a successful update. This process cannot be interrupted.`,
    () => {
      performFirmwareUpload();
    }
  );
}

async function performFirmwareUpload() {
  const uploadBtn = document.getElementById('upload-btn');
  const updateInfo = document.querySelector('.update-info');
  
  // Disable upload button
  uploadBtn.disabled = true;
  uploadBtn.textContent = 'Uploading...';
  
  // Add progress bar
  const progressHtml = `
    <div class="upload-progress">
      <div class="upload-progress-bar" id="upload-progress-bar"></div>
    </div>
    <div class="upload-status info" id="upload-status">
      Preparing upload...
    </div>
  `;
  
  // Insert progress bar after the upload button
  uploadBtn.insertAdjacentHTML('afterend', progressHtml);
  
  const progressBar = document.getElementById('upload-progress-bar');
  const statusDiv = document.getElementById('upload-status');
  
  try {
    // Create XMLHttpRequest for upload
    const xhr = new XMLHttpRequest();
    
    // Track upload progress
    xhr.upload.addEventListener('progress', (e) => {
      if (e.lengthComputable) {
        const percentComplete = (e.loaded / e.total) * 100;
        progressBar.style.width = percentComplete + '%';
        statusDiv.textContent = `Uploading... ${Math.round(percentComplete)}%`;
      }
    });
    
    // Handle completion
    xhr.addEventListener('load', () => {
      console.log('Upload completed with status:', xhr.status);
      console.log('Response text:', xhr.responseText);
      
      if (xhr.status === 200) {
        // Update status
        statusDiv.textContent = 'Upload successful! Device is restarting...';
        statusDiv.className = 'upload-status success';
        progressBar.style.width = '100%';
        
        // Show success notification
        showNotification('Firmware uploaded successfully! Device is restarting...', false);
        
        // Wait a moment then redirect to main page
        setTimeout(() => {
          window.location.href = '/';
        }, 3000);
      } else {
        // Try to get more detailed error info
        let errorMsg = `Upload failed: ${xhr.status} ${xhr.statusText}`;
        if (xhr.responseText) {
          errorMsg += ` - ${xhr.responseText}`;
        }
        throw new Error(errorMsg);
      }
    });
    
    // Handle errors
    xhr.addEventListener('error', () => {
      throw new Error('Network error during upload');
    });
    
    // Handle timeout
    xhr.addEventListener('timeout', () => {
      throw new Error('Upload timeout - please try again');
    });
    
    // Create FormData
    const formData = new FormData();
    formData.append('update', selectedFile);
    
    // Start upload
    xhr.open('POST', '/update');
    xhr.timeout = 300000; // 5 minutes timeout
    xhr.send(formData);
    
  } catch (error) {
    console.error('Upload error:', error);
    
    // Update status
    statusDiv.textContent = `Upload failed: ${error.message}`;
    statusDiv.className = 'upload-status error';
    
    // Re-enable upload button
    uploadBtn.disabled = false;
    uploadBtn.textContent = 'Upload & Flash Firmware';
    
    // Show error notification
    showNotification(`Upload failed: ${error.message}`, true);
  }
}
)raw_string";

#endif
