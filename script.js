// Just UI, no API calls yet

const effects = document.querySelectorAll('.effect-btn');

effects.forEach(btn => {
  btn.addEventListener('click', () => {
    effects.forEach(b => b.classList.remove('active'));
    btn.classList.add('active');

    // Here you would do something like send effect change request later
    console.log('Selected effect:', btn.dataset.effect);
  });
});

const brightnessSlider = document.getElementById('brightness');
brightnessSlider.addEventListener('input', () => {
  // Later send brightness change request here
  console.log('Brightness:', brightnessSlider.value);
});



const dropdownToggle = document.querySelector('.dropdown-toggle');
const effectButtons = document.querySelectorAll('.effect-btn');
const effectDropdown = document.querySelector('.effect-dropdown');

dropdownToggle.addEventListener('click', e => {
  e.stopPropagation();
  const isOpen = effectDropdown.classList.toggle('open');
  dropdownToggle.setAttribute('aria-expanded', isOpen);
});

effectButtons.forEach(btn => {
  btn.addEventListener('click', () => {
    effectButtons.forEach(b => b.classList.remove('active'));
    btn.classList.add('active');

    // Update dropdown button text with current effect name
    dropdownToggle.innerHTML = `${btn.dataset.effect} <span class="arrow">▼</span>`;

    effectDropdown.classList.remove('open');
    dropdownToggle.setAttribute('aria-expanded', false);

    console.log('Selected effect:', btn.dataset.effect);
  });
});

document.addEventListener('click', () => {
  effectDropdown.classList.remove('open');
  dropdownToggle.setAttribute('aria-expanded', false);
});








function showNotification(msg, type = 'success', duration = 3000) {
  const notif = document.getElementById('notifications');
  notif.textContent = msg;
  notif.className = `notifications show ${type}`;

  setTimeout(() => {
    notif.className = 'notifications';
  }, duration);
}
