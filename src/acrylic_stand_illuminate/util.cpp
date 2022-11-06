#include "util.h"

// プルアップなため、押されたときに 0
#define PRESS 0
#define RELEASE 1

int
IsWait(int wait)
{
    static unsigned long s_last_time = 0;
    unsigned long current_time = micros();
    int diff = (int)(current_time - s_last_time);
    if (diff > wait)
    {
        s_last_time = current_time;
        return 0;
    }
    return 1;
}

// スイッチが離されたときにステータスが変更される
bool
IsSwitchPressed()
{
    static int s_last_status = RELEASE;
    int status = digitalRead(TACT_INPUT_PIN);  // 入力ピン(タクトスイッチ)の状態を読み取る

    if (status == PRESS)
    {
        if (s_last_status == RELEASE)
        {
            s_last_status = PRESS;
            return true;
        }
        s_last_status = PRESS;
        return false;
    }
    s_last_status = RELEASE;
    return false;
}