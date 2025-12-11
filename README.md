# Window Crank Servo Control System

ESP32-based window crank controller with three-position servo control and OLED status display.

## Hardware Requirements

- **ESP32 Development Board**
- **Standard Servo Motor** (SG90 or similar)
- **SSD1306 OLED Display** (128x64, I2C)
- **Push Button** (momentary, normally open)
- **LED** (status indicator)
- **Resistors** (10kΩ pull-up, 220Ω LED current limiting)

## Wiring Diagram

```
ESP32 Pin | Component        | Notes
----------|------------------|------------------
GPIO 15   | Servo Signal     | PWM control
GPIO 25   | Button           | Internal pull-up
GPIO 26   | Status LED       | 220Ω resistor
GPIO 21   | OLED SDA         | I2C data
GPIO 22   | OLED SCL         | I2C clock
5V        | Servo VCC        | External power recommended
3.3V      | OLED VCC         | 
GND       | All GND          | Common ground
```

## Features

- **Three Position Control**: 0°, 90°, 180° servo positions
- **Button Cycling**: Press to advance through positions
- **OLED Display**: Real-time position and pulse width display
- **Status LED**: Visual feedback during operation
- **Hardware PWM**: ESP32 LEDC peripheral for precise timing

## Project Structure

```
Window Crank/
├── src/
│   ├── main.cpp          # Main application
│   ├── Servo.cpp         # Servo control implementation
│   └── display.cpp       # OLED display implementation
├── include/
│   ├── Servo.h           # Servo control interface
│   └── display.h         # Display interface
├── lib/                  # External libraries
├── test/                 # Unit tests
└── platformio.ini        # Build configuration
```

## Dependencies

Add to `platformio.ini`:

```ini
lib_deps = 
    olikraus/U8g2@^2.34.22
```

## Operation

1. **Power On**: System initializes to position 0° (1000μs pulse)
2. **Button Press**: Cycles through positions (0° → 90° → 180° → 0°)
3. **Display**: Shows current position, pulse width, and button status
4. **LED**: Illuminates during position changes

## Technical Specifications

- **PWM Frequency**: 50Hz (standard servo)
- **Pulse Width Range**: 1000-2000μs
- **Resolution**: 16-bit (65535 steps)
- **Display Update**: Real-time
- **Debounce Delay**: 300ms

## Author

**Michael Garcia, M&E Design**  
Contact: michael@mandedesign.studio  
Website: www.mandedesign.studio

## License

MIT License - Copyright (c) 2025 Michael Garcia, M&E Design