/********************************************************************
* Exemplo para criação de tasks
* Esse exemplo exibe como criar tarefas no FreeRTOS
* Fabiano Piroli
********************************************************************/
//Biblioteca arduino
#include <Arduino.h>
//Biblioteca FreeRTOS
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

//LED built-in --- IGNORE ---
#ifndef LED_BUILTIN
  #define LED_BUILTIN 2
#endif

// Variáveis para armazenar os handles das tasks
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;

// Protótipos das funções das tasks
void task1(void *pvParameters);
void task2(void *pvParameters);

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // Criação das tasks
  xTaskCreate(task1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 1, &task1Handle);
  xTaskCreate(task2, "Task 2", configMINIMAL_STACK_SIZE + 1024, NULL, 2, &task2Handle);
}

void loop() {
  vTaskDelay(1000); // Delay para evitar que o loop principal consuma muito tempo
}

// Função da Task 1
void task1(void *pvParameters) {
  while (1) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Alterna o estado do LED
    vTaskDelay(pdMS_TO_TICKS(200)); // Delay de 200ms
  }
}

// Função da Task 2
void task2(void *pvParameters) {

  int cont = 0;

  while (1) {
    Serial.println("Task 2: " + String(cont++)); // Imprime a contagem no Serial Monitor
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay de 1 segundo
  }
}