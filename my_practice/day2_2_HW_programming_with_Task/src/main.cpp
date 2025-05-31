#include <Arduino.h>
#include "hw_mic.h"

//function prototypes
void task_read_mic(void *pvParameters);
void task_process_mic(void *pvParameters);

// shared variables
SemaphoreHandle_t xSemaphore = NULL;
int mem_idx = 0;
int32_t mic_samples[2*1600];  // buffer for microphone samples

void setup() {
  

  // 1. prepare semaphore
  xSemaphore = xSemaphoreCreateBinary();

  // 2. create task to read mic
  xTaskCreate(
    task_read_mic,          // Task function
    "task_read_mic",          // Name of the task
    4096,                   // Stack size in bytes
    NULL,                   // Task input parameter
    3,                      // Priority of the task
    NULL                    // Task handle to keep track of the task
  );

  // 3. create task to process mic data
  xTaskCreate(
    task_process_mic,          // Task function
    "task_process_mic",          // Name of the task
    4096,                   // Stack size in bytes
    NULL,                   // Task input parameter
    3,                      // Priority of the task
    NULL                    // Task handle to keep track of the task
  );

 

}

void loop() {
 delay(1000);  // main loop does nothing, tasks handle the work
}
 
void task_read_mic(void *pvParameters){
  //setup ()
  int cur_mem_idx = 0; // current memory index
  hw_mic_init(16000);
  //loop()
  while(1){
    unsigned int num_samples = 1600; // number of samples to read
    hw_mic_read(mic_samples + mem_idx*1600 , &num_samples);
    mem_idx = cur_mem_idx; // update memory index
    cur_mem_idx = (cur_mem_idx+1) %2; // current memory index
    xSemaphoreGive(xSemaphore); // give semaphore to signal that data is ready
  }
}


void task_process_mic(void *pvParameters){
  //setup ()
  Serial.begin(9600);
  //loop()
  while(1){
    xSemaphoreTake(xSemaphore, portMAX_DELAY); // wait for semaphore to be given
    float avg_val = 0.0; // average value of samples
    for (int i = 0; i < 1600; i++) {
      avg_val += (float)abs(mic_samples[mem_idx*1600 + i]) / 1600.0; // calculate average value
      
    }
    Serial.println(avg_val); // print average value
  }
}