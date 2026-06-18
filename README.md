# PC ARGB Fan Controller

Arduino Pro Micro firmware for an ARGB case fan:
- 25 kHz PWM fan speed control (pin 10)
- WS2812B addressable RGB animation (pin 5)
- Fan speed controlled with a 1 kΩ potentiometer

## Hardware

- Arduino Pro Micro 5V/16MHz
- UPSiREN UF-9 PRISM 4 PRO 92mm ARGB fan (or any 4-pin PWM + 3-pin ARGB fan)
- 1 kΩ potentiometer (model 102L)
- 12 V PSU for the fan motor

## Wiring

### Fan 4-pin PWM
| Fan wire | Connect to |
|----------|------------|
| Red (+12V) | 12 V PSU |
| Black (GND) | PSU GND |
| Yellow (tach) | unused |
| Blue (PWM) | Arduino D10 |

### Fan 3-pin ARGB
| Fan wire | Connect to |
|----------|------------|
| +5V | Arduino 5V |
| Data | Arduino D5 |
| GND | Arduino GND |

### Potentiometer
| Pot pin | Connect to |
|---------|------------|
| Outer 1 | Arduino 5V |
| Wiper | Arduino A0 |
| Outer 2 | Arduino GND |

> Tie PSU GND and Arduino GND together.

## Build & Flash

```bash
pio run
pio run --target upload
```

## Notes

- `NUM_LEDS` in `src/main.cpp` should match your fan's actual LED count.
- If the fan spins down when you turn the pot up, swap the two outer pot pins.
