#ifndef UI_H
#define UI_H

// Serve your custom captive portal HTML here
const char* captivePortalHTML = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Scooter LED Control</title>
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

.effects-bar {
  display: flex;
  flex-direction: column;
  gap: 0.5rem;
}

.effects-list {
  display: flex;
  flex-direction: column; /* stacked buttons */
  gap: 0.7rem;
}

.effect-btn {
  width: 100%; /* full width buttons */
  padding: 0.6rem 0;
  background: #2a2a2a;
  border: none;
  border-radius: 6px;
  color: #f1f1f1;
  font-weight: 600;
  font-size: 0.95rem;
  cursor: pointer;
  transition: background 0.3s;
  text-transform: none;
  user-select: none;
}

.effect-btn:hover,
.effect-btn.active {
  background: #4db8ff;
  color: #121212;
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

form {
  display: flex;
  flex-direction: column;
  gap: 1.4rem;
}

.bottom-nav {
  position: fixed;
  bottom: 0;
  left: 0;
  right: 0;
  height: 48px;
  border-top-left-radius: 6px;
  border-top-right-radius: 6px;
  background: #2a2a2a;
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 0 1rem;
  font-size: 1.5rem;
  color: #f1f1f1;
  z-index: 1000;
}

.nav-btn {
  background: none;
  border: none;
  color: inherit;
  cursor: pointer;
  font-size: 1.8rem;
  padding: 0;
  display: flex;
  align-items: center;
  justify-content: center;
}
.settings-btn {
  font-size: 2.5rem;
}

.nav-title {
  font-weight: 600;
  font-size: 1rem;
  user-select: none;
}

.effect-dropdown {
  position: relative;
  user-select: none;
}

.dropdown-toggle {
  width: 100%;
  padding: 0.6rem 0.8rem; /* added some right padding for arrow */
  background: #2a2a2a;
  border: none;
  border-radius: 6px;
  color: #f1f1f1;
  font-weight: 600;
  font-size: 0.95rem;
  cursor: pointer;
  transition: background 0.3s;

  display: flex;
  justify-content: center; /* center text + arrow horizontally */
  align-items: center;
  gap: 0.5rem; /* space between text and arrow */
  text-align: center;
}

.dropdown-toggle:hover,
.dropdown-toggle.active {
  background: #4db8ff;
  color: #121212;
}

.arrow {
  display: inline-block;
  transition: transform 0.3s ease;
  font-size: 1rem;
  margin-right: 0.4rem; /* a bit of space to the right */
}

.effect-dropdown.open .arrow {
  transform: rotate(180deg);
}
.dropdown-list {
  display: flex;
  flex-direction: column;
  gap: 0.7rem;
  margin-top: 0.5rem;
  background: #2a2a2a;
  border-radius: 6px;
  padding: 0.5rem 0;
  position: absolute;
  width: 100%;
  z-index: 999;

  /* Slide/fade animation setup */
  max-height: 0;
  opacity: 0;
  overflow: hidden;
  pointer-events: none;
  transition: max-height 0.2s ease, opacity 0.2s ease;
}

.effect-dropdown.open .dropdown-list {
  max-height: 500px;
  opacity: 1;
  pointer-events: auto;
}

.btn {
  border: none; /* Remove borders */
  color: white; /* Add a text color */
  padding: 14px 28px; /* Add some padding */
  cursor: pointer; /* Add a pointer cursor on mouse-over */
  border-radius: 6px;
}

.swatchGrid {
  display: flex;
  gap: 8px; /* a bit more space */
  flex-wrap: wrap;
  max-width: 400px;
  justify-content: center; /* dead center horizontally */
  margin: 0 auto; /* centre the whole block */
}

#color-picker-ui {
  display: flex; /* make it a flex container */
  justify-content: center; /* centre content horizontally */
  margin: 1rem auto; /* vertical spacing + centre block */
  max-width: 400px; /* limit width to your palettes width */
  width: 100%; /* be responsive */
}

.swatch {
  width: 28px;
  height: 28px;
  border-radius: 50%;
  cursor: pointer;
  border: 1px solid transparent;
  transition: border-color 0.2s;
}
.swatch:hover {
  border-color: #fff;
}

section {
  margin-bottom: 2rem; /* space between sections */
}

  </style>
</head>
<body>

  <div id="loader" class="loader">
    <div class="spinner">
      <div class="spinnerin"></div>
    </div>
  </div>
  <nav class="bottom-nav">

    <button class="nav-btn power-btn" title="Power Toggle">⏻</button>
    <div class="nav-title">Main</div>
    <button class="nav-btn settings-btn" title="Settings" onclick="window.location.href='/settings'">⚙</button>

  </nav>

  <main>

    <section class="color-section">
      <label for="color-picker">LED Colour</label>
      <div id="color-picker-ui"></div>
      <br>
      <div class="swatchGrid" id="swatch-grid">
        <div class="swatch" data-color="#ff0000" style="background:#ff0000;"></div>
        <div class="swatch" data-color="#00FF00" style="background:#00FF00;"></div>
        <div class="swatch" data-color="#0000FF" style="background:#0000FF;"></div>
        <div class="swatch" data-color="#00FFFF" style="background:#00FFFF;"></div>
        <div class="swatch" data-color="#FF00FF" style="background:#FF00FF;"></div>
        <div class="swatch" data-color="#ffffff" style="background:#ffffff;"></div>
      </div>

    </section>

    <section class="effects-section">

      <h2>Effects</h2>

      <div class="effects-bar">
        <div class="effect-dropdown">
          <button class="dropdown-toggle" aria-haspopup="true" aria-expanded="false">
            Select Effect <span class="arrow">▼</span>
          </button>

          <div class="dropdown-list" role="menu" aria-label="Effects List">
            <button class="effect-btn" data-effect="Solid" role="menuitem">Solid</button>
            <button class="effect-btn" data-effect="Rainbow" role="menuitem">Rainbow</button>
            <button class="effect-btn" data-effect="Pulse" role="menuitem">Pulse</button>
            <button class="effect-btn" data-effect="Sparkle" role="menuitem">Sparkle</button>
            <button class="effect-btn" data-effect="Chase" role="menuitem">Chase</button>
          </div>
        </div>

        <label for="effect-speed">Effect Speed</label>
        <input type="range" id="effect-speed" min="1" max="100" value="40" />

        <br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>
      </div>

    </section>


  </main>

  <script>
document.addEventListener('DOMContentLoaded', () => {
  const effects = document.querySelectorAll('.effect-btn');
  const dropdownToggle = document.querySelector('.dropdown-toggle');
  const effectButtons = document.querySelectorAll('.effect-btn');
  const effectDropdown = document.querySelector('.effect-dropdown');

  // Fetch current LED state on load
  fetch('/getState')
    .then(res => res.json())
    .then(data => {
      // Set initial colour
      if (data.color) {
        colorPicker.color.set(data.color);
      }

      // Set active effect button
      if (data.effect) {
        const activeBtn = document.querySelector(`.effect-btn[data-effect="${data.effect}"]`);
        if (activeBtn) {
          activeBtn.classList.add('active');
          dropdownToggle.innerHTML = `${data.effect} <span class="arrow">▼</span>`;
        }
      }
    }).catch(err => {
      console.error('Failed to fetch initial LED state:', err);
    });

  effects.forEach(btn => {
    btn.addEventListener('click', () => {
      effects.forEach(b => b.classList.remove('active'));
      btn.classList.add('active');

      const selectedEffect = btn.dataset.effect;
      console.log('Selected effect:', selectedEffect);
      fetch(`/setEffect?effect=${encodeURIComponent(selectedEffect)}`)
        .then(res => res.ok ? console.log(`Effect sent: ${selectedEffect}`) : console.error('Effect send failed'))
        .catch(err => console.error('Fetch error:', err));
    });
  });

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
    const hex = color.hexString;
    console.log('Selected color hex:', hex);
    fetch(`/setColor?color=${encodeURIComponent(hex)}`)
      .then(res => res.ok ? console.log(`Color sent: ${hex}`) : console.error('Color send failed'))
      .catch(err => console.error('Fetch error:', err));
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


/*!
 * iro.js v5.5.2
 * 2016-2021 James Daniel
 * Licensed under MPL 2.0
 * github.com/jaames/iro.js
 */
! function(t, n) {
  "object" == typeof exports && "undefined" != typeof module ? module.exports = n() : "function" == typeof define && define.amd ? define(n) : (t = t || self).iro = n()
}(this, function() {
  "use strict";
  var m, s, n, i, o, x = {},
    j = [],
    r = /acit|ex(?:s|g|n|p|$)|rph|grid|ows|mnc|ntw|ine[ch]|zoo|^ord|^--/i;

  function M(t, n) {
    for (var i in n) t[i] = n[i];
    return t
  }

  function y(t) {
    var n = t.parentNode;
    n && n.removeChild(t)
  }

  function h(t, n, i) {
    var r, e, u, o, l = arguments;
    if (n = M({}, n), 3 < arguments.length)
      for (i = [i], r = 3; r < arguments.length; r++) i.push(l[r]);
    if (null != i && (n.children = i), null != t && null != t.defaultProps)
      for (e in t.defaultProps) void 0 === n[e] && (n[e] = t.defaultProps[e]);
    return o = n.key, null != (u = n.ref) && delete n.ref, null != o && delete n.key, c(t, n, o, u)
  }

  function c(t, n, i, r) {
    var e = {
      type: t,
      props: n,
      key: i,
      ref: r,
      n: null,
      i: null,
      e: 0,
      o: null,
      l: null,
      c: null,
      constructor: void 0
    };
    return m.vnode && m.vnode(e), e
  }

  function O(t) {
    return t.children
  }

  function I(t, n) {
    this.props = t, this.context = n
  }

  function w(t, n) {
    if (null == n) return t.i ? w(t.i, t.i.n.indexOf(t) + 1) : null;
    for (var i; n < t.n.length; n++)
      if (null != (i = t.n[n]) && null != i.o) return i.o;
    return "function" == typeof t.type ? w(t) : null
  }

  function a(t) {
    var n, i;
    if (null != (t = t.i) && null != t.c) {
      for (t.o = t.c.base = null, n = 0; n < t.n.length; n++)
        if (null != (i = t.n[n]) && null != i.o) {
          t.o = t.c.base = i.o;
          break
        } return a(t)
    }
  }

  function e(t) {
    (!t.f && (t.f = !0) && 1 === s.push(t) || i !== m.debounceRendering) && (i = m.debounceRendering, (m.debounceRendering || n)(u))
  }

  function u() {
    var t, n, i, r, e, u, o, l;
    for (s.sort(function(t, n) {
        return n.d.e - t.d.e
      }); t = s.pop();) t.f && (r = i = void 0, u = (e = (n = t).d).o, o = n.p, l = n.u, n.u = !1, o && (i = [], r = k(o, e, M({}, e), n.w, void 0 !== o.ownerSVGElement, null, i, l, null == u ? w(e) : u), d(i, e), r != u && a(e)))
  }

  function S(n, i, t, r, e, u, o, l, s) {
    var c, a, f, h, v, d, g, b = t && t.n || j,
      p = b.length;
    if (l == x && (l = null != u ? u[0] : p ? w(t, 0) : null), c = 0, i.n = A(i.n, function(t) {
        if (null != t) {
          if (t.i = i, t.e = i.e + 1, null === (f = b[c]) || f && t.key == f.key && t.type === f.type) b[c] = void 0;
          else
            for (a = 0; a < p; a++) {
              if ((f = b[a]) && t.key == f.key && t.type === f.type) {
                b[a] = void 0;
                break
              }
              f = null
            }
          if (h = k(n, t, f = f || x, r, e, u, o, null, l, s), (a = t.ref) && f.ref != a && (g = g || []).push(a, t.c || h, t), null != h) {
            if (null == d && (d = h), null != t.l) h = t.l, t.l = null;
            else if (u == f || h != l || null == h.parentNode) {
              t: if (null == l || l.parentNode !== n) n.appendChild(h);
                else {
                  for (v = l, a = 0;
                    (v = v.nextSibling) && a < p; a += 2)
                    if (v == h) break t;
                  n.insertBefore(h, l)
                }
              "option" == i.type && (n.value = "")
            }
            l = h.nextSibling, "function" == typeof i.type && (i.l = h)
          }
        }
        return c++, t
      }), i.o = d, null != u && "function" != typeof i.type)
      for (c = u.length; c--;) null != u[c] && y(u[c]);
    for (c = p; c--;) null != b[c] && N(b[c], b[c]);
    if (g)
      for (c = 0; c < g.length; c++) E(g[c], g[++c], g[++c])
  }

  function A(t, n, i) {
    if (null == i && (i = []), null == t || "boolean" == typeof t) n && i.push(n(null));
    else if (Array.isArray(t))
      for (var r = 0; r < t.length; r++) A(t[r], n, i);
    else i.push(n ? n(function(t) {
      if (null == t || "boolean" == typeof t) return null;
      if ("string" == typeof t || "number" == typeof t) return c(null, t, null, null);
      if (null == t.o && null == t.c) return t;
      var n = c(t.type, t.props, t.key, null);
      return n.o = t.o, n
    }(t)) : t);
    return i
  }

  function f(t, n, i) {
    "-" === n[0] ? t.setProperty(n, i) : t[n] = "number" == typeof i && !1 === r.test(n) ? i + "px" : null == i ? "" : i
  }

  function R(t, n, i, r, e) {
    var u, o, l, s, c;
    if ("key" === (n = e ? "className" === n ? "class" : n : "class" === n ? "className" : n) || "children" === n);
    else if ("style" === n)
      if (u = t.style, "string" == typeof i) u.cssText = i;
      else {
        if ("string" == typeof r && (u.cssText = "", r = null), r)
          for (o in r) i && o in i || f(u, o, "");
        if (i)
          for (l in i) r && i[l] === r[l] || f(u, l, i[l])
      }
    else "o" === n[0] && "n" === n[1] ? (s = n !== (n = n.replace(/Capture$/, "")), n = ((c = n.toLowerCase()) in t ? c : n).slice(2), i ? (r || t.addEventListener(n, v, s), (t.t || (t.t = {}))[n] = i) : t.removeEventListener(n, v, s)) : "list" !== n && "tagName" !== n && "form" !== n && !e && n in t ? t[n] = null == i ? "" : i : "function" != typeof i && "dangerouslySetInnerHTML" !== n && (n !== (n = n.replace(/^xlink:?/, "")) ? null == i || !1 === i ? t.removeAttributeNS("http://www.w3.org/1999/xlink", n.toLowerCase()) : t.setAttributeNS("http://www.w3.org/1999/xlink", n.toLowerCase(), i) : null == i || !1 === i ? t.removeAttribute(n) : t.setAttribute(n, i))
  }

  function v(t) {
    return this.t[t.type](m.event ? m.event(t) : t)
  }

  function k(t, n, i, r, e, u, o, l, s, c) {
    var a, f, h, v, d, g, b, p, y, w, k = n.type;
    if (void 0 !== n.constructor) return null;
    (a = m.e) && a(n);
    try {
      t: if ("function" == typeof k) {
        if (p = n.props, y = (a = k.contextType) && r[a.c], w = a ? y ? y.props.value : a.i : r, i.c ? b = (f = n.c = i.c).i = f.k : ("prototype" in k && k.prototype.render ? n.c = f = new k(p, w) : (n.c = f = new I(p, w), f.constructor = k, f.render = z), y && y.sub(f), f.props = p, f.state || (f.state = {}), f.context = w, f.w = r, h = f.f = !0, f.m = []), null == f.j && (f.j = f.state), null != k.getDerivedStateFromProps && M(f.j == f.state ? f.j = M({}, f.j) : f.j, k.getDerivedStateFromProps(p, f.j)), h) null == k.getDerivedStateFromProps && null != f.componentWillMount && f.componentWillMount(), null != f.componentDidMount && o.push(f);
        else {
          if (null == k.getDerivedStateFromProps && null == l && null != f.componentWillReceiveProps && f.componentWillReceiveProps(p, w), !l && null != f.shouldComponentUpdate && !1 === f.shouldComponentUpdate(p, f.j, w)) {
            for (f.props = p, f.state = f.j, f.f = !1, (f.d = n).o = null != s ? s !== i.o ? s : i.o : null, n.n = i.n, a = 0; a < n.n.length; a++) n.n[a] && (n.n[a].i = n);
            break t
          }
          null != f.componentWillUpdate && f.componentWillUpdate(p, f.j, w)
        }
        for (v = f.props, d = f.state, f.context = w, f.props = p, f.state = f.j, (a = m.M) && a(n), f.f = !1, f.d = n, f.p = t, a = f.render(f.props, f.state, f.context), n.n = A(null != a && a.type == O && null == a.key ? a.props.children : a), null != f.getChildContext && (r = M(M({}, r), f.getChildContext())), h || null == f.getSnapshotBeforeUpdate || (g = f.getSnapshotBeforeUpdate(v, d)), S(t, n, i, r, e, u, o, s, c), f.base = n.o; a = f.m.pop();) f.j && (f.state = f.j), a.call(f);
        h || null == v || null == f.componentDidUpdate || f.componentDidUpdate(v, d, g), b && (f.k = f.i = null)
      } else n.o = function(t, n, i, r, e, u, o, l) {
        var s, c, a, f, h = i.props,
          v = n.props;
        if (e = "svg" === n.type || e, null == t && null != u)
          for (s = 0; s < u.length; s++)
            if (null != (c = u[s]) && (null === n.type ? 3 === c.nodeType : c.localName === n.type)) {
              t = c, u[s] = null;
              break
            } if (null == t) {
          if (null === n.type) return document.createTextNode(v);
          t = e ? document.createElementNS("http://www.w3.org/2000/svg", n.type) : document.createElement(n.type), u = null
        }
        return null === n.type ? h !== v && (null != u && (u[u.indexOf(t)] = null), t.data = v) : n !== i && (null != u && (u = j.slice.call(t.childNodes)), a = (h = i.props || x).dangerouslySetInnerHTML, f = v.dangerouslySetInnerHTML, l || (f || a) && (f && a && f.O == a.O || (t.innerHTML = f && f.O || "")), function(t, n, i, r, e) {
          var u;
          for (u in i) u in n || R(t, u, null, i[u], r);
          for (u in n) e && "function" != typeof n[u] || "value" === u || "checked" === u || i[u] === n[u] || R(t, u, n[u], i[u], r)
        }(t, v, h, e, l), n.n = n.props.children, f || S(t, n, i, r, "foreignObject" !== n.type && e, u, o, x, l), l || ("value" in v && void 0 !== v.value && v.value !== t.value && (t.value = null == v.value ? "" : v.value), "checked" in v && void 0 !== v.checked && v.checked !== t.checked && (t.checked = v.checked))), t
      }(i.o, n, i, r, e, u, o, c);
      (a = m.diffed) && a(n)
    }
    catch (t) {
      m.o(t, n, i)
    }
    return n.o
  }

  function d(t, n) {
    for (var i; i = t.pop();) try {
      i.componentDidMount()
    } catch (t) {
      m.o(t, i.d)
    }
    m.c && m.c(n)
  }

  function E(t, n, i) {
    try {
      "function" == typeof t ? t(n) : t.current = n
    } catch (t) {
      m.o(t, i)
    }
  }

  function N(t, n, i) {
    var r, e, u;
    if (m.unmount && m.unmount(t), (r = t.ref) && E(r, null, n), i || "function" == typeof t.type || (i = null != (e = t.o)), t.o = t.l = null, null != (r = t.c)) {
      if (r.componentWillUnmount) try {
        r.componentWillUnmount()
      } catch (t) {
        m.o(t, n)
      }
      r.base = r.p = null
    }
    if (r = t.n)
      for (u = 0; u < r.length; u++) r[u] && N(r[u], n, i);
    null != e && y(e)
  }

  function z(t, n, i) {
    return this.constructor(t, i)
  }

  function g(t, n) {
    for (var i = 0; i < n.length; i++) {
      var r = n[i];
      r.enumerable = r.enumerable || !1, r.configurable = !0, "value" in r && (r.writable = !0), Object.defineProperty(t, r.key, r)
    }
  }

  function b() {
    return (b = Object.assign || function(t) {
      for (var n = arguments, i = 1; i < arguments.length; i++) {
        var r = n[i];
        for (var e in r) Object.prototype.hasOwnProperty.call(r, e) && (t[e] = r[e])
      }
      return t
    }).apply(this, arguments)
  }
  m = {}, I.prototype.setState = function(t, n) {
    var i = this.j !== this.state && this.j || (this.j = M({}, this.state));
    "function" == typeof t && !(t = t(i, this.props)) || M(i, t), null != t && this.d && (this.u = !1, n && this.m.push(n), e(this))
  }, I.prototype.forceUpdate = function(t) {
    this.d && (t && this.m.push(t), this.u = !0, e(this))
  }, I.prototype.render = O, s = [], n = "function" == typeof Promise ? Promise.prototype.then.bind(Promise.resolve()) : setTimeout, i = m.debounceRendering, m.o = function(t, n, i) {
    for (var r; n = n.i;)
      if ((r = n.c) && !r.i) try {
        if (r.constructor && null != r.constructor.getDerivedStateFromError) r.setState(r.constructor.getDerivedStateFromError(t));
        else {
          if (null == r.componentDidCatch) continue;
          r.componentDidCatch(t)
        }
        return e(r.k = r)
      } catch (n) {
        t = n
      }
    throw t
  }, o = x;
  var t = "(?:[-\\+]?\\d*\\.\\d+%?)|(?:[-\\+]?\\d+%?)",
    l = "[\\s|\\(]+(" + t + ")[,|\\s]+(" + t + ")[,|\\s]+(" + t + ")\\s*\\)?",
    p = "[\\s|\\(]+(" + t + ")[,|\\s]+(" + t + ")[,|\\s]+(" + t + ")[,|\\s]+(" + t + ")\\s*\\)?",
    _ = new RegExp("rgb" + l),
    H = new RegExp("rgba" + p),
    P = new RegExp("hsl" + l),
    $ = new RegExp("hsla" + p),
    T = "^(?:#?|0x?)",
    W = "([0-9a-fA-F]{1})",
    C = "([0-9a-fA-F]{2})",
    D = new RegExp(T + W + W + W + "$"),
    F = new RegExp(T + W + W + W + W + "$"),
    L = new RegExp(T + C + C + C + "$"),
    B = new RegExp(T + C + C + C + C + "$"),
    q = Math.log,
    G = Math.round,
    Z = Math.floor;

  function J(t, n, i) {
    return Math.min(Math.max(t, n), i)
  }

  function K(t, n) {
    var i = -1 < t.indexOf("%"),
      r = parseFloat(t);
    return i ? n / 100 * r : r
  }

  function Q(t) {
    return parseInt(t, 16)
  }

  function U(t) {
    return t.toString(16).padStart(2, "0")
  }
  var V = function() {
    function l(t, n) {
      this.$ = {
        h: 0,
        s: 0,
        v: 0,
        a: 1
      }, t && this.set(t), this.onChange = n, this.initialValue = b({}, this.$)
    }
    var t = l.prototype;
    return t.set = function(t) {
        if ("string" == typeof t) /^(?:#?|0x?)[0-9a-fA-F]{3,8}$/.test(t) ? this.hexString = t : /^rgba?/.test(t) ? this.rgbString = t : /^hsla?/.test(t) && (this.hslString = t);
        else {
          if ("object" != typeof t) throw new Error("Invalid color value");
          t instanceof l ? this.hsva = t.hsva : "r" in t && "g" in t && "b" in t ? this.rgb = t : "h" in t && "s" in t && "v" in t ? this.hsv = t : "h" in t && "s" in t && "l" in t ? this.hsl = t : "kelvin" in t && (this.kelvin = t.kelvin)
        }
      }, t.setChannel = function(t, n, i) {
        var r;
        this[t] = b({}, this[t], ((r = {})[n] = i, r))
      }, t.reset = function() {
        this.hsva = this.initialValue
      }, t.clone = function() {
        return new l(this)
      }, t.unbind = function() {
        this.onChange = void 0
      }, l.hsvToRgb = function(t) {
        var n = t.h / 60,
          i = t.s / 100,
          r = t.v / 100,
          e = Z(n),
          u = n - e,
          o = r * (1 - i),
          l = r * (1 - u * i),
          s = r * (1 - (1 - u) * i),
          c = e % 6,
          a = [s, r, r, l, o, o][c],
          f = [o, o, s, r, r, l][c];
        return {
          r: J(255 * [r, l, o, o, s, r][c], 0, 255),
          g: J(255 * a, 0, 255),
          b: J(255 * f, 0, 255)
        }
      }, l.rgbToHsv = function(t) {
        var n = t.r / 255,
          i = t.g / 255,
          r = t.b / 255,
          e = Math.max(n, i, r),
          u = Math.min(n, i, r),
          o = e - u,
          l = 0,
          s = e,
          c = 0 === e ? 0 : o / e;
        switch (e) {
          case u:
            l = 0;
            break;
          case n:
            l = (i - r) / o + (i < r ? 6 : 0);
            break;
          case i:
            l = (r - n) / o + 2;
            break;
          case r:
            l = (n - i) / o + 4
        }
        return {
          h: 60 * l % 360,
          s: J(100 * c, 0, 100),
          v: J(100 * s, 0, 100)
        }
      }, l.hsvToHsl = function(t) {
        var n = t.s / 100,
          i = t.v / 100,
          r = (2 - n) * i,
          e = r <= 1 ? r : 2 - r,
          u = e < 1e-9 ? 0 : n * i / e;
        return {
          h: t.h,
          s: J(100 * u, 0, 100),
          l: J(50 * r, 0, 100)
        }
      }, l.hslToHsv = function(t) {
        var n = 2 * t.l,
          i = t.s * (n <= 100 ? n : 200 - n) / 100,
          r = n + i < 1e-9 ? 0 : 2 * i / (n + i);
        return {
          h: t.h,
          s: J(100 * r, 0, 100),
          v: J((n + i) / 2, 0, 100)
        }
      }, l.kelvinToRgb = function(t) {
        var n, i, r, e = t / 100;
        return r = e < 66 ? (n = 255, i = -155.25485562709179 - .44596950469579133 * (i = e - 2) + 104.49216199393888 * q(i), e < 20 ? 0 : .8274096064007395 * (r = e - 10) - 254.76935184120902 + 115.67994401066147 * q(r)) : (n = 351.97690566805693 + .114206453784165 * (n = e - 55) - 40.25366309332127 * q(n), i = 325.4494125711974 + .07943456536662342 * (i = e - 50) - 28.0852963507957 * q(i), 255), {
          r: J(Z(n), 0, 255),
          g: J(Z(i), 0, 255),
          b: J(Z(r), 0, 255)
        }
      }, l.rgbToKelvin = function(t) {
        for (var n, i = t.r, r = t.b, e = 2e3, u = 4e4; .4 < u - e;) {
          var o = l.kelvinToRgb(n = .5 * (u + e));
          o.b / o.r >= r / i ? u = n : e = n
        }
        return n
      },
      function(t, n, i) {
        n && g(t.prototype, n), i && g(t, i)
      }(l, [{
        key: "hsv",
        get: function() {
          var t = this.$;
          return {
            h: t.h,
            s: t.s,
            v: t.v
          }
        },
        set: function(t) {
          var n = this.$;
          if (t = b({}, n, t), this.onChange) {
            var i = {
              h: !1,
              v: !1,
              s: !1,
              a: !1
            };
            for (var r in n) i[r] = t[r] != n[r];
            this.$ = t, (i.h || i.s || i.v || i.a) && this.onChange(this, i)
          } else this.$ = t
        }
      }, {
        key: "hsva",
        get: function() {
          return b({}, this.$)
        },
        set: function(t) {
          this.hsv = t
        }
      }, {
        key: "hue",
        get: function() {
          return this.$.h
        },
        set: function(t) {
          this.hsv = {
            h: t
          }
        }
      }, {
        key: "saturation",
        get: function() {
          return this.$.s
        },
        set: function(t) {
          this.hsv = {
            s: t
          }
        }
      }, {
        key: "value",
        get: function() {
          return this.$.v
        },
        set: function(t) {
          this.hsv = {
            v: t
          }
        }
      }, {
        key: "alpha",
        get: function() {
          return this.$.a
        },
        set: function(t) {
          this.hsv = b({}, this.hsv, {
            a: t
          })
        }
      }, {
        key: "kelvin",
        get: function() {
          return l.rgbToKelvin(this.rgb)
        },
        set: function(t) {
          this.rgb = l.kelvinToRgb(t)
        }
      }, {
        key: "red",
        get: function() {
          return this.rgb.r
        },
        set: function(t) {
          this.rgb = b({}, this.rgb, {
            r: t
          })
        }
      }, {
        key: "green",
        get: function() {
          return this.rgb.g
        },
        set: function(t) {
          this.rgb = b({}, this.rgb, {
            g: t
          })
        }
      }, {
        key: "blue",
        get: function() {
          return this.rgb.b
        },
        set: function(t) {
          this.rgb = b({}, this.rgb, {
            b: t
          })
        }
      }, {
        key: "rgb",
        get: function() {
          var t = l.hsvToRgb(this.$),
            n = t.r,
            i = t.g,
            r = t.b;
          return {
            r: G(n),
            g: G(i),
            b: G(r)
          }
        },
        set: function(t) {
          this.hsv = b({}, l.rgbToHsv(t), {
            a: void 0 === t.a ? 1 : t.a
          })
        }
      }, {
        key: "rgba",
        get: function() {
          return b({}, this.rgb, {
            a: this.alpha
          })
        },
        set: function(t) {
          this.rgb = t
        }
      }, {
        key: "hsl",
        get: function() {
          var t = l.hsvToHsl(this.$),
            n = t.h,
            i = t.s,
            r = t.l;
          return {
            h: G(n),
            s: G(i),
            l: G(r)
          }
        },
        set: function(t) {
          this.hsv = b({}, l.hslToHsv(t), {
            a: void 0 === t.a ? 1 : t.a
          })
        }
      }, {
        key: "hsla",
        get: function() {
          return b({}, this.hsl, {
            a: this.alpha
          })
        },
        set: function(t) {
          this.hsl = t
        }
      }, {
        key: "rgbString",
        get: function() {
          var t = this.rgb;
          return "rgb(" + t.r + ", " + t.g + ", " + t.b + ")"
        },
        set: function(t) {
          var n, i, r, e, u = 1;
          if ((n = _.exec(t)) ? (i = K(n[1], 255), r = K(n[2], 255), e = K(n[3], 255)) : (n = H.exec(t)) && (i = K(n[1], 255), r = K(n[2], 255), e = K(n[3], 255), u = K(n[4], 1)), !n) throw new Error("Invalid rgb string");
          this.rgb = {
            r: i,
            g: r,
            b: e,
            a: u
          }
        }
      }, {
        key: "rgbaString",
        get: function() {
          var t = this.rgba;
          return "rgba(" + t.r + ", " + t.g + ", " + t.b + ", " + t.a + ")"
        },
        set: function(t) {
          this.rgbString = t
        }
      }, {
        key: "hexString",
        get: function() {
          var t = this.rgb;
          return "#" + U(t.r) + U(t.g) + U(t.b)
        },
        set: function(t) {
          var n, i, r, e, u = 255;
          if ((n = D.exec(t)) ? (i = 17 * Q(n[1]), r = 17 * Q(n[2]), e = 17 * Q(n[3])) : (n = F.exec(t)) ? (i = 17 * Q(n[1]), r = 17 * Q(n[2]), e = 17 * Q(n[3]), u = 17 * Q(n[4])) : (n = L.exec(t)) ? (i = Q(n[1]), r = Q(n[2]), e = Q(n[3])) : (n = B.exec(t)) && (i = Q(n[1]), r = Q(n[2]), e = Q(n[3]), u = Q(n[4])), !n) throw new Error("Invalid hex string");
          this.rgb = {
            r: i,
            g: r,
            b: e,
            a: u / 255
          }
        }
      }, {
        key: "hex8String",
        get: function() {
          var t = this.rgba;
          return "#" + U(t.r) + U(t.g) + U(t.b) + U(Z(255 * t.a))
        },
        set: function(t) {
          this.hexString = t
        }
      }, {
        key: "hslString",
        get: function() {
          var t = this.hsl;
          return "hsl(" + t.h + ", " + t.s + "%, " + t.l + "%)"
        },
        set: function(t) {
          var n, i, r, e, u = 1;
          if ((n = P.exec(t)) ? (i = K(n[1], 360), r = K(n[2], 100), e = K(n[3], 100)) : (n = $.exec(t)) && (i = K(n[1], 360), r = K(n[2], 100), e = K(n[3], 100), u = K(n[4], 1)), !n) throw new Error("Invalid hsl string");
          this.hsl = {
            h: i,
            s: r,
            l: e,
            a: u
          }
        }
      }, {
        key: "hslaString",
        get: function() {
          var t = this.hsla;
          return "hsla(" + t.h + ", " + t.s + "%, " + t.l + "%, " + t.a + ")"
        },
        set: function(t) {
          this.hslString = t
        }
      }]), l
  }();

  function X(t) {
    var n, i = t.width,
      r = t.sliderSize,
      e = t.borderWidth,
      u = t.handleRadius,
      o = t.padding,
      l = t.sliderShape,
      s = "horizontal" === t.layoutDirection;
    return r = null != (n = r) ? n : 2 * o + 2 * u, "circle" === l ? {
      handleStart: t.padding + t.handleRadius,
      handleRange: i - 2 * o - 2 * u,
      width: i,
      height: i,
      cx: i / 2,
      cy: i / 2,
      radius: i / 2 - e / 2
    } : {
      handleStart: r / 2,
      handleRange: i - r,
      radius: r / 2,
      x: 0,
      y: 0,
      width: s ? r : i,
      height: s ? i : r
    }
  }

  function Y(t, n) {
    var i = X(t),
      r = i.width,
      e = i.height,
      u = i.handleRange,
      o = i.handleStart,
      l = "horizontal" === t.layoutDirection,
      s = l ? r / 2 : e / 2,
      c = o + function(t, n) {
        var i = n.hsva,
          r = n.rgb;
        switch (t.sliderType) {
          case "red":
            return r.r / 2.55;
          case "green":
            return r.g / 2.55;
          case "blue":
            return r.b / 2.55;
          case "alpha":
            return 100 * i.a;
          case "kelvin":
            var e = t.minTemperature,
              u = t.maxTemperature - e,
              o = (n.kelvin - e) / u * 100;
            return Math.max(0, Math.min(o, 100));
          case "hue":
            return i.h /= 3.6;
          case "saturation":
            return i.s;
          case "value":
          default:
            return i.v
        }
      }(t, n) / 100 * u;
    return l && (c = -1 * c + u + 2 * o), {
      x: l ? s : c,
      y: l ? c : s
    }
  }
  var tt, nt = 2 * Math.PI,
    it = function(t, n) {
      return (t % n + n) % n
    },
    rt = function(t, n) {
      return Math.sqrt(t * t + n * n)
    };

  function et(t) {
    return t.width / 2 - t.padding - t.handleRadius - t.borderWidth
  }

  function ut(t) {
    var n = t.width / 2;
    return {
      width: t.width,
      radius: n - t.borderWidth,
      cx: n,
      cy: n
    }
  }

  function ot(t, n, i) {
    var r = t.wheelAngle,
      e = t.wheelDirection;
    return i && "clockwise" === e ? n = r + n : "clockwise" === e ? n = 360 - r + n : i && "anticlockwise" === e ? n = r + 180 - n : "anticlockwise" === e && (n = r - n), it(n, 360)
  }

  function lt(t, n, i) {
    var r = ut(t),
      e = r.cx,
      u = r.cy,
      o = et(t);
    n = e - n, i = u - i;
    var l = ot(t, Math.atan2(-i, -n) * (360 / nt)),
      s = Math.min(rt(n, i), o);
    return {
      h: Math.round(l),
      s: Math.round(100 / o * s)
    }
  }

  function st(t) {
    var n = t.width,
      i = t.boxHeight;
    return {
      width: n,
      height: null != i ? i : n,
      radius: t.padding + t.handleRadius
    }
  }

  function ct(t, n, i) {
    var r = st(t),
      e = r.width,
      u = r.height,
      o = r.radius,
      l = (n - o) / (e - 2 * o) * 100,
      s = (i - o) / (u - 2 * o) * 100;
    return {
      s: Math.max(0, Math.min(l, 100)),
      v: Math.max(0, Math.min(100 - s, 100))
    }
  }

  function at(t, n, i, r) {
    for (var e = 0; e < r.length; e++) {
      var u = r[e].x - n,
        o = r[e].y - i;
      if (Math.sqrt(u * u + o * o) < t.handleRadius) return e
    }
    return null
  }

  function ft(t) {
    return {
      boxSizing: "border-box",
      border: t.borderWidth + "px solid " + t.borderColor
    }
  }

  function ht(t, n, i) {
    return t + "-gradient(" + n + ", " + i.map(function(t) {
      var n = t[0];
      return t[1] + " " + n + "%"
    }).join(",") + ")"
  }

  function vt(t) {
    return "string" == typeof t ? t : t + "px"
  }
  var dt = ["mousemove", "touchmove", "mouseup", "touchend"],
    gt = function(n) {
      function t(t) {
        n.call(this, t), this.uid = (Math.random() + 1).toString(36).substring(5)
      }
      return n && (t.__proto__ = n), ((t.prototype = Object.create(n && n.prototype)).constructor = t).prototype.render = function(t) {
        var n = this.handleEvent.bind(this),
          i = {
            onMouseDown: n,
            ontouchstart: n
          },
          r = "horizontal" === t.layoutDirection,
          e = null === t.margin ? t.sliderMargin : t.margin,
          u = {
            overflow: "visible",
            display: r ? "inline-block" : "block"
          };
        return 0 < t.index && (u[r ? "marginLeft" : "marginTop"] = e), h(O, null, t.children(this.uid, i, u))
      }, t.prototype.handleEvent = function(t) {
        var n = this,
          i = this.props.onInput,
          r = this.base.getBoundingClientRect();
        t.preventDefault();
        var e = t.touches ? t.changedTouches[0] : t,
          u = e.clientX - r.left,
          o = e.clientY - r.top;
        switch (t.type) {
          case "mousedown":
          case "touchstart":
            !1 !== i(u, o, 0) && dt.forEach(function(t) {
              document.addEventListener(t, n, {
                passive: !1
              })
            });
            break;
          case "mousemove":
          case "touchmove":
            i(u, o, 1);
            break;
          case "mouseup":
          case "touchend":
            i(u, o, 2), dt.forEach(function(t) {
              document.removeEventListener(t, n, {
                passive: !1
              })
            })
        }
      }, t
    }(I);

  function bt(t) {
    var n = t.r,
      i = t.url,
      r = n,
      e = n;
    return h("svg", {
      className: "IroHandle IroHandle--" + t.index + " " + (t.isActive ? "IroHandle--isActive" : ""),
      style: {
        "-webkit-tap-highlight-color": "rgba(0, 0, 0, 0);",
        transform: "translate(" + vt(t.x) + ", " + vt(t.y) + ")",
        willChange: "transform",
        top: vt(-n),
        left: vt(-n),
        width: vt(2 * n),
        height: vt(2 * n),
        position: "absolute",
        overflow: "visible"
      }
    }, i && h("use", Object.assign({
      xlinkHref: function(t) {
        tt = tt || document.getElementsByTagName("base");
        var n = window.navigator.userAgent,
          i = /^((?!chrome|android).)*safari/i.test(n),
          r = /iPhone|iPod|iPad/i.test(n),
          e = window.location;
        return (i || r) && 0 < tt.length ? e.protocol + "//" + e.host + e.pathname + e.search + t : t
      }(i)
    }, t.props)), !i && h("circle", {
      cx: r,
      cy: e,
      r: n,
      fill: "none",
      "stroke-width": 2,
      stroke: "#000"
    }), !i && h("circle", {
      cx: r,
      cy: e,
      r: n - 2,
      fill: t.fill,
      "stroke-width": 2,
      stroke: "#fff"
    }))
  }

  function pt(e) {
    var t = e.activeIndex,
      u = void 0 !== t && t < e.colors.length ? e.colors[t] : e.color,
      n = X(e),
      r = n.width,
      o = n.height,
      l = n.radius,
      s = Y(e, u),
      c = function(t, n) {
        var i = n.hsv,
          r = n.rgb;
        switch (t.sliderType) {
          case "red":
            return [
              [0, "rgb(0," + r.g + "," + r.b + ")"],
              [100, "rgb(255," + r.g + "," + r.b + ")"]
            ];
          case "green":
            return [
              [0, "rgb(" + r.r + ",0," + r.b + ")"],
              [100, "rgb(" + r.r + ",255," + r.b + ")"]
            ];
          case "blue":
            return [
              [0, "rgb(" + r.r + "," + r.g + ",0)"],
              [100, "rgb(" + r.r + "," + r.g + ",255)"]
            ];
          case "alpha":
            return [
              [0, "rgba(" + r.r + "," + r.g + "," + r.b + ",0)"],
              [100, "rgb(" + r.r + "," + r.g + "," + r.b + ")"]
            ];
          case "kelvin":
            for (var e = [], u = t.minTemperature, o = t.maxTemperature, l = o - u, s = u, c = 0; s < o; s += l / 8, c += 1) {
              var a = V.kelvinToRgb(s),
                f = a.r,
                h = a.g,
                v = a.b;
              e.push([12.5 * c, "rgb(" + f + "," + h + "," + v + ")"])
            }
            return e;
          case "hue":
            return [
              [0, "#f00"],
              [16.666, "#ff0"],
              [33.333, "#0f0"],
              [50, "#0ff"],
              [66.666, "#00f"],
              [83.333, "#f0f"],
              [100, "#f00"]
            ];
          case "saturation":
            var d = V.hsvToHsl({
                h: i.h,
                s: 0,
                v: i.v
              }),
              g = V.hsvToHsl({
                h: i.h,
                s: 100,
                v: i.v
              });
            return [
              [0, "hsl(" + d.h + "," + d.s + "%," + d.l + "%)"],
              [100, "hsl(" + g.h + "," + g.s + "%," + g.l + "%)"]
            ];
          case "value":
          default:
            var b = V.hsvToHsl({
              h: i.h,
              s: i.s,
              v: 100
            });
            return [
              [0, "#000"],
              [100, "hsl(" + b.h + "," + b.s + "%," + b.l + "%)"]
            ]
        }
      }(e, u);
    return h(gt, Object.assign({}, e, {
      onInput: function(t, n, i) {
        var r = function(t, n, i) {
          var r, e = X(t),
            u = e.handleRange,
            o = e.handleStart;
          r = "horizontal" === t.layoutDirection ? -1 * i + u + o : n - o, r = Math.max(Math.min(r, u), 0);
          var l = Math.round(100 / u * r);
          switch (t.sliderType) {
            case "kelvin":
              var s = t.minTemperature;
              return s + l / 100 * (t.maxTemperature - s);
            case "alpha":
              return l / 100;
            case "hue":
              return 3.6 * l;
            case "red":
            case "blue":
            case "green":
              return 2.55 * l;
            default:
              return l
          }
        }(e, t, n);
        e.parent.inputActive = !0, u[e.sliderType] = r, e.onInput(i, e.id)
      }
    }), function(t, n, i) {
      return h("div", Object.assign({}, n, {
        className: "IroSlider",
        style: Object.assign({}, {
          position: "relative",
          width: vt(r),
          height: vt(o),
          borderRadius: vt(l),
          background: "conic-gradient(#ccc 25%, #fff 0 50%, #ccc 0 75%, #fff 0)",
          backgroundSize: "8px 8px"
        }, i)
      }), h("div", {
        className: "IroSliderGradient",
        style: Object.assign({}, {
          position: "absolute",
          top: 0,
          left: 0,
          width: "100%",
          height: "100%",
          borderRadius: vt(l),
          background: ht("linear", "horizontal" === e.layoutDirection ? "to top" : "to right", c)
        }, ft(e))
      }), h(bt, {
        isActive: !0,
        index: u.index,
        r: e.handleRadius,
        url: e.handleSvg,
        props: e.handleProps,
        x: s.x,
        y: s.y
      }))
    })
  }

  function yt(e) {
    var t = st(e),
      r = t.width,
      u = t.height,
      o = t.radius,
      l = e.colors,
      s = e.parent,
      n = e.activeIndex,
      c = void 0 !== n && n < e.colors.length ? e.colors[n] : e.color,
      a = function(t, n) {
        return [
          [
            [0, "#fff"],
            [100, "hsl(" + n.hue + ",100%,50%)"]
          ],
          [
            [0, "rgba(0,0,0,0)"],
            [100, "#000"]
          ]
        ]
      }(0, c),
      f = l.map(function(t) {
        return function(t, n) {
          var i = st(t),
            r = i.width,
            e = i.height,
            u = i.radius,
            o = n.hsv,
            l = u,
            s = r - 2 * u,
            c = e - 2 * u;
          return {
            x: l + o.s / 100 * s,
            y: l + (c - o.v / 100 * c)
          }
        }(e, t)
      });
    return h(gt, Object.assign({}, e, {
      onInput: function(t, n, i) {
        if (0 === i) {
          var r = at(e, t, n, f);
          null !== r ? s.setActiveColor(r) : (s.inputActive = !0, c.hsv = ct(e, t, n), e.onInput(i, e.id))
        } else 1 === i && (s.inputActive = !0, c.hsv = ct(e, t, n));
        e.onInput(i, e.id)
      }
    }), function(t, n, i) {
      return h("div", Object.assign({}, n, {
        className: "IroBox",
        style: Object.assign({}, {
          width: vt(r),
          height: vt(u),
          position: "relative"
        }, i)
      }), h("div", {
        className: "IroBox",
        style: Object.assign({}, {
          width: "100%",
          height: "100%",
          borderRadius: vt(o)
        }, ft(e), {
          background: ht("linear", "to bottom", a[1]) + "," + ht("linear", "to right", a[0])
        })
      }), l.filter(function(t) {
        return t !== c
      }).map(function(t) {
        return h(bt, {
          isActive: !1,
          index: t.index,
          fill: t.hslString,
          r: e.handleRadius,
          url: e.handleSvg,
          props: e.handleProps,
          x: f[t.index].x,
          y: f[t.index].y
        })
      }), h(bt, {
        isActive: !0,
        index: c.index,
        fill: c.hslString,
        r: e.activeHandleRadius || e.handleRadius,
        url: e.handleSvg,
        props: e.handleProps,
        x: f[c.index].x,
        y: f[c.index].y
      }))
    })
  }
  bt.defaultProps = {
    fill: "none",
    x: 0,
    y: 0,
    r: 8,
    url: null,
    props: {
      x: 0,
      y: 0
    }
  }, pt.defaultProps = Object.assign({}, {
    sliderShape: "bar",
    sliderType: "value",
    minTemperature: 2200,
    maxTemperature: 11e3
  });

  function wt(e) {
    var r = ut(e).width,
      u = e.colors,
      o = (e.borderWidth, e.parent),
      l = e.color,
      s = l.hsv,
      c = u.map(function(t) {
        return function(t, n) {
          var i = n.hsv,
            r = ut(t),
            e = r.cx,
            u = r.cy,
            o = et(t),
            l = (180 + ot(t, i.h, !0)) * (nt / 360),
            s = i.s / 100 * o,
            c = "clockwise" === t.wheelDirection ? -1 : 1;
          return {
            x: e + s * Math.cos(l) * c,
            y: u + s * Math.sin(l) * c
          }
        }(e, t)
      }),
      a = {
        position: "absolute",
        top: 0,
        left: 0,
        width: "100%",
        height: "100%",
        borderRadius: "50%",
        boxSizing: "border-box"
      };
    return h(gt, Object.assign({}, e, {
      onInput: function(t, n, i) {
        if (0 === i) {
          if (! function(t, n, i) {
              var r = ut(t),
                e = r.cx,
                u = r.cy,
                o = t.width / 2;
              return rt(e - n, u - i) < o
            }(e, t, n)) return !1;
          var r = at(e, t, n, c);
          null !== r ? o.setActiveColor(r) : (o.inputActive = !0, l.hsv = lt(e, t, n), e.onInput(i, e.id))
        } else 1 === i && (o.inputActive = !0, l.hsv = lt(e, t, n));
        e.onInput(i, e.id)
      }
    }), function(t, n, i) {
      return h("div", Object.assign({}, n, {
        className: "IroWheel",
        style: Object.assign({}, {
          width: vt(r),
          height: vt(r),
          position: "relative"
        }, i)
      }), h("div", {
        className: "IroWheelHue",
        style: Object.assign({}, a, {
          transform: "rotateZ(" + (e.wheelAngle + 90) + "deg)",
          background: "clockwise" === e.wheelDirection ? "conic-gradient(red, yellow, lime, aqua, blue, magenta, red)" : "conic-gradient(red, magenta, blue, aqua, lime, yellow, red)"
        })
      }), h("div", {
        className: "IroWheelSaturation",
        style: Object.assign({}, a, {
          background: "radial-gradient(circle closest-side, #fff, transparent)"
        })
      }), e.wheelLightness && h("div", {
        className: "IroWheelLightness",
        style: Object.assign({}, a, {
          background: "#000",
          opacity: 1 - s.v / 100
        })
      }), h("div", {
        className: "IroWheelBorder",
        style: Object.assign({}, a, ft(e))
      }), u.filter(function(t) {
        return t !== l
      }).map(function(t) {
        return h(bt, {
          isActive: !1,
          index: t.index,
          fill: t.hslString,
          r: e.handleRadius,
          url: e.handleSvg,
          props: e.handleProps,
          x: c[t.index].x,
          y: c[t.index].y
        })
      }), h(bt, {
        isActive: !0,
        index: l.index,
        fill: l.hslString,
        r: e.activeHandleRadius || e.handleRadius,
        url: e.handleSvg,
        props: e.handleProps,
        x: c[l.index].x,
        y: c[l.index].y
      }))
    })
  }
  var kt = function(i) {
    function t(t) {
      var n = this;
      i.call(this, t), this.colors = [], this.inputActive = !1, this.events = {}, this.activeEvents = {}, this.deferredEvents = {}, this.id = t.id, (0 < t.colors.length ? t.colors : [t.color]).forEach(function(t) {
        return n.addColor(t)
      }), this.setActiveColor(0), this.state = Object.assign({}, t, {
        color: this.color,
        colors: this.colors,
        layout: t.layout
      })
    }
    return i && (t.__proto__ = i), ((t.prototype = Object.create(i && i.prototype)).constructor = t).prototype.addColor = function(t, n) {
      void 0 === n && (n = this.colors.length);
      var i = new V(t, this.onColorChange.bind(this));
      this.colors.splice(n, 0, i), this.colors.forEach(function(t, n) {
        return t.index = n
      }), this.state && this.setState({
        colors: this.colors
      }), this.deferredEmit("color:init", i)
    }, t.prototype.removeColor = function(t) {
      var n = this.colors.splice(t, 1)[0];
      n.unbind(), this.colors.forEach(function(t, n) {
        return t.index = n
      }), this.state && this.setState({
        colors: this.colors
      }), n.index === this.color.index && this.setActiveColor(0), this.emit("color:remove", n)
    }, t.prototype.setActiveColor = function(t) {
      this.color = this.colors[t], this.state && this.setState({
        color: this.color
      }), this.emit("color:setActive", this.color)
    }, t.prototype.setColors = function(t, n) {
      var i = this;
      void 0 === n && (n = 0), this.colors.forEach(function(t) {
        return t.unbind()
      }), this.colors = [], t.forEach(function(t) {
        return i.addColor(t)
      }), this.setActiveColor(n), this.emit("color:setAll", this.colors)
    }, t.prototype.on = function(t, n) {
      var i = this,
        r = this.events;
      (Array.isArray(t) ? t : [t]).forEach(function(t) {
        (r[t] || (r[t] = [])).push(n), i.deferredEvents[t] && (i.deferredEvents[t].forEach(function(t) {
          n.apply(null, t)
        }), i.deferredEvents[t] = [])
      })
    }, t.prototype.off = function(t, i) {
      var r = this;
      (Array.isArray(t) ? t : [t]).forEach(function(t) {
        var n = r.events[t];
        n && n.splice(n.indexOf(i), 1)
      })
    }, t.prototype.emit = function(t) {
      for (var n = this, i = [], r = arguments.length - 1; 0 < r--;) i[r] = arguments[r + 1];
      var e = this.activeEvents;
      !!e.hasOwnProperty(t) && e[t] || (e[t] = !0, (this.events[t] || []).forEach(function(t) {
        return t.apply(n, i)
      }), e[t] = !1)
    }, t.prototype.deferredEmit = function(t) {
      for (var n, i = [], r = arguments.length - 1; 0 < r--;) i[r] = arguments[r + 1];
      var e = this.deferredEvents;
      (n = this).emit.apply(n, [t].concat(i)), (e[t] || (e[t] = [])).push(i)
    }, t.prototype.setOptions = function(t) {
      this.setState(t)
    }, t.prototype.resize = function(t) {
      this.setOptions({
        width: t
      })
    }, t.prototype.reset = function() {
      this.colors.forEach(function(t) {
        return t.reset()
      }), this.setState({
        colors: this.colors
      })
    }, t.prototype.onMount = function(t) {
      this.el = t, this.deferredEmit("mount", this)
    }, t.prototype.onColorChange = function(t, n) {
      this.setState({
        color: this.color
      }), this.inputActive && (this.inputActive = !1, this.emit("input:change", t, n)), this.emit("color:change", t, n)
    }, t.prototype.emitInputEvent = function(t, n) {
      0 === t ? this.emit("input:start", this.color, n) : 1 === t ? this.emit("input:move", this.color, n) : 2 === t && this.emit("input:end", this.color, n)
    }, t.prototype.render = function(t, e) {
      var u = this,
        n = e.layout;
      return Array.isArray(n) || (n = [{
        component: wt
      }, {
        component: pt
      }], e.transparency && n.push({
        component: pt,
        options: {
          sliderType: "alpha"
        }
      })), h("div", {
        class: "IroColorPicker",
        id: e.id,
        style: {
          display: e.display
        }
      }, n.map(function(t, n) {
        var i = t.component,
          r = t.options;
        return h(i, Object.assign({}, e, r, {
          ref: void 0,
          onInput: u.emitInputEvent.bind(u),
          parent: u,
          index: n
        }))
      }))
    }, t
  }(I);
  kt.defaultProps = Object.assign({}, {
    width: 300,
    height: 300,
    color: "#fff",
    colors: [],
    padding: 6,
    layoutDirection: "vertical",
    borderColor: "#fff",
    borderWidth: 0,
    handleRadius: 8,
    activeHandleRadius: null,
    handleSvg: null,
    handleProps: {
      x: 0,
      y: 0
    },
    wheelLightness: !0,
    wheelAngle: 0,
    wheelDirection: "anticlockwise",
    sliderSize: null,
    sliderMargin: 12,
    boxHeight: null
  }, {
    colors: [],
    display: "block",
    id: null,
    layout: "default",
    margin: null
  });
  var mt, xt, jt, Mt, Ot = (It.prototype = (mt = kt).prototype, Object.assign(It, mt), It.I = mt, It);

  function It(n, t) {
    var i, r = document.createElement("div");

    function e() {
      var t = n instanceof Element ? n : document.querySelector(n);
      t.appendChild(i.base), i.onMount(t)
    }
    return function(t, n, i) {
      var r, e, u;
      m.i && m.i(t, n), e = (r = i === o) ? null : i && i.n || n.n, t = h(O, null, [t]), u = [], k(n, r ? n.n = t : (i || n).n = t, e || x, x, void 0 !== n.ownerSVGElement, i && !r ? [i] : e ? null : j.slice.call(n.childNodes), u, !1, i || x, r), d(u, t)
    }(h(mt, Object.assign({}, {
      ref: function(t) {
        return i = t
      }
    }, t)), r), "loading" !== document.readyState ? e() : document.addEventListener("DOMContentLoaded", e), i
  }
  return (jt = xt = xt || {}).version = "5.5.2", jt.Color = V, jt.ColorPicker = Ot, (Mt = jt.ui || (jt.ui = {})).h = h, Mt.ComponentBase = gt, Mt.Handle = bt, Mt.Slider = pt, Mt.Wheel = wt, Mt.Box = yt, xt
});
  </script>

</body>
</html>


)rawliteral";
#endif
