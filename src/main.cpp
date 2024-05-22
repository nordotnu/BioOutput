#include <Arduino.h>

#define sEMG_PIN_A 13
#define sEMG_PIN_B 14
#define sEMG_PIN_C 26
#define sEMG_PIN_D 25

#define LED_PIN 2

uint16_t valueA = 0;
uint16_t valueB = 0;
uint16_t valueC = 0;
uint16_t valueD = 0;
char buffer[128];
bool sendData = true;

// Time interval for processing the input signal.
TickType_t sampleTicks = 2 / portTICK_PERIOD_MS;
TickType_t sampleTicks2 = 1 / portTICK_PERIOD_MS;

void sensorTask(void *);
void serialTask(void *);

void receiveCb()
{
  int bytes = Serial.readBytes(buffer, sizeof(buffer));
  if (bytes == 0) {
    return;
  }

  if (buffer[bytes - 1] == 'R'){
  digitalWrite(LED_PIN, LOW);
  ESP.restart();
  }
}

void setup()
{
  Serial.begin(921600);


  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  xTaskCreate(sensorTask, "SensorTask", 48000, NULL, configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(serialTask, "SerialTask", 48000, NULL, configMAX_PRIORITIES, NULL);
}

void loop()
{
  vTaskDelay(60000 / portTICK_PERIOD_MS);
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

    BaseType_t res = xTaskDelayUntil(&startTicks, sampleTicks);
  }
}

void serialTask(void *)
{
  for (;;)
  {
    TickType_t startTicks = xTaskGetTickCount();

    Serial.printf("S%u,%u,%u,%uE\n", valueA, valueB, valueC, valueD);

    digitalWrite(LED_PIN, HIGH);

    BaseType_t res = xTaskDelayUntil(&startTicks, sampleTicks);
  }
}