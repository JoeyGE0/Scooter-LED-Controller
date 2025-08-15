// Global variables
let colorPicker;
let lastColor = '#ff0000';

// DOM elements cache
const elements = {
  loader: null,
  effects: null,
  dropdownToggle: null,
  effectButtons: null,
  effectDropdown: null,
  popupBox: null,
  popupCloseBtn: null,
  popupTitle: null,
  popupDesc: null,
  swatchGrid: null,
  notification: null,
  effectSpeedSlider: null,
  effectSpeedValue: null
};

// Cached selectors and constants
const SELECTORS = {
  EFFECT_BTN: '.effect-btn',
  ACTION_BTN: '.action-btn',
  DROPDOWN_TOGGLE: '.dropdown-toggle',
  EFFECT_DROPDOWN: '.effect-dropdown',
  EFFECT_SPEED: '#effect-speed',
  EFFECT_SPEED_VALUE: '#effect-speed-value'
};

// Effect icons cache
const EFFECT_ICONS = {
  'Solid': '<svg class="dropdown-icon" xmlns="http://www.w3.org/2000/svg" fill="currentColor" viewBox="0 0 16 16"><circle cx="8" cy="8" r="8"/></svg>',
  'Rainbow': '<svg class="dropdown-icon" xmlns="http://www.w3.org/2000/svg" fill="currentColor" viewBox="0 0 16 16"><path d="M8 4.5a7 7 0 0 0-7 7 .5.5 0 0 1-1 0 8 8 0 1 1 16 0 .5.5 0 0 1-1 0 7 7 0 0 0-7-7m0 2a5 5 0 0 0-5 5 .5.5 0 0 1-1 0 6 6 0 1 1 12 0 .5.5 0 0 1-1 0 5 5 0 0 0-5-5m0 2a3 3 0 0 0-3 3 .5.5 0 0 1-1 0 4 4 0 1 1 8 0 .5.5 0 0 1-1 0 3 3 0 0 0-3-3m0 2a1 1 0 0 0-1 1 .5.5 0 0 1-1 0 2 2 0 1 1 4 0 .5.5 0 0 1-1 0 1 1 0 0 0-1-1"/></svg>',
  'Pulse': '<svg class="dropdown-icon" xmlns="http://www.w3.org/2000/svg" fill="currentColor" viewBox="0 0 16 16"><path fill-rule="evenodd" d="M6 2a.5.5 0 0 1 .47.33L10 12.036l1.53-4.208A.5.5 0 0 1 12 7.5h3.5a.5.5 0 0 1 0 1h-3.15l-1.88 5.17a.5.5 0 0 1-.94 0L6 3.964 4.47 8.171A.5.5 0 0 1 4 8.5H.5a.5.5 0 0 1 0-1h3.15l1.88-5.17A.5.5 0 0 1 6 2"/></svg>',
  'Sparkle': '<svg class="dropdown-icon" xmlns="http://www.w3.org/2000/svg" fill="currentColor" viewBox="0 0 16 16"><path d="M7.657 6.247c.11-.33.576-.33.686 0l.645 1.937a2.89 2.89 0 0 0 1.829 1.828l1.936.645c.33.11.33.576 0 .686l-1.937.645a2.89 2.89 0 0 0-1.828 1.829l-.645 1.936a.361.361 0 0 1-.686 0l-.645-1.937a2.89 2.89 0 0 0-1.828-1.828l-1.937-.645a.361.361 0 0 1 0-.686l1.937-.645a2.89 2.89 0 0 0 1.828-1.828zM3.794 1.148a.217.217 0 0 1 .412 0l.387 1.162c.173.518.579.924 1.097 1.097l1.162.387a.217.217 0 0 1 0 .412l-1.162.387A1.73 1.73 0 0 0 4.593 5.69l-.387 1.162a.217.217 0 0 1-.412 0L3.407 5.69A1.73 1.73 0 0 0 2.31 4.593l-1.162-.387a.217.217 0 0 1 0-.412l1.162-.387A1.73 1.73 0 0 0 3.407 2.31zM10.863.099a.145.145 0 0 1 .274 0l.258.774c.115.346.386.617.732.732l.774.258a.145.145 0 0 1 0 .274l-.774.258a1.16 1.16 0 0 0-.732.732l-.258.774a.145.145 0 0 1-.274 0l-.258-.774a1.16 1.16 0 0 0-.732-.732L9.1 2.137a.145.145 0 0 1 0-.274l.774-.258c.346-.115.617-.386.732-.732z"/></svg>',
  'Chase': '<svg class="dropdown-icon" xmlns="http://www.w3.org/2000/svg" fill="currentColor" viewBox="0 0 16 16"><path d="M11.534 7h3.932a.25.25 0 0 1 .192.41l-1.966 2.36a.25.25 0 0 1-.384 0l-1.966-2.36a.25.25 0 0 1 .192-.41m-11 2h3.932a.25.25 0 0 0 .192-.41L2.692 6.23a.25.25 0 0 0-.384 0L.342 8.59A.25.25 0 0 0 .534 9"/><path fill-rule="evenodd" d="M8 3c-1.552 0-2.94.707-3.857 1.818a.5.5 0 1 1-.771-.636A6.002 6.002 0 0 1 13.917 7H12.9A5 5 0 0 0 8 3M3.1 9a5.002 5.002 0 0 0 8.757 2.182.5.5 0 1 1 .771.636A6.002 6.002 0 0 1 2.083 9z"/></svg>',
  'Hazard': '<svg class="dropdown-icon" xmlns="http://www.w3.org/2000/svg" fill="currentColor" viewBox="0 0 16 16"><path d="M7.938 2.016A.13.13 0 0 1 8.002 2a.13.13 0 0 1 .063.016.15.15 0 0 1 .054.057l6.857 11.667c.036.06.035.124.002.183a.2.2 0 0 1-.054.06.1.1 0 0 1-.066.017H1.146a.1.1 0 0 1-.066-.017.2.2 0 0 1-.054-.06.18.18 0 0 1 .002-.183L7.884 2.073a.15.15 0 0 1 .054-.057m1.044-.45a1.13 1.13 0 0 0-1.96 0L.165 13.233c-.457.778.091 1.767.98 1.767h13.713c.889 0 1.438-.99.98-1.767z"/><path d="M7.002 12a1 1 0 1 1 2 0 1 1 0 0 1-2 0M7.1 5.995a.905.905 0 1 1 1.8 0l-.35 3.507a.552.552 0 0 1-1.1 0z"/></svg>',
  'X-lights': '<svg class="dropdown-icon" xmlns="http://www.w3.org/2000/svg" fill="currentColor" viewBox="0 0 16 16"><path d="m7.646 9.354-3.792 3.792a.5.5 0 0 0 .353.854h7.586a.5.5 0 0 0 .354-.854L8.354 9.354a.5.5 0 0 0-.708 0"/><path d="M11.414 11H14.5a.5.5 0 0 0 .5-.5v-7a.5.5 0 0 0-.5-.5h-13a.5.5 0 0 0-.5.5v7a.5.5 0 0 0 .5.5h3.086l-1 1H1.5A1.5 1.5 0 0 1 0 10.5v-7A1.5 1.5 0 0 1 1.5 2h13A1.5 1.5 0 0 1 16 3.5v7a1.5 1.5 0 0 1-1.5 1.5h-2.086z"/></svg>'
};

// Initialize when DOM is ready
document.addEventListener('DOMContentLoaded', () => {
  initializeElements();
  setupEventListeners();
  initializeColorPicker();
  fetchInitialState();
  fetchSettings();
});

// Cache DOM elements for better performance
function initializeElements() {
  elements.loader = document.getElementById('loader');
  elements.effects = document.querySelectorAll(SELECTORS.EFFECT_BTN);
  elements.dropdownToggle = document.querySelector(SELECTORS.DROPDOWN_TOGGLE);
  elements.effectButtons = document.querySelectorAll(SELECTORS.EFFECT_BTN);
  elements.effectDropdown = document.querySelector(SELECTORS.EFFECT_DROPDOWN);
  elements.popupBox = document.getElementById('popupBox');
  elements.popupCloseBtn = document.getElementById('popupCloseBtn');
  elements.popupTitle = document.getElementById('popupTitle');
  elements.popupDesc = document.getElementById('popupDesc');
  elements.swatchGrid = document.getElementById('swatch-grid');
  elements.notification = document.getElementById('notification');
  elements.effectSpeedSlider = document.getElementById('effect-speed');
  elements.effectSpeedValue = document.getElementById('effect-speed-value');
}

// Setup all event listeners
function setupEventListeners() {
  // Effect buttons
  elements.effects.forEach(btn => {
    btn.addEventListener('click', handleEffectClick);
  });

  // Dropdown toggle
  elements.dropdownToggle.addEventListener('click', handleDropdownToggle);

  // Effect dropdown buttons
  elements.effectButtons.forEach(btn => {
    btn.addEventListener('click', handleEffectDropdownClick);
  });

  // Close dropdown when clicking outside
  document.addEventListener('click', closeDropdown);

  // Popup close button
  elements.popupCloseBtn.addEventListener('click', closePopup);

  // Swatch grid
  elements.swatchGrid.addEventListener('click', handleSwatchClick);

  // Effect speed slider
  if (elements.effectSpeedSlider) {
    elements.effectSpeedSlider.addEventListener('input', handleEffectSpeedChange);
    elements.effectSpeedSlider.addEventListener('change', handleEffectSpeedChange);
  }

  // Notification swipe events
  if (elements.notification) {
    elements.notification.addEventListener('touchstart', handleTouchStart, { passive: false });
    elements.notification.addEventListener('touchmove', handleTouchMove, { passive: false });
    elements.notification.addEventListener('touchend', handleTouchEnd, { passive: false });
  }
}

// Initialize color picker
function initializeColorPicker() {
  colorPicker = new iro.ColorPicker("#color-picker-ui", {
    width: 200,
    color: "#ff0000",
  });

  // Only send color when user releases the picker
  colorPicker.on('input:end', handleColorChange);
  window.colorPicker = colorPicker;
}

// Generic fetch function with error handling
async function fetchWithErrorHandling(url, errorType) {
  try {
    const res = await fetch(url);
    if (!res.ok) throw new Error(res.status);
    return res;
  } catch (err) {
    console.error(`${errorType} failed: ${err.message}`);
    showNotification(`${errorType} failed: ${err.message}`, true, `${errorType} Error`);
    throw err;
  }
}

// Fetch initial LED state
async function fetchInitialState() {
  try {
    const res = await fetchWithErrorHandling('/getState', 'Fetch initial LED state');
    const data = await res.json();
    
    if (data.color && colorPicker) {
      colorPicker.color.set(data.color);
    }

    if (data.effect) {
      setActiveEffect(data.effect);
      updateQuickActionButtonsFromEffect(data.effect);
    }
  } catch (err) {
    // Error already handled by fetchWithErrorHandling
  }
}


// Handle effect button clicks
function handleEffectClick() {
  elements.effects.forEach(b => b.classList.remove('active'));
  this.classList.add('active');

  const selectedEffect = this.dataset.effect;
  
  // Show popup for X-lights effect
  if (selectedEffect.toLowerCase().includes('x-lights')) {
    openPopup(
      'X-Lights/DDP Mode Active',
      'Full strip control mode - disables other functions. Waiting for data...'
    );
  } else {
    closePopup();
  }

  sendEffect(selectedEffect);
}

// Handle dropdown toggle
function handleDropdownToggle(e) {
  e.stopPropagation();
  const isOpen = elements.effectDropdown.classList.toggle('open');
  elements.dropdownToggle.setAttribute('aria-expanded', isOpen);
}

// Handle effect dropdown clicks
function handleEffectDropdownClick() {
  elements.effectButtons.forEach(b => b.classList.remove('active'));
  this.classList.add('active');

  const effect = this.getAttribute('data-effect');
  updateDropdownToggle(effect);
  closeDropdown();
}

// Close dropdown
function closeDropdown() {
  elements.effectDropdown.classList.remove('open');
  elements.dropdownToggle.setAttribute('aria-expanded', false);
}

// Handle color change
function handleColorChange(color) {
  const hex = color.hexString;
  console.log('Selected color hex:', hex);
  sendColor(hex);
}

// Handle effect speed change
function handleEffectSpeedChange() {
  const speed = elements.effectSpeedSlider.value;
  
  // Update the display value
  if (elements.effectSpeedValue) {
    elements.effectSpeedValue.textContent = speed;
  }
  
  // Send the speed to the server
  sendEffectSpeed(speed);
}

// Handle swatch clicks
function handleSwatchClick(e) {
  if (e.target.classList.contains("swatch")) {
    const color = e.target.dataset.color;
    if (color && colorPicker) {
      colorPicker.color.set(color);
      // Send color immediately for swatch clicks
      sendColor(color);
    }
  }
}

// Send effect to server
async function sendEffect(effect) {
  console.log('Selected effect:', effect);
  try {
    await fetchWithErrorHandling(`/setEffect?effect=${encodeURIComponent(effect)}`, 'Effect send');
    console.log(`Effect sent: ${effect}`);
  } catch (err) {
    // Error already handled by fetchWithErrorHandling
  }
}

// Send color to server
async function sendColor(color) {
  try {
    await fetchWithErrorHandling(`/setColor?color=${encodeURIComponent(color)}`, 'Color send');
    console.log(`Color sent: ${color}`);
  } catch (err) {
    // Error already handled by fetchWithErrorHandling
  }
}

// Send effect speed to server
async function sendEffectSpeed(speed) {
  try {
    await fetchWithErrorHandling(`/setEffectSpeed?speed=${encodeURIComponent(speed)}`, 'Effect speed send');
    console.log(`Effect speed sent: ${speed}`);
  } catch (err) {
    // Error already handled by fetchWithErrorHandling
  }
}

// Set active effect
function setActiveEffect(effect) {
  const activeBtn = document.querySelector(`${SELECTORS.EFFECT_BTN}[data-effect="${effect}"]`);
  if (activeBtn) {
    activeBtn.classList.add('active');
    updateDropdownToggle(effect);
  }
}

// Update dropdown toggle with icon and text
function updateDropdownToggle(effect) {
  const icon = EFFECT_ICONS[effect] || EFFECT_ICONS['Solid'];
  const effectName = effect === 'X-lights' ? 'X-Lights/DDP' : effect;
  
  elements.dropdownToggle.innerHTML = `
    ${icon}
    <span class="dropdown-text">${effectName}</span>
    <span class="arrow">▼</span>
  `;
}

// Popup functions
function openPopup(title, desc) {
  // Add X-lights icon to the title if it's the X-lights popup
  if (title.includes('X-Lights/DDP')) {
    elements.popupTitle.innerHTML = `
      <svg class="popup-icon" xmlns="http://www.w3.org/2000/svg" fill="currentColor" viewBox="0 0 16 16">
        <path d="m7.646 9.354-3.792 3.792a.5.5 0 0 0 .353.854h7.586a.5.5 0 0 0 .354-.854L8.354 9.354a.5.5 0 0 0-.708 0"/>
        <path d="M11.414 11H14.5a.5.5 0 0 0 .5-.5v-7a.5.5 0 0 0-.5-.5h-13a.5.5 0 0 0-.5.5v7a.5.5 0 0 0 .5.5h3.086l-1 1H1.5A1.5 1.5 0 0 1 0 10.5v-7A1.5 1.5 0 0 1 1.5 2h13A1.5 1.5 0 0 1 16 3.5v7a1.5 1.5 0 0 1-1.5 1.5h-2.086z"/>
      </svg>
      ${title}
    `;
  } else {
    elements.popupTitle.textContent = title;
  }
  elements.popupDesc.textContent = desc;
  elements.popupBox.classList.add('visible');
  elements.popupBox.setAttribute('aria-hidden', 'false');
}

function closePopup() {
  elements.popupBox.classList.remove('visible');
  elements.popupBox.setAttribute('aria-hidden', 'true');
}

// Show notification
function showNotification(message, isError = false, title = null) {
  if (!isError || !elements.notification) return;
  
  const notificationContent = title 
    ? `
      <svg class="notification-icon" xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-exclamation-triangle-fill" viewBox="0 0 16 16">
        <path d="M8.982 1.566a1.13 1.13 0 0 0-1.96 0L.165 13.233c-.457.778.091 1.767.98 1.767h13.713c.889 0 1.438-.99.98-1.767zM8 5c.535 0 .954.462.9.995l-.35 3.507a.552.552 0 0 1-1.1 0L7.1 5.995A.905.905 0 0 1 8 5m.002 6a1 1 0 1 1 0 2 1 1 0 0 1 0-2"/>
      </svg>
      <div class="notification-content">
        <div class="notification-title">${title}</div>
        <div class="notification-text">${message}</div>
      </div>
    `
    : `
      <svg class="notification-icon" xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-exclamation-triangle-fill" viewBox="0 0 16 16">
        <path d="M8.982 1.566a1.13 1.13 0 0 0-1.96 0L.165 13.233c-.457.778.091 1.767.98 1.767h13.713c.889 0 1.438-.99.98-1.767zM8 5c.535 0 .954.462.9.995l-.35 3.507a.552.552 0 0 1-1.1 0L7.1 5.995A.905.905 0 0 1 8 5m.002 6a1 1 0 1 1 0 2 1 1 0 0 1 0-2"/>
      </svg>
      <span class="notification-text">${message}</span>
    `;
  
  elements.notification.innerHTML = notificationContent;
  elements.notification.classList.add('show', 'error');
  
  // Reset transform for swipe functionality
  elements.notification.style.transform = 'translateY(0)';
  
  if (elements.notification.timeoutId) {
    clearTimeout(elements.notification.timeoutId);
  }
  
  elements.notification.timeoutId = setTimeout(() => {
    dismissNotification();
  }, 3500);
}

// Dismiss notification
function dismissNotification() {
  if (!elements.notification) return;
  elements.notification.classList.remove('show');
}

// Swipe functionality
let touchStartX = 0;
let touchStartY = 0;
let touchEndX = 0;
let touchEndY = 0;

