export function solve(disks) {
  if (!Number.isInteger(disks) || disks < 1 || disks > 16) throw new RangeError('Disks must be between 1 and 16');
  const moves = [];
  function visit(count, from, to, spare) {
    if (!count) return;
    visit(count - 1, from, spare, to);
    moves.push({ disk: count, from, to });
    visit(count - 1, spare, to, from);
  }
  visit(disks, 0, 2, 1);
  return moves;
}
export class HanoiModel {
  constructor(disks = 10) { this.reset(disks); }
  reset(disks) {
    this.moves = solve(disks);
    this.disks = disks;
    this.step = 0;
    this.towers = [Array.from({length: disks}, (_, i) => disks - i), [], []];
  }
  get total() { return this.moves.length; }
  get currentMove() { return this.step ? this.moves[this.step - 1] : null; }
  next() {
    if (this.step === this.total) return false;
    const move = this.moves[this.step++];
    this.towers[move.to].push(this.towers[move.from].pop());
    return true;
  }
  previous() {
    if (!this.step) return false;
    const move = this.moves[--this.step];
    this.towers[move.from].push(this.towers[move.to].pop());
    return true;
  }
  seek(step) {
    if (!Number.isInteger(step)) throw new RangeError('Step must be an integer');
    const target = Math.max(0, Math.min(step, this.total));
    while (this.step > target) this.previous();
    while (this.step < target) this.next();
  }
}
export function duration(ms) {
  const seconds = Math.ceil(ms / 1000);
  const h = Math.floor(seconds / 3600), m = Math.floor(seconds % 3600 / 60), s = seconds % 60;
  return h ? `${h}h ${m}m ${s}s` : m ? `${m}m ${s}s` : `${s}s`;
}
