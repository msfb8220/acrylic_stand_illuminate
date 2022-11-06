#include "Arduino.h"

#define DELAY(wait) if (IsWait(wait)){ return; }

// 指定された時間が経過すると 0 を返す
int IsWait(int wait);

#define TACT_INPUT_PIN 8  // タクトスイッチからの入力を8番ピンに割り当て

// タクトスイッチが押された時 1 を返す
bool IsSwitchPressed();
