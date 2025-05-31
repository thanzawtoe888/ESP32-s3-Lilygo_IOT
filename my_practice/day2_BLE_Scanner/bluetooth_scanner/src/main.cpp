#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 5; // in seconds
BLEScan* pBLEScan;

// class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
//   void onResult(BLEAdvertisedDevice advertisedDevice) {
//     if (advertisedDevice.getRSSI() > -65) {
//       Serial.printf("Name : %s \n", advertisedDevice.getName().c_str());
//       Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
     
//     }   
//   }
// };


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveName()) 
    {
      if (advertisedDevice.getName() == "DESKTOP-R8AJIM5")
      {
        Serial.printf("Name : %s \n", advertisedDevice.getRSSI() );
      }
    } 
    else 
    {
      Serial.println("No name found");
    }
  }
};

// class MyExtAdvertisedDeviceCallbacks: public BLEExtAdvertisingCallbacks {
//   void onResult( esp_ble_gap_ext_adv_reprot_t report) {
//     Serial.printf("Found device: %s \n", advertisedDevice.toString().c_str());
//     Serial.printf("RSSI: %d \n", advertisedDevice.getRSSI());
//     Serial.printf("Address: %s \n", advertisedDevice.getAddress().toString().c_str());
//   }
// };

void setup() {
Serial.begin(115200);
BLEDevice::init("");
pBLEScan = BLEDevice::getScan(); //create new scan
pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
pBLEScan->setInterval(1000);
pBLEScan->setWindow(99);  // less or equal setInterval value

}

void loop() {
BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
Serial.print("Devices found: ");
Serial.println(foundDevices.getCount());
Serial.println("Scan done!");
pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
delay(2000);
}
