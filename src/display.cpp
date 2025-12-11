/**
 * @file display.cpp
 * @brief Implementation of OLED display interface
 * @version 2.0.0
 * 
 * Renders window crank status:
 *  - Title ("Window Crank")
 *  - Current servo position (0°, 90°, 180°)
 *  - Pulse width in microseconds
 *  - Button press indicator
 *  - Position indicator bar
 * 
 * Uses U8g2 SSD1306 128x64 hardware I2C driver.
 * 
 * @author Michael Garcia, M&E Design
 * @contact michael@mandedesign.studio
 * @website www.mandedesign.studio
 * @copyright Copyright (c) 2025 Michael Garcia, M&E Design
 * @license MIT License
 */

#include "display.h"

Display::Display()
    : u8g2(U8G2_R0, U8X8_PIN_NONE)   // rotation, reset pin (none)
{
}

void Display::begin()
{
    u8g2.begin();
    u8g2.setFont(u8g2_font_ncenB08_tr);

    // Optional splash / clear
    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, "Window Crank");
    u8g2.drawStr(0, 24, "System Ready");
    u8g2.sendBuffer();
}

void Display::update(int position, int pulseUs, bool buttonPressed)
{
    u8g2.clearBuffer();

    // Title
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 10, "Window Crank");

    // Position line
    char posStr[32];
    const char* angles[] = {"0°", "90°", "180°"};
    snprintf(posStr, sizeof(posStr), "Position: %s (%d)", angles[position], position + 1);
    u8g2.drawStr(0, 30, posStr);

    // Pulse width line
    char pulseStr[32];
    snprintf(pulseStr, sizeof(pulseStr), "Pulse: %d us", pulseUs);
    u8g2.drawStr(0, 42, pulseStr);

    // Button status
    if (buttonPressed)
    {
        u8g2.drawStr(0, 54, "Button: PRESSED");
    }
    else
    {
        u8g2.drawStr(0, 54, "Button: Ready");
    }

    // Position indicator bar
    u8g2.drawFrame(0, 58, 128, 6);
    int barPos = (position * 124) / 2;  // 0, 62, 124
    u8g2.drawBox(barPos, 60, 4, 2);

    u8g2.sendBuffer();
}
