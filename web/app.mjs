import { HanoiModel, duration } from './model.mjs';
const $ = id => document.getElementById(id);
const model = new HanoiModel(10);
const darkPreference = window.matchMedia('(prefers-color-scheme: dark)');
let timer = null;
let playing = false;
const names = ['A', 'B', 'C'];
for (let n = 1; n <= 16; n++) $('disks').add(new Option(String(n), String(n)));
$('disks').value = '10';
function readSettings() {
  try {
    const saved = JSON.parse(localStorage.getItem('hanoi-settings') || '{}');
    if (Number.isInteger(saved.disks) && saved.disks >= 1 && saved.disks <= 16) { model.reset(saved.disks); $('disks').value = String(saved.disks); }
    if (Number.isInteger(saved.speed) && saved.speed >= 50 && saved.speed <= 5000) $('speed').value = String(saved.speed);
    if (['system', 'light', 'dark'].includes(saved.theme)) $('theme').value = saved.theme;
    if (typeof saved.loop === 'boolean') $('loop').checked = saved.loop;
  } catch { /* Storage may be unavailable; the slideshow still works. */ }
}
function saveSettings() {
  try { localStorage.setItem('hanoi-settings', JSON.stringify({ disks: model.disks, speed: Number($('speed').value), theme: $('theme').value, loop: $('loop').checked })); } catch { /* Optional persistence. */ }
}
function applyTheme() {
  document.documentElement.dataset.theme = $('theme').value === 'system' ? (darkPreference.matches ? 'dark' : 'light') : $('theme').value;
}
function element(tag, attrs = {}, text = '') {
  const node = document.createElementNS('http://www.w3.org/2000/svg', tag);
  for (const [key, value] of Object.entries(attrs)) node.setAttribute(key, String(value));
  if (text) node.textContent = text;
  return node;
}
function draw() {
  const scene = $('drawing');
  scene.replaceChildren();
  const active = model.currentMove;
  const base = 365, height = Math.min(23, 265 / (model.disks + 1));
  if (active) {
    const from = 150 + active.from * 300, to = 150 + active.to * 300;
    const direction = Math.sign(to - from);
    scene.append(element('path', {d:`M ${from + direction * 30} 48 Q ${(from + to) / 2} 0 ${to - direction * 30} 48`, class:'move-arrow', 'marker-end':'url(#arrowhead)'}));
  }
  scene.append(element('rect', {x:30, y:base, width:840, height:10, rx:5, class:'platform'}));
  model.towers.forEach((tower, index) => {
    const center = 150 + 300 * index;
    scene.append(element('rect', {x:center - 4, y:87, width:8, height:278, rx:4, class:'peg'}));
    scene.append(element('text', {x:center, y:408, 'text-anchor':'middle', class:'tower-label'}, names[index]));
    tower.forEach((disk, level) => {
      const ratio = model.disks === 1 ? 1 : (disk - 1) / (model.disks - 1);
      const width = 70 + 160 * ratio;
      const hue = (disk * 37 + 205) % 360;
      scene.append(element('rect', {x:center - width/2, y:base - (level+1) * height + 1, width, height:height-2, rx:5, fill:`hsl(${hue} 43% 66%)`, class:`disk${active?.disk === disk ? ' active-disk' : ''}`}));
    });
  });
  $('board-desc').textContent = `Step ${model.step} of ${model.total}. ` + model.towers.map((tower,i) => `Tower ${names[i]}: ${tower.length ? tower.join(', ') + ' from bottom to top' : 'empty'}`).join('. ');
}
function render() {
  const active = model.currentMove;
  const speed = Number($('speed').value);
  $('move-label').textContent = active ? `Disk ${active.disk} · ${names[active.from]} → ${names[active.to]}${model.step === model.total ? ' · Complete' : ''}` : 'Ready when you are';
  $('speed-label').textContent = `${(speed / 1000).toFixed(2)} s / move`;
  $('step-label').textContent = `${model.step.toLocaleString()} / ${model.total.toLocaleString()} moves`;
  $('remaining').textContent = `${duration((model.total-model.step)*speed)} remaining`;
  $('formula').textContent = `${model.disks} ${model.disks === 1 ? 'disk' : 'disks'} → ${model.total.toLocaleString()} ${model.total === 1 ? 'move' : 'moves'}.`;
  $('progress').max = String(model.total);
  $('progress').value = String(model.step);
  $('progress').setAttribute('aria-valuetext', `Step ${model.step} of ${model.total}`);
  $('first').disabled = $('previous').disabled = model.step === 0;
  $('last').disabled = model.step === model.total;
  $('next').disabled = model.step === model.total && !$('loop').checked;
  $('play-label').textContent = playing ? 'Pause' : 'Play';
  $('play-icon').textContent = playing ? 'Ⅱ' : '▶';
  $('play').setAttribute('aria-label', playing ? 'Pause' : 'Play');
  $('play').setAttribute('aria-pressed', String(playing));
  draw();
}
function schedule() {
  clearTimeout(timer);
  timer = null;
  if (!playing || document.hidden) return;
  timer = setTimeout(() => {
    if (!model.next()) {
      if ($('loop').checked) model.seek(0);
      else playing = false;
    } else if (model.step === model.total && !$('loop').checked) playing = false;
    render();
    schedule();
  }, Number($('speed').value));
}
function setPlaying(value) {
  if (value && model.step === model.total) model.seek(0);
  playing = value;
  render();
  schedule();
}
function manual(action) { setPlaying(false); action(); render(); }
$('first').onclick = () => manual(() => model.seek(0));
$('previous').onclick = () => manual(() => model.previous());
$('next').onclick = () => manual(() => { if (!model.next() && $('loop').checked) model.seek(0); });
$('last').onclick = () => manual(() => model.seek(model.total));
$('play').onclick = () => setPlaying(!playing);
$('progress').oninput = () => manual(() => model.seek(Number($('progress').value)));
$('disks').onchange = () => { const resume = playing; model.reset(Number($('disks').value)); saveSettings(); setPlaying(resume); };
$('speed').oninput = () => { saveSettings(); render(); schedule(); };
$('theme').onchange = () => { applyTheme(); saveSettings(); };
$('loop').onchange = () => { saveSettings(); render(); };
darkPreference.addEventListener('change', applyTheme);
document.addEventListener('visibilitychange', schedule);
function closeSettings() { $('settings').hidden = true; $('settings-button').setAttribute('aria-expanded','false'); }
$('settings-button').onclick = () => { const open = $('settings').hidden; $('settings').hidden = !open; $('settings-button').setAttribute('aria-expanded', String(open)); };
document.addEventListener('click', event => { if (!$('settings').contains(event.target) && !$('settings-button').contains(event.target)) closeSettings(); });
$('fullscreen').hidden = !document.fullscreenEnabled;
$('fullscreen').onclick = async () => {
  try { if (document.fullscreenElement) await document.exitFullscreen(); else await document.querySelector('.player').requestFullscreen(); closeSettings(); } catch { $('fullscreen').textContent = 'Fullscreen unavailable'; }
};
document.addEventListener('fullscreenchange', () => { $('fullscreen').textContent = document.fullscreenElement ? 'Exit fullscreen' : 'Enter fullscreen'; });
document.addEventListener('keydown', event => {
  if (event.key === 'Escape') { closeSettings(); $('settings-button').focus(); return; }
  if (event.target.closest('input,select,button,a') || event.ctrlKey || event.metaKey || event.altKey) return;
  const action = {' ':() => setPlaying(!playing), ArrowLeft:() => $('previous').click(), ArrowRight:() => $('next').click(), Home:() => $('first').click(), End:() => $('last').click()}[event.key];
  if (action) { event.preventDefault(); action(); }
});
readSettings(); applyTheme(); render();
