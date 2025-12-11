# Window Crank Flutter App

Flutter mobile application for controlling the ESP32-based window crank system via BLE.

## Features

- **BLE Connection**: Automatically scans and connects to "WindowCrank" device
- **Position Control**: Three buttons for Closed (0°), Half (90°), Open (180°)
- **Notification Sender**: Send messages to display on ESP32 OLED
- **Real-time Status**: Shows current window position and connection status

## Setup

1. **Install Dependencies**:
   ```bash
   flutter pub get
   ```

2. **Run the App**:
   ```bash
   flutter run
   ```

## Permissions

The app requires the following Android permissions:
- Bluetooth scan and connect
- Location access (required for BLE on Android)

## Usage

1. **Connect**: Tap "Connect" to scan for and connect to the ESP32 device
2. **Control Window**: Use Closed/Half/Open buttons to set servo position
3. **Send Notifications**: Type a message and tap "Send to Display" to show on OLED
4. **Monitor Status**: View connection status and current window position

## BLE Protocol

- **Service UUID**: `12345678-1234-1234-1234-123456789abc`
- **Position Characteristic**: `87654321-4321-4321-4321-cba987654321`
- **Notification Characteristic**: `11111111-2222-3333-4444-555555555555`

## Author

**Michael Garcia, M&E Design**  
Contact: michael@mandedesign.studio  
Website: www.mandedesign.studio