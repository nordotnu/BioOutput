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
// Time interval for processing the input signal.
TickType_t sampleTicks = 2 / portTICK_PERIOD_MS;
void sensorTask(void *);

void setup()
{  
   Serial.begin(115200);
   pinMode(LED_PIN, OUTPUT);
   xTaskCreate(sensorTask, "SensorTask", 48000, NULL, configMAX_PRIORITIES - 1, NULL);
}

void loop(){
vTaskDelay(60000 / portTICK_PERIOD_MS);
}

void sensorTask(void *)
{
  for(;;) {
    TickType_t startTicks = xTaskGetTickCount();
    valueA = analogRead(sEMG_PIN_A);
    valueB = analogRead(sEMG_PIN_B);
    valueC = analogRead(sEMG_PIN_C);
    valueD = analogRead(sEMG_PIN_D);
    u_int64_t value = ((u_int64_t)valueA << 48) | ((u_int64_t)valueB << 32) | ((u_int64_t)valueC << 16) | valueD;
    Serial.println(value);
    //Serial.printf(">A:%d\n>B:%d\n>C:%d\n>D:%d\n", valueA, valueB, valueC, valueD);
    BaseType_t res = xTaskDelayUntil(&startTicks, sampleTicks);
    digitalWrite(LED_PIN, !res);
  }
}
