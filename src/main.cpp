/**
 * @file main.cpp
 * @brief Window crank servo control system (non-blocking)
 * @version 1.1.0
 * 
 * ESP32-based window crank controller with non-blocking operation:
 * - Three-position servo control (0°, 90°, 180°)
 * - Button debouncing with fixed 5ms tick interval
 * - Non-blocking LED pulse and display refresh
 * - Real-time OLED status display
 * 
 * Hardware:
 * - ESP32 microcontroller
 * - Standard servo motor on pin 15
 * - Push button on pin 25 (pull-up)
 * - Status LED on pin 26
 * - SSD1306 OLED display (I2C)
 * 
 * @author Michael Garcia, M&E Design
 * @contact michael@mandedesign.studio
 * @website www.mandedesign.studio
 * @copyright Copyright (c) 2025 Michael Garcia, M&E Design
 * @license MIT License
 */

#include <Arduino.h>
#include "servo.h"
#include "display.h"
#include "buttonDebounce.h"

// Pins
#define SERVO_PIN   15
#define BUTTON_PIN  25   // button to GND, INPUT_PULLUP
#define LED_PIN     26

#define SERVO_MIN   1000     // µs ~ 0°
#define SERVO_MAX   2000     // µs ~ 180°

static constexpr uint32_t DEBOUNCE_TICK_MS = 5;     // fixed sample period
static constexpr uint32_t DISPLAY_PERIOD_MS = 100;  // 10 Hz display refresh
static constexpr uint32_t LED_PULSE_MS = 300;       // LED on-time after press

Servo servo(0, SERVO_PIN);
Display display;

ButtonDebounce button; // Use default config

int idx = 0;
const int pulses[] = { SERVO_MIN, (SERVO_MIN + SERVO_MAX) / 2, SERVO_MAX };

// Scheduling state
uint32_t lastDebounceTick = 0;
uint32_t lastDisplayTick  = 0;
uint32_t ledUntil         = 0;

void setup()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  servo.begin();
  display.begin();

  servo.writeUs(pulses[idx]);
  display.update(idx, pulses[idx], false);

  const uint32_t now = millis();
  lastDebounceTick = now;
  lastDisplayTick  = now;
}

void loop()
{
  uint32_t now = millis();

  // ---- Debounce tick (fixed period) ----
  while ((uint32_t)(now - lastDebounceTick) >= DEBOUNCE_TICK_MS) {
    lastDebounceTick += DEBOUNCE_TICK_MS;

    button.updateActiveLow(digitalRead(BUTTON_PIN));

    if (button.pressed()) {
      idx = (idx + 1) % 3;
      servo.writeUs(pulses[idx]);

      digitalWrite(LED_PIN, HIGH);
      ledUntil = lastDebounceTick + LED_PULSE_MS; // tie to tick timeline
    }

    now = millis(); // refresh so catch-up remains accurate
  }

  // ---- LED timeout (non-blocking) ----
  if (ledUntil != 0 && (int32_t)(millis() - ledUntil) >= 0) {
    digitalWrite(LED_PIN, LOW);
    ledUntil = 0;
  }

  // ---- Display refresh (non-blocking) ----
  if ((uint32_t)(millis() - lastDisplayTick) >= DISPLAY_PERIOD_MS) {
    lastDisplayTick = millis();
    display.update(idx, pulses[idx], button.down());
  }
}
