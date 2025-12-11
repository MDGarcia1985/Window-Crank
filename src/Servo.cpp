/**
 * @file servo.cpp
 * @brief Implementation of ESP32 servo control
 * @version 1.0.0
 * 
 * Uses ESP32 LEDC peripheral for precise servo control:
 * - 50Hz PWM frequency for standard servos
 * - 16-bit resolution (65535 steps)
 * - Direct microsecond pulse width control
 * - Hardware-based timing for accuracy
 * 
 * @author Michael Garcia, M&E Design
 * @contact michael@mandedesign.studio
 * @website www.mandedesign.studio
 * @copyright Copyright (c) 2025 Michael Garcia, M&E Design
 * @license MIT License
 */

#include "servo.h"

#define FREQ        50
#define RES_BITS    16

Servo::Servo(uint8_t ch, uint8_t servoPin) : channel(ch), pin(servoPin) {}

void Servo::begin() {
  ledcSetup(channel, FREQ, RES_BITS);
  ledcAttachPin(pin, channel);
}

void Servo::writeUs(int us) {
  const uint32_t period_us = 1000000UL / FREQ;
  const uint32_t maxDuty = (1UL << RES_BITS) - 1;
  uint32_t duty = (uint32_t)((uint64_t)us * maxDuty / period_us);
  ledcWrite(channel, duty);
}