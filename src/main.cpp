#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    16
#define FAN_PWM_PIN 10
#define POT_PIN     A0
#define TACH_PIN    7    // Fan tachometer wire (connect TACH + 10k pull-up to 5V)

CRGB leds[NUM_LEDS];

// Tachometer pulse counting (2 pulses per revolution for most PC fans)
volatile uint16_t tachPulses = 0;
void tachISR() { tachPulses++; }

void setup() {
  Serial.begin(9600);

  // Timer1 -> 25 kHz Fast PWM on OC1B (pin 10), TOP = 639
  pinMode(FAN_PWM_PIN, OUTPUT);
  TCCR1A = (1 << COM1B1) | (1 << WGM11);
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);
  ICR1 = 639;

  // Tachometer input (open-collector, needs external 10k pull-up to 5V)
  pinMode(TACH_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(TACH_PIN), tachISR, FALLING);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(150);
}

void loop() {
  uint16_t pot = analogRead(POT_PIN);   // 0-1023

  // Map pot to duty: full 0..639 range
  // At max pot: force constant HIGH for absolute 100%
  if (pot >= 1000) {
    TCCR1A &= ~(1 << COM1B1);
    digitalWrite(FAN_PWM_PIN, HIGH);
  } else {
    TCCR1A |= (1 << COM1B1);
    OCR1B = (uint32_t)pot * 639 / 1000;
  }

  // Every 1 second: calculate RPM and print debug info
  static unsigned long lastRPM = 0;
  static uint16_t rpm = 0;
  if (millis() - lastRPM >= 1000) {
    noInterrupts();
    uint16_t pulses = tachPulses;
    tachPulses = 0;
    interrupts();
    rpm = pulses * 30;  // 2 pulses/rev -> pulses/2*60 = pulses*30
    lastRPM = millis();

    Serial.print("POT=");
    Serial.print(pot);
    Serial.print(" DUTY=");
    Serial.print((pot >= 1000) ? 100 : (int)((uint32_t)pot * 100 / 1000));
    Serial.print("% RPM=");
    Serial.print(rpm);
    Serial.print(" MODE=");
    Serial.println((pot >= 1000) ? "DC_HIGH" : "PWM");
  }

  static uint8_t hue = 0;
  fill_rainbow(leds, NUM_LEDS, hue++, 8);
  FastLED.show();
  delay(30);
}
