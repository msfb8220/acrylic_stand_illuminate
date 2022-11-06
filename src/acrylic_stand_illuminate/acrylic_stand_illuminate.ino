#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include "util.h"
// Arduino IDE のプロトタイプ宣言の自動挿入により、typedef より前にプロトタイプ宣言がされてしまうため、ヘッダファイルで typedef を定義
// 参考: https://qiita.com/JJ1LIS/items/a1ffdc27be6cd9212d4d
#include "typedef.h"

/* setting LED */
#define LED_DATA_PIN 6
#define NUMPIXELS 8
#define BRIGHTNESS 32  // 最大輝度の設定をする(暗い←0 ～ 255→明い)
#define DELAYVAL (20 * 1000)  // [ms]


Adafruit_NeoPixel pixels(NUMPIXELS, LED_DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    /* setup LED */
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif
    pixels.begin();
    pixels.clear();

    /* setup LED */
    pinMode(TACT_INPUT_PIN, INPUT_PULLUP);  // タクトスイッチに繋いだピンを入力に設定

    /* setup SERIAL */
    Serial.begin(9600);
}

void loop()
{
    SelectOutputMode();
}


// LEDを連続的に虹色に変化させる関数
// 色の値を取得するには、0〜255の値を入力します。
// 色の移り変わりはR(赤)→G(緑)→B(青)からR(赤)に戻ります。
void
Rainbow(int wait)
{
    static uint16_t s_color_val = 0;
    uint16_t led_num;
    for (led_num = 0; led_num < pixels.numPixels(); led_num++)
    {
        uint8_t wheel_pos = s_color_val;
        if (wheel_pos < 85)
        {
            pixels.setPixelColor(led_num, (wheel_pos * 3) * BRIGHTNESS / 255, (255 - wheel_pos * 3) * BRIGHTNESS / 255, 0);
        } else if (wheel_pos < 170)
        {
            wheel_pos -= 85;
            pixels.setPixelColor(led_num, (255 - wheel_pos * 3) * BRIGHTNESS / 255, 0, (wheel_pos * 3) * BRIGHTNESS / 255);
        } else {
            wheel_pos -= 170;
            pixels.setPixelColor(led_num, 0, (wheel_pos * 3) * BRIGHTNESS / 255, (255 - wheel_pos * 3) * BRIGHTNESS / 255);
        }
    }
    DELAY(wait);
    s_color_val++;
    if (s_color_val > 255)
    {
        s_color_val = 0;
    }
    pixels.show();
}

/* solid color */
void
SetColor(uint8_t brightness, uint8_t r, uint8_t g, uint8_t b)
{
    for (uint8_t i = 0; i < pixels.numPixels(); i++)
    {
        pixels.setPixelColor(i, r * brightness / 255, g * brightness / 255, b * brightness / 255);
    }
    pixels.show();
}

void
SetRed(uint8_t brightness)
{
    SetColor(brightness, 255, 0, 0);
}

void
SetGreen(uint8_t brightness)
{
    SetColor(brightness, 0, 255, 0);
}

void
SetBlue(uint8_t brightness)
{
    SetColor(brightness, 0, 0, 255);
}


/* mode */
void
Solid(uint8_t brightness, pColorFunc color)
{
    color(brightness);
}

void
HeartBeet(int wait, pColorFunc color)
{
    static uint8_t s_brightness_val = 0;
    static int s_is_count_up = 1;
    if (s_is_count_up)
    {
        color(s_brightness_val);
        DELAY(wait);
        if (s_brightness_val >= BRIGHTNESS)
        {
            s_is_count_up = 0;
            s_brightness_val = BRIGHTNESS;
            return;
        }
        s_brightness_val++;
    }
    else
    {
        color(s_brightness_val);
        DELAY(wait);
        if (s_brightness_val <= 0)
        {
            s_is_count_up = 1;
            s_brightness_val = 0;
            return;
        }
        s_brightness_val--;
    }
}


pColorFunc SelectColor(ColorPalette color)
{
    switch (color)
    {
    case RED:
        return SetRed;
    case GREEN:
        return SetGreen;
    case BLUE:
        return SetBlue;
    default:
        pixels.clear();
        Serial.println("not supported color");
        while(1) { /* do nothing */ };
        return NULL;
    }
}

void
SelectOutputMode()
{
    const int NUM_MODE_MAX = 7;
    static int s_mode = 0;
    if (IsSwitchPressed())
    {
        s_mode++;
        if (s_mode >= NUM_MODE_MAX)
        {
            s_mode = 0;
        }
    }

    switch (s_mode)
    {
    case 0:
        Rainbow(DELAYVAL);
        break;
    case 1:
        Solid(BRIGHTNESS, SelectColor(RED));
        break;
    case 2:
        Solid(BRIGHTNESS, SelectColor(GREEN));
        break;
    case 3:
        Solid(BRIGHTNESS, SelectColor(BLUE));
        break;
    case 4:
        HeartBeet(DELAYVAL, SelectColor(RED));
        break;
    case 5:
        HeartBeet(DELAYVAL, SelectColor(GREEN));
        break;
    case 6:
        HeartBeet(DELAYVAL, SelectColor(BLUE));
        break;
    default:
        pixels.clear();
        Serial.println("not supported mode");
        while(1) { /* do nothing */ };
    }
}
