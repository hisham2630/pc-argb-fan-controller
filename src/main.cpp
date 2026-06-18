#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    16   // set to actual LED count on your fan
#define FAN_PWM_PIN 10
#define POT_PIN     A0

CRGB leds[NUM_LEDS];

void setup() {
  // Timer1 -> 25 kHz Fast PWM on OC1B (pin 10), TOP = 639
  pinMode(FAN_PWM_PIN, OUTPUT);
  TCCR1A = (1 << COM1B1) | (1 << WGM11);
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);
  ICR1 = 639;

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(150);
}

void loop() {
  uint16_t pot = analogRead(POT_PIN);          // 0-1023
  OCR1B = (uint32_t)pot * 639 / 1023;          // map to 0-639

  static uint8_t hue = 0;
  fill_rainbow(leds, NUM_LEDS, hue++, 8);
  FastLED.show();
  delay(30);
}