function handleTouchStart(e) {
  touchStartX = e.changedTouches[0].screenX;
  touchStartY = e.changedTouches[0].screenY;
}

function handleTouchMove(e) {
  if (!elements.notification || !elements.notification.classList.contains('show')) return;
  
  touchEndX = e.changedTouches[0].screenX;
  touchEndY = e.changedTouches[0].screenY;
  
  const deltaX = touchEndX - touchStartX;
  const deltaY = touchEndY - touchStartY;
  
  // Only allow horizontal swipes with minimal vertical movement
  if (Math.abs(deltaX) > Math.abs(deltaY) && Math.abs(deltaY) < 50) {
    e.preventDefault();
    
    // Limit swipe distance and add resistance
    const maxSwipe = 200;
    const swipeDistance = Math.max(-maxSwipe, Math.min(maxSwipe, deltaX));
    const opacity = Math.max(0.3, 1 - Math.abs(swipeDistance) / maxSwipe);
    
    elements.notification.style.transform = `translateX(${swipeDistance}px)`;
    elements.notification.style.opacity = opacity;
  }
}

function handleTouchEnd(e) {
  if (!elements.notification || !elements.notification.classList.contains('show')) return;
  
  const deltaX = touchEndX - touchStartX;
  const deltaY = touchEndY - touchStartY;
  
  // Check if it's a valid horizontal swipe
  if (Math.abs(deltaX) > Math.abs(deltaY) && Math.abs(deltaY) < 50) {
    // If swiped more than 100px, dismiss the notification
    if (Math.abs(deltaX) > 100) {
      dismissNotification();
    } else {
      // Reset position if not swiped enough
      elements.notification.style.transform = 'translateY(0)';
      elements.notification.style.opacity = '1';
    }
  }
}

// Quick action function
async function quickAction(action) {
  let effect = '';
  let color = '';
  
  switch(action) {
    case 'off':
      effect = 'Solid';
      color = '#000000';
      if (colorPicker && colorPicker.color.hexString !== '#000000') {
        lastColor = colorPicker.color.hexString;
      }
      break;
    case 'solid':
      effect = 'Solid';
      color = lastColor;
      break;
    case 'rainbow':
      effect = 'Rainbow';
      break;
    case 'hazard':
      effect = 'Hazard';
      break;
  }
  
  // Set effect first, then color
  if (effect) {
    try {
      await fetchWithErrorHandling(`/setEffect?effect=${encodeURIComponent(effect)}`, 'Quick action effect');
      console.log(`Quick action effect sent: ${effect}`);
      
      if (color) {
        await fetchWithErrorHandling(`/setColor?color=${encodeURIComponent(color)}`, 'Quick action color');
        console.log(`Quick action color sent: ${color}`);
      }
    } catch (err) {
      // Error already handled by fetchWithErrorHandling
    }
  }
  
  // Update UI
  const activeBtn = document.querySelector(`${SELECTORS.EFFECT_BTN}[data-effect="${effect}"]`);
  if (activeBtn) {
    document.querySelectorAll(SELECTORS.EFFECT_BTN).forEach(b => b.classList.remove('active'));
    activeBtn.classList.add('active');
    updateDropdownToggle(effect);
  }
  
  updateQuickActionButtons(action);
  
  // Update color picker
  if (colorPicker && (action === 'off' || action === 'solid')) {
    colorPicker.color.set(color);
  }
}

// Update quick action button states
function updateQuickActionButtons(activeAction) {
  document.querySelectorAll(SELECTORS.ACTION_BTN).forEach(btn => {
    btn.classList.remove('active');
  });
  
  const activeBtn = document.querySelector(`${SELECTORS.ACTION_BTN}[onclick*="${activeAction}"]`);
  if (activeBtn) {
    activeBtn.classList.add('active');
  }
}

// Update quick action buttons based on current effect
function updateQuickActionButtonsFromEffect(effect) {
  let action = '';
  
  switch(effect.toLowerCase()) {
    case 'solid':
      if (colorPicker && colorPicker.color.hexString === '#000000') {
        action = 'off';
      } else {
        action = 'solid';
      }
      break;
    case 'rainbow':
      action = 'rainbow';
      break;
    case 'hazard':
      action = 'hazard';
      break;
    default:
      return;
  }
  
  updateQuickActionButtons(action);
}

// Hide loader when page is loaded
window.addEventListener("load", () => {
  if (elements.loader) {
    elements.loader.style.opacity = 0;
    elements.loader.style.transition = "opacity 0.5s ease";
    setTimeout(() => {
      elements.loader.style.display = "none";
    }, 500);
  }
});

