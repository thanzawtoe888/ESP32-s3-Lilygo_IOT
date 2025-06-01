#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 5; // in seconds
BLEScan* pBLEScan;

// class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
//   void onResult(BLEAdvertisedDevice advertisedDevice) {
//     if(advertisedDevice.getRSSI() > -60){
//       Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
//       BLEAddress addr = advertisedDevice.getAddress();
//       Serial.printf("Device Address: %s \n", addr.toString().c_str());
//       BLEAddress phone_addr = BLEAddress("ed:8e:0e:e5:60:82"); // Replace with your phone's address
//       if (addr == phone_addr) {
//         Serial.println("Found my phone!");
//       } 
//   }
// }
// };

// class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
//   void onResult(BLEAdvertisedDevice advertisedDevice) {
//     BLEAddress phone_addr = BLEAddress("ed:8e:0e:e5:60:82"); // Replace with your phone's address
//     BLEAddress addr = advertisedDevice.getAddress();
//     if (addr == phone_addr) {
//       Serial.printf("Found my phone: %s \n", advertisedDevice.toString().c_str());
//     }

//   }
// };

//Get the name of the device and print it
// class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
//   void onResult(BLEAdvertisedDevice advertisedDevice) {
//     // Check if the device name is "RAPOO BT4.0 MS"
//     if (advertisedDevice.getName() == "RAPOO BT4.0 MS") {
//       Serial.printf("Found Target Device: %s \n", advertisedDevice.toString().c_str());
//     }
//   }
// };

//print only target device name and RSSI
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveName()) {
      if(advertisedDevice.getName() == "RAPOO BT4.0 MS") {
        Serial.printf("My Mouse: %d \n", advertisedDevice.getRSSI());
      }
    }
  }
};

void setup() {
  Serial.begin(9600);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); // [Necessary Line]
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster (Optional)
  pBLEScan->setInterval(100); // (Optional)
  pBLEScan->setWindow(99);  // less or equal setInterval value (Optional)
}

void loop() {
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}