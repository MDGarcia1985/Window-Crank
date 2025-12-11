/**
 * @file bleComm.h
 * @brief BLE communication interface for Flutter app integration
 * @version 1.0.0
 * 
 * Provides BLE server functionality for window crank system:
 * - Device advertising as "WindowCrank"
 * - Servo position control commands
 * - Notification data reception
 * - Connection status management
 * 
 * @author Michael Garcia, M&E Design
 * @contact michael@mandedesign.studio
 * @website www.mandedesign.studio
 * @copyright Copyright (c) 2025 Michael Garcia, M&E Design
 * @license MIT License
 */

#pragma once
#include <Arduino.h>
#include <NimBLEDevice.h>

class BLE {
private:
    NimBLEServer* server;
    NimBLEService* service;
    NimBLECharacteristic* positionChar;
    NimBLECharacteristic* notificationChar;
    bool deviceConnected = false;
    
public:
    BLE();
    void begin();
    bool isConnected();
    int getPositionCommand();
    bool hasNotification();
    void getNotification(char* sender, char* time, char* message);
};