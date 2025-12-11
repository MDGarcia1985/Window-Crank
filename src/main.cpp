/**
 * @file main.cpp
 * @brief Window crank servo control system
 * @version 1.0.0
 * 
 * ESP32-based window crank controller with OLED display:
 * - Three-position servo control (0°, 90°, 180°)
 * - Button-activated position cycling
 * - Real-time OLED status display
 * - LED status indicator
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
#include "Servo.h"
#include "display.h"

// Pins
#define SERVO_PIN   15   // servo signal
#define BUTTON_PIN  25   // button to GND
#define LED_PIN     26   // status LED

#define SERVO_MIN   1000     // µs ~ 0°
#define SERVO_MAX   2000     // µs ~ 180°

Servo servo(0, SERVO_PIN);
Display display;
int idx = 0;
const int pulses[] = { SERVO_MIN, (SERVO_MIN + SERVO_MAX) / 2, SERVO_MAX };

void setup() 
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  servo.begin();
  display.begin();
  servo.writeUs(pulses[idx]); // start at first position
  display.update(idx, pulses[idx], false);
}

void loop() 
{
  bool buttonPressed = digitalRead(BUTTON_PIN) == LOW;
  
  if (buttonPressed) 
  {
    digitalWrite(LED_PIN, HIGH);
    display.update(idx, pulses[idx], true);
    idx = (idx + 1) % 3;
    servo.writeUs(pulses[idx]);
    delay(300);                 // debounce / move time
    digitalWrite(LED_PIN, LOW);
    while (digitalRead(BUTTON_PIN) == LOW) { delay(10); } // wait release
  }
  
  display.update(idx, pulses[idx], false);
  delay(20);
}
