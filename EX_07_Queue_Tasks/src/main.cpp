/********************************************************************
 * Exemplo que demonstra como cria uma fila, adiciona e le dados da
 * fila através de duas tasks.
 * Fabiano Piroli
********************************************************************/
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#define LED 2

QueueHandle_t xQueue;

TaskHandle_t xTask1Handle, xtask2Handle;

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

void setup() {

  BaseType_t xReturned;

  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  xQueue = xQueueCreate(5, sizeof(int));

  if(xQueue == NULL) {
    Serial.println("Nao foi possivel criar a fila");

    while(1);
  }

  xReturned = xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTask1Handle);

  if(xReturned == pdFAIL) {
    Serial.println("Nao foi possivel criar a task 1");
    while(1);
  }

  xReturned = xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xtask2Handle);

  if(xReturned == pdFAIL) {
    Serial.println("Nao foi possivel criar a task 2");
    while(1);
  }
}

void loop() {
  digitalWrite(LED, !digitalRead(LED));
  delay(pdMS_TO_TICKS(1000));
}

void vTask1(void *pvParameters) {
  int count = 0;

  while(1) {
    if(count < 10) {
      xQueueSend(xQueue, &count, portMAX_DELAY);
      count++;
    }
    else {
      count = 0;
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void vTask2(void *pvParameters){
  int value = 0;

  while(1) {
    if(xQueueReceive(xQueue, &value, pdMS_TO_TICKS(1000)) == pdTRUE) {
      Serial.println("Task 2 recebeu: " + String(value));
    }
    else {
      Serial.println("Task 2 nao recebeu nada (TIMEOUT).");
    }
  }
}