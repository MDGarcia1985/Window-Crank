/**
 * @file bleComm.cpp
 * @brief Implementation of BLE communication interface
 * @version 1.0.0
 * 
 * Implements BLE server for Flutter app communication:
 * - NimBLE stack for ESP32
 * - Custom service UUID for window crank
 * - Position control and notification characteristics
 * - Connection callbacks and data handling
 * 
 * @author Michael Garcia, M&E Design
 * @contact michael@mandedesign.studio
 * @website www.mandedesign.studio
 * @copyright Copyright (c) 2025 Michael Garcia, M&E Design
 * @license MIT License
 */

#include "bleComm.h"

#define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define POSITION_CHAR_UUID  "87654321-4321-4321-4321-cba987654321"
#define NOTIFY_CHAR_UUID    "11111111-2222-3333-4444-555555555555"

static int positionCommand = -1;
static bool newNotification = false;
static char notifSender[32] = "";
static char notifTime[16] = "";
static char notifMessage[128] = "";

class ServerCallbacks: public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer) {
        // Connection handled automatically
    }
    
    void onDisconnect(NimBLEServer* pServer) {
        pServer->startAdvertising();
    }
};

class PositionCallbacks: public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic* pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        if (value.length() > 0) {
            positionCommand = value[0] - '0';
        }
    }
};

class NotificationCallbacks: public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic* pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        if (value.length() > 0) {
            // Format: "sender|time|message"
            int pos1 = value.find('|');
            int pos2 = value.find('|', pos1 + 1);
            
            if (pos1 != std::string::npos && pos2 != std::string::npos) {
                strncpy(notifSender, value.substr(0, pos1).c_str(), 31);
                strncpy(notifTime, value.substr(pos1 + 1, pos2 - pos1 - 1).c_str(), 15);
                strncpy(notifMessage, value.substr(pos2 + 1).c_str(), 127);
                newNotification = true;
            }
        }
    }
};

BLE::BLE() {}

void BLE::begin() {
    NimBLEDevice::init("WindowCrank");
    server = NimBLEDevice::createServer();
    server->setCallbacks(new ServerCallbacks());
    
    service = server->createService(SERVICE_UUID);
    
    positionChar = service->createCharacteristic(
        POSITION_CHAR_UUID,
        NIMBLE_PROPERTY::WRITE
    );
    positionChar->setCallbacks(new PositionCallbacks());
    
    notificationChar = service->createCharacteristic(
        NOTIFY_CHAR_UUID,
        NIMBLE_PROPERTY::WRITE
    );
    notificationChar->setCallbacks(new NotificationCallbacks());
    
    service->start();
    server->startAdvertising();
}

bool BLE::isConnected() {
    return server->getConnectedCount() > 0;
}

int BLE::getPositionCommand() {
    int cmd = positionCommand;
    positionCommand = -1;
    return cmd;
}

bool BLE::hasNotification() {
    return newNotification;
}

void BLE::getNotification(char* sender, char* time, char* message) {
    strcpy(sender, notifSender);
    strcpy(time, notifTime);
    strcpy(message, notifMessage);
    newNotification = false;
}