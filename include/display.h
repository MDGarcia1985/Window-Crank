/**
 * @file display.h
 * @brief OLED display interface using SSD1306 128x64
 * @version 2.1.0
 * 
 * Displays window crank status including:
 * - Current servo position (0°, 90°, 180°)
 * - Position index and microsecond pulse width
 * - Button press status
 * 
 * Uses I2C communication on ESP32 SDA/SCL pins
 * 
 * @author Michael Garcia, M&E Design
 * @contact michael@mandedesign.studio
 * @website www.mandedesign.studio
 * @copyright Copyright (c) 2025 Michael Garcia, M&E Design
 * @license MIT License
 */

#pragma once
#include <Arduino.h>
#include <U8g2lib.h>

class Display
{
public:
    /**
     * @brief Constructor
     */
    Display();
    
    /**
     * @brief Initialize OLED display
     */
    void begin();
    
    /**
     * @brief Update display with current servo status
     * @param position Position index (0, 1, 2)
     * @param pulseUs Servo pulse width in microseconds
     * @param buttonPressed Button press state
     */
    void update(int position, int pulseUs, bool buttonPressed);

private:
    /// @brief SSD1306 128x64 OLED driver (hardware I2C, full buffer)
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
};
