#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "hw_mic.h"


//function prototypes
void task_read_mic(void *pvParameters); 
void task_process_mic(void *pvParameters);
void on_message(char* topic, byte* payload, unsigned int length);


//shared variables
SemaphoreHandle_t xSemaphore = NULL;
int mem_idx = 0;
int32_t mic_samples[2*1600];
WiFiClient espClient;
PubSubClient mqtt_client(espClient);
JsonDocument doc;
float avg_val = 0.0;

void setup() {

  //0. init Wifi
  delay(3000);
  Serial.begin(9600);
  WiFi.begin("Xiaomi5G", "111333888");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.println(".");
  }
  Serial.println("WiFi Connected");
  Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("RSSI: %d\n", WiFi.RSSI());
  

  // 1. preapre semaphore 
  xSemaphore = xSemaphoreCreateBinary();

  // 2. create task to read mic 
  xTaskCreate(
    task_read_mic,    // task function
    "task_read_mic",  // name of task
    4096,           // stack size of task
    NULL,           // parameter of the task
    3,              // priority of the task
    NULL           // task handle to keep track of created task
  );
  
  // 3. create task to proces mic data
  xTaskCreate(
    task_process_mic,    // task function
    "task_process_mic",  // name of task
    4096,           // stack size of task
    NULL,           // parameter of the task
    3,              // priority of the task
    NULL           // task handle to keep track of created task
  );

  //4. connect to mqtt broker
    mqtt_client.setServer("broker.emqx.io", 1883);
    mqtt_client.setCallback(on_message);
    mqtt_client.connect("tztluca_883757348214838rfiafhdhfadfadklfad");
    mqtt_client.subscribe("aiot/#");
    Serial.println("Connected to MQTT broker");
}

void loop() {
  mqtt_client.loop(); // process incoming MQTT messages
  delay(1000);
  
}

void task_read_mic(void *pvParameters) {
  int cur_mem_idx = 0;
  hw_mic_init(16000);
  //loop
  while(1) {
    static unsigned int num_samples = 1600;
    hw_mic_read(mic_samples + cur_mem_idx*1600, &num_samples);
    mem_idx = cur_mem_idx;
    cur_mem_idx = (cur_mem_idx + 1) % 2;  
    xSemaphoreGive(xSemaphore);

    }
  
}


void task_process_mic(void *pvParameters) {
  
  while(1) {
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    float avg_val = 0.0;
    for(int i = 0; i < 1600; i++) {
      avg_val = (float)abs(mic_samples[mem_idx * 1600 + i]) / 1600;
    }
    //Serial.println(avg_val);
  }
}

void on_message(char* topic, byte* payload, unsigned int length){
  char buf[200];
  memcpy(buf, payload, length);
  buf[length] = '\0';
  Serial.printf("Received on topic %s: %s\n", topic, buf);
  // deserializeJson(doc, buf);
  // if (doc["cmd"] == "listen"){
  //   //do something
  //   Serial.println("Start listening");
  //   doc.clear();
  //   doc["status"] = "ok" ;
  //   doc["value"] = avg_val;
  //   serializeJson(doc, buf);
  //   mqtt_client.publish("aiot/luca/esp32/resp", buf);

  // }

}