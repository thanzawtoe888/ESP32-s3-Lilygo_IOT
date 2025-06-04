#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "hw_mic.h"

// function prototypes
void task_read_mic(void *pvParameters);
void task_process_mic(void *pvParameters);
void on_message(char* topic, byte* payload, unsigned int length);  //downlink from PubSubClient.h

//shared variables
SemaphoreHandle_t xSemaphore = NULL;
int mem_idx=0;
int32_t mic_samples[2*1600]; // 2 seconds of 16000Hz samples
WiFiClient espClient;
PubSubClient mqtt_client(espClient);

void setup(){
  delay(3000); // wait for serial monitor
  Serial.begin(9600);
  WiFi.begin("Xiaomi2020Laptop", "11112222");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("MAC address: %s\n", WiFi.macAddress().c_str());
  Serial.printf("RSSI: %d\n", WiFi.RSSI());
  
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

  // 4. connect to MQTT broker
  mqtt_client.setServer("broker.emqx.io", 1883); // public broker
  mqtt_client.setCallback(on_message); // set callback for incoming messages
  mqtt_client.connect("tzt_fjasdfjghehtrej", "ttt", "zzz"); // connect with user and password
  mqtt_client.subscribe("tzt/luca/esp32/cmd"); // subscribe to topic
  if (!mqtt_client.connected()) {
    Serial.println("Failed to connect to MQTT broker");
    return;
  }
  Serial.println("Connected to MQTT broker");
}
  
void loop() {
  mqtt_client.loop(); // process MQTT messages
  if (!mqtt_client.connected()) {
    Serial.println("MQTT client not connected, reconnecting...");
    mqtt_client.connect("alexa!@$@$&^@%!*&^%$&^@!%^$*&@faer343", "ttt", "zzz"); // reconnect
  }

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
    //Serial.println(avg_val); // print average value
  }
}

void on_message(char* topic, byte* payload, unsigned int length) {
  char buf[200];
  memcpy(buf, payload, length);
  buf[length] = '\0'; // null-terminate the string
  Serial.printf("Received message on topic %s: %s\n", topic, buf);
}