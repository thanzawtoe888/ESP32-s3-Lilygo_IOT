#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

void setup() {

  BLEDevice::init("ThanZawToe");
  BLEServer* pServer = BLEDevice::createServer();
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help to set values to the advertising data
  pAdvertising->setMaxPreferred(0x12);
  pAdvertising->addServiceUUID(BLEUUID("0000180D-0000-1000-8000-00805f9b34fb")); // Heart Rate Service
  BLEDevice::startAdvertising();
}

void loop() {
  // Nothing to do here, just wait for BLE events
  delay(2000);
}