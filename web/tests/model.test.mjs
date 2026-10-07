import test from 'node:test';
import assert from 'node:assert/strict';
import { HanoiModel, solve, duration } from '../model.mjs';
for (let n = 1; n <= 16; n++) {
  test(`${n} disks: legal shortest solution and full reverse`, () => {
    const model = new HanoiModel(n);
    assert.equal(model.total, 2 ** n - 1);
    assert.equal(model.previous(), false);
    for (const move of model.moves) {
      const from = model.towers[move.from], to = model.towers[move.to];
      assert.equal(from.at(-1), move.disk);
      assert.ok(!to.length || to.at(-1) > move.disk);
      assert.equal(model.next(), true);
    }
    assert.deepEqual(model.towers, [[], [], Array.from({length:n},(_,i)=>n-i)]);
    assert.equal(model.next(), false);
    while (model.previous()) {}
    assert.deepEqual(model.towers, [Array.from({length:n},(_,i)=>n-i), [], []]);
  });
}
test('seek matches sequential playback in both directions', () => {
  const model = new HanoiModel(16);
  for (const step of [65535, 120, 32768, 0, 65534, 1]) {
    const reference = new HanoiModel(16);
    for (let i = 0; i < step; i++) reference.next();
    model.seek(step);
    assert.deepEqual(model.towers, reference.towers);
    assert.deepEqual(model.currentMove, reference.currentMove);
  }
  model.seek(-100); assert.equal(model.step,0);
  model.seek(99999); assert.equal(model.step,model.total);
});
test('invalid disk inputs and reset', () => {
  for (const n of [0,17,-1,1.5,NaN,Infinity,'3']) assert.throws(()=>solve(n),RangeError);
  const model = new HanoiModel(3); model.seek(7); model.reset(1);
  assert.equal(model.step,0); assert.deepEqual(model.towers,[[1],[],[]]);
  assert.throws(()=>model.seek(NaN),RangeError);
});
test('duration handles subsecond moves and long solutions', () => {
  assert.equal(duration(0),'0s'); assert.equal(duration(50),'1s');
  assert.equal(duration(65535 * 1000),'18h 12m 15s');
});
