document.addEventListener('DOMContentLoaded', () => {
  const effects = document.querySelectorAll('.effect-btn');

  effects.forEach(btn => {
    btn.addEventListener('click', () => {
      effects.forEach(b => b.classList.remove('active'));
      btn.classList.add('active');

      console.log('Selected effect:', btn.dataset.effect);
    });
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

  var colorPicker = new iro.ColorPicker("#color-picker-ui", {
    width: 200,
    color: "#ff0000",
  });
  colorPicker.on('color:change', function(color) {
    console.log('Selected color hex:', color.hexString);
  });


  const swatchGrid = document.getElementById("swatch-grid");

  swatchGrid.addEventListener("click", (e) => {
    if (e.target.classList.contains("swatch")) {
      const color = e.target.dataset.color;
      if (color) {
        colorPicker.color.set(color);
      }
    }
  });
});


window.addEventListener("load", () => {
  const loader = document.getElementById("loader");
  loader.style.opacity = 0;
  loader.style.transition = "opacity 0.5s ease";
  setTimeout(() => {
    loader.style.display = "none";
  }, 500);
});
