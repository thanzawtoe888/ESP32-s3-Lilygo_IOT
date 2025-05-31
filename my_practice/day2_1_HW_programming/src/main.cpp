#include <Arduino.h>
#include "hw_mic.h"

int32_t buf[1600];

void setup() {
  Serial.begin(9600);
  hw_mic_init(1600);  // Initialize microphone with 16kHz sample rate

}

void loop() {
  unsigned int num_samples = 1600;
  hw_mic_read(buf, &num_samples);  // Read samples from microphone
  float avg_val =0;
  for (int i = 0; i < num_samples; i++) {
    avg_val += (float)abs(buf[i]) / num_samples;  // Calculate average value
  }
  Serial.printf("%d,%f\n",num_samples, avg_val);  // Print average value to serial monitor
  
}
