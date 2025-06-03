#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "hw_mic.h"
#include "dtmf_detect.h"

#define F_SAMPLES 16000
#define N_SAMPLES 1600
int32_t mic_samples[N_SAMPLES];

void setup() {
  Serial.begin(9600);

  // Initialize BLE broadcasting
  BLEDevice::init("luca");
  BLEServer *pServer = BLEDevice::createServer();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // set the minimum preferred connection interval
  pAdvertising->setMinPreferred(0x12); // set the maximum preferred connection interval
  BLEDevice::startAdvertising();


 // initialize sound interface
  hw_mic_init(F_SAMPLES); // initialize microphone with sample rate
}

void loop() {
  // start recording
  unsigned int num_samples = N_SAMPLES;
  hw_mic_read(mic_samples, &num_samples);
  for (int i =0; i<N_SAMPLES; i++){
    mic_samples[i] /= 1e5; // scale samples
  }
  float avg_val = 0;
  for (int i = 0; i < N_SAMPLES; i++) {
    avg_val += abs(mic_samples[i]);
  }
  Serial.printf("Average value: %f\n", avg_val / N_SAMPLES);
  static double sp_amp[2] = {0.1, 0.1}; // amplitude of the sine wave
  sp_amp[0] = 0.1;
  sp_amp[1] = 0.0;
  static double dtmf[2] = {0.0, 0.0}; // DTMF signal
  dtmf[0] = 0.0;
  dtmf[1] = 0.0;
  uint32_t start_time = millis();
  dtmf_detect(mic_samples, F_SAMPLES, N_SAMPLES, sp_amp, dtmf);
  Serial.printf("DTMF: %d / %f %f / %f %f \n", millis()- start_time, sp_amp[0], sp_amp[1], dtmf[0], dtmf[1]);
  delay(500); // wait for 500 ms before next recording
                
}

 