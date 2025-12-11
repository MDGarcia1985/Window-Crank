/**
 * @file Servo.h
 * @brief ESP32 servo control using LEDC PWM
 * @version 1.0.0
 * 
 * Provides servo control interface using ESP32's native LEDC peripheral:
 * - Hardware PWM generation at 50Hz
 * - Microsecond pulse width control (1000-2000µs)
 * - 16-bit resolution for precise timing
 * 
 * @author Michael Garcia, M&E Design
 * @contact michael@mandedesign.studio
 * @website www.mandedesign.studio
 * @copyright Copyright (c) 2025 Michael Garcia, M&E Design
 * @license MIT License
 */

#pragma once
#include <Arduino.h>

class Servo {
private:
  uint8_t channel;
  uint8_t pin;
  
public:
  Servo(uint8_t ch, uint8_t servoPin);
  void begin();
  void writeUs(int us);
};