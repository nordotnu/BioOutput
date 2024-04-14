#include <Arduino.h>

#define sEMG_PIN_A 12
#define sEMG_PIN_B 14
#define sEMG_PIN_C 27
#define sEMG_PIN_D 26

#define LED_PIN 2

uint16_t valueA = 0;
uint16_t valueB = 0;
uint16_t valueC = 0;
uint16_t valueD = 0;
uint8_t startByte = 0xFF;
uint8_t stopByte = 0xFE;
// Time interval for processing the input signal.
TickType_t sampleTicks = 2 / portTICK_PERIOD_MS;
TickType_t sampleTicks2 = 1 / portTICK_PERIOD_MS;
void sensorTask(void *);
void serialTask(void *);

void setup()
{
  Serial.begin(921600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  xTaskCreate(sensorTask, "SensorTask", 48000, NULL, configMAX_PRIORITIES - 1, NULL);
  //xTaskCreate(serialTask, "SerialTask", 48000, NULL, configMAX_PRIORITIES - 1, NULL);
}

void loop()
{
  
      Serial.printf("S%0u,%0u,%0u,%0uE\n", valueA, valueB, valueC, valueD);
      vTaskDelay(1 / portTICK_PERIOD_MS);
}

void sensorTask(void *)
{
  for (;;)
  {
    TickType_t startTicks = xTaskGetTickCount();
    valueA = analogRead(sEMG_PIN_A);
    valueB = analogRead(sEMG_PIN_B);
    valueC = analogRead(sEMG_PIN_C);
    valueD = analogRead(sEMG_PIN_D);
    /*
        char ack = Serial.read();
        if (ack == 'A')
        {
          vTaskDelay(1 / portTICK_PERIOD_MS);
          Serial.printf("S%u,%u,%u,%uE", valueA, valueB, valueC, valueD);
        } else if (ack == 'R') {
          Serial.printf("FLUSH");
          Serial.flush();
        }
    */
    BaseType_t res = xTaskDelayUntil(&startTicks, sampleTicks);
    // digitalWrite(LED_PIN, !res);
  }
}

void serialTask(void *)
{
  for (;;)
  {
  TickType_t startTicks = xTaskGetTickCount();
      Serial.printf("S%0u,%0u,%0u,%0uE\n", valueA, valueB, valueC, valueD);
      //Serial.printf("S%u,%u,%u,%uE\n", valueA, valueB, valueC, valueD);
    /*char ack = Serial.read();
    if (ack == 'A')
    {
      //Serial.println("S1111,2222,3333,4444E");
    }*/
      //vTaskDelay(1 / portTICK_PERIOD_MS);
    BaseType_t res = xTaskDelayUntil(&startTicks, sampleTicks2);
    //digitalWrite(LED_PIN, !res);
  }
}