#pragma once
/*
 * nukiBle.h
 *
 *  Created on: 14 okt. 2020
 *      Author: Jeroen
 */

#include "BLEDevice.h"
#include "nukiConstants.h"
#include "Arduino.h"

// #define BLE_DEBUG

class NukiBle : public BLEClientCallbacks{
  public:
    NukiBle(std::string& bleAddress);
    virtual ~NukiBle();

    QueueHandle_t  nukiBleIsrFlagQueue;

    void onConnect(BLEClient*) override;
    void onDisconnect(BLEClient*) override;

    virtual void initialize();
    bool connect();
    bool isPaired = false;
    void pairedStatus(bool status){
      isPaired = status;
    }

    static void handleErrorCode(uint8_t errorCode);
    static void handleReturnCode(uint16_t returnCode);

    bool executeLockAction(lockAction action);

  private:

    TaskHandle_t TaskHandleNukiBle;
    BaseType_t xHigherPriorityTaskWoken;
    void startNukiBleXtask();
    void pushNotificationToQueue();
    
    bool registerOnGdioChar();
    void sendPlainMessage(nukiCommand commandIdentifier, char* payload, uint8_t payloadLen);
    static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
    static void my_gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t* param);
        
    std::string bleAddress = "";
    BLEClient* pClient;
    BLERemoteService* pKeyturnerPairingService = nullptr;
    BLERemoteCharacteristic* pGdioCharacteristic = nullptr;
};

// class MyClientCallback: public BLEClientCallbacks {
//     public:
//     // friend NukiBle;
//     MyClientCallback();
//     virtual ~MyClientCallback() {};
//     void onConnect(BLEClient*) {};
//     void onDisconnect(BLEClient*) {
//         // pairedStatus(false);  
//         log_d("onDisconnect");
//     };
// };