/********************************************************************
* Exemplo para passagem de parâmetros
* Fabiano Piroli
********************************************************************/
//Biblioteca arduino
#include <Arduino.h>
//Biblioteca FreeRTOS
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// LED1: LED interno da placa (GPIO 2)
#ifndef LED_BUILTIN
  #define LED_BUILTIN 2
#endif
#define LED1 LED_BUILTIN

// LED2: Segundo LED (GPIO 4 - externo)
#define LED2 4

// Variáveis para armazenar os handles das tasks
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;

// Protótipos das funções das tasks
void task1(void *pvParameters);
void task2(void *pvParameters);

void setup() {
  Serial.begin(9600);

  // Criação das tasks
  xTaskCreate(task1, "Task 1", configMINIMAL_STACK_SIZE, (void*)LED1, 1, &task1Handle);
  xTaskCreate(task2, "Task 2", configMINIMAL_STACK_SIZE + 1024, NULL, 2, &task2Handle);
}

void loop() {
  vTaskDelay(1000); // Delay para evitar que o loop principal consuma muito tempo
}

// Função da Task 1
void task1(void *pvParameters) {

  int pin = (int) pvParameters; // Converte o parâmetro para inteiro
  pinMode(pin, OUTPUT);
  while (1) {
    digitalWrite(pin, !digitalRead(pin)); // Alterna o estado do LED
    vTaskDelay(pdMS_TO_TICKS(200)); // Delay de 200ms
  }
}

// Função da Task 2
void task2(void *pvParameters) {

  int cont = 0;

  while (1) {
    Serial.println("Task 2: " + String(cont++)); // Imprime a contagem no Serial Monitor

    if(cont == 10) {
      Serial.println("Suspendendo da Task 1...");
      digitalWrite(LED_BUILTIN, LOW); // Garante que o LED esteja apagado após a Task 1 ser suspensa
      vTaskSuspend(task1Handle); // Suspende a Task 1
    }
    else if(cont == 15) {
      Serial.println("Reiniciando Task 1...");
      vTaskResume(task1Handle); // Reinicia a Task 1
      digitalWrite(LED_BUILTIN, HIGH); // Garante que o LED esteja aceso após a Task 1 ser reiniciada
      cont = 0; // Reinicia a contagem
    }
  
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay de 1 segundo
  }
}