/********************************************************************
 * Exemplo para definir em qual core cada task será executada no ESP32
 * Fabiano Piroli
 ********************************************************************/
// Biblioteca arduino
#include <Arduino.h>
// Biblioteca FreeRTOS
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
TaskHandle_t task3Handle = NULL;

// Protótipos das funções das tasks
void taskBlink(void *pvParameters);
void task2(void *pvParameters);

// Variáveis auxiliares
int valor = 500;

void setup()
{
  Serial.begin(9600);

  xTaskCreatePinnedToCore(
      taskBlink,
      "Task 1",
      configMINIMAL_STACK_SIZE,
      (void *)LED1,
      1,
      &task1Handle,
      APP_CPU_NUM
    ); // Task 1 no Core 0

  xTaskCreatePinnedToCore(
      task2,
      "Task 2",
      configMINIMAL_STACK_SIZE + 1024,
      (void *)valor,
      2,
      &task2Handle,
      PRO_CPU_NUM
    ); // Task 2 no Core 1

  xTaskCreatePinnedToCore(
      taskBlink,
      "Task 3",
      configMINIMAL_STACK_SIZE,
      (void *)LED2,
      1,
      &task3Handle,
      APP_CPU_NUM
    ); // Task 3 no Core 0
}

void loop()
{
  vTaskDelay(3000); // Delay para evitar que o loop principal consuma muito tempo
}

// Função da Task 1
void taskBlink(void *pvParameters)
{

  int pin = (int)pvParameters; // Converte o parâmetro para inteiro
  pinMode(pin, OUTPUT);
  while (1)
  {
    digitalWrite(pin, !digitalRead(pin)); // Alterna o estado do LED
    vTaskDelay(pdMS_TO_TICKS(200));       // Delay de 200ms
  }
}

// Função da Task 2
void task2(void *pvParameters)
{

  int cont = (int)pvParameters; // Converte o parâmetro para inteiro

  while (1)
  {
    Serial.println("Task 2: " + String(cont++)); // Imprime a contagem no Serial Monitor
    vTaskDelay(pdMS_TO_TICKS(1000));             // Delay de 1 segundo
  }
}