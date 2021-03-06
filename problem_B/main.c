#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#define MIN_CROSSING_DELAY 500
#define CROSSING_DELAY 1000
#define ARRIVE_DELAY 1500

xTaskHandle *batTaskHandles; // Create as a pointer since the number of bats are
                             // unknown at compile time

SemaphoreHandle_t guardResource;
SemaphoreHandle_t NQue;
SemaphoreHandle_t SQue;
SemaphoreHandle_t EQue;
SemaphoreHandle_t WQue;

int *batIds;
int i = 0;

volatile int counter = 0;

void goSouth(int batId) {
  const TickType_t waitingToArriveDelay =(rand() % ARRIVE_DELAY) / portTICK_PERIOD_MS; // Generate a random delay
  const TickType_t crossingDelay = (rand() % CROSSING_DELAY + MIN_CROSSING_DELAY) / portTICK_PERIOD_MS;
  vPrintf("BAT %d from North has started to run and is on its way to the ""crossing\n", batId);
  vTaskDelay(waitingToArriveDelay);
  vPrintf("BAT %d from North arrives at crossing\n", batId);
  xSemaphoreGive(NQue);
  vPrintf("\nQUE LENGTHS N:%d S:%d E:%d W:%d\n",uxSemaphoreGetCount(NQue),uxSemaphoreGetCount(SQue),uxSemaphoreGetCount(EQue),uxSemaphoreGetCount(WQue));
  while (uxSemaphoreGetCount(WQue) > 0) {
      if((uxSemaphoreGetCount(NQue)>0) && (uxSemaphoreGetCount(SQue)>0) && (uxSemaphoreGetCount(EQue)>0) && (uxSemaphoreGetCount(WQue)>0))
      {
          break;
      }
  }
  xSemaphoreTake(guardResource, portMAX_DELAY);
  xSemaphoreTake(NQue, portMAX_DELAY);
  vPrintf("\nQUE LENGTHS N:%d S:%d E:%d W:%d\n",uxSemaphoreGetCount(NQue),uxSemaphoreGetCount(SQue),uxSemaphoreGetCount(EQue),uxSemaphoreGetCount(WQue));
  vPrintf("BAT %d from North enters crossing\n", batId);
  vTaskDelay(crossingDelay);
  vPrintf("BAT %d from North leaving crossing\n", batId);
  xSemaphoreGive(guardResource);
  vTaskDelay(waitingToArriveDelay);
}

void goNorth(int batId) {
  const TickType_t waitingToArriveDelay =(rand() % ARRIVE_DELAY) / portTICK_PERIOD_MS; // Generate a random delay
  const TickType_t crossingDelay =(rand() % CROSSING_DELAY + MIN_CROSSING_DELAY) / portTICK_PERIOD_MS;
  vPrintf("BAT %d from South has started to run and is on its way to the ""crossing\n",batId);
  vTaskDelay(waitingToArriveDelay);
  vPrintf("BAT %d from South arrives at crossing\n", batId);
  xSemaphoreGive(SQue);
  vPrintf("\nQUE LENGTHS N:%d S:%d E:%d W:%d\n",uxSemaphoreGetCount(NQue),uxSemaphoreGetCount(SQue),uxSemaphoreGetCount(EQue),uxSemaphoreGetCount(WQue));
  while (uxSemaphoreGetCount(EQue) > 0) {
  }
  xSemaphoreTake(guardResource, portMAX_DELAY);
  xSemaphoreTake(SQue, portMAX_DELAY);
  vPrintf("\nQUE LENGTHS N:%d S:%d E:%d W:%d\n",uxSemaphoreGetCount(NQue),uxSemaphoreGetCount(SQue),uxSemaphoreGetCount(EQue),uxSemaphoreGetCount(WQue));
  vPrintf("BAT %d from South enters crossing\n", batId);
  vTaskDelay(crossingDelay);
  vPrintf("BAT %d from South leaving crossing\n", batId);
  xSemaphoreGive(guardResource);
  vTaskDelay(waitingToArriveDelay);
}

void goEast(int batId)
 {
  const TickType_t waitingToArriveDelay =(rand() % ARRIVE_DELAY) / portTICK_PERIOD_MS; // Generate a random delay
  const TickType_t crossingDelay =(rand() % CROSSING_DELAY + MIN_CROSSING_DELAY) / portTICK_PERIOD_MS;
  vPrintf( "BAT %d from West has started to run and is on its way to the crossing\n", batId);
  vTaskDelay(waitingToArriveDelay);
  vPrintf("BAT %d from West arrives at crossing\n", batId);
  xSemaphoreGive(WQue);
  vPrintf("\nQUE LENGTHS N:%d S:%d E:%d W:%d\n",uxSemaphoreGetCount(NQue),uxSemaphoreGetCount(SQue),uxSemaphoreGetCount(EQue),uxSemaphoreGetCount(WQue));
  while (uxSemaphoreGetCount(SQue) > 0) {
  }
  xSemaphoreTake(guardResource, portMAX_DELAY);
  xSemaphoreTake(WQue, portMAX_DELAY);
  vPrintf("\nQUE LENGTHS N:%d S:%d E:%d W:%d\n",uxSemaphoreGetCount(NQue),uxSemaphoreGetCount(SQue),uxSemaphoreGetCount(EQue),uxSemaphoreGetCount(WQue));
  vPrintf("BAT %d from West enters crossing\n", batId);
  vTaskDelay(crossingDelay);
  vPrintf("BAT %d from West leaving crossing\n", batId);
  xSemaphoreGive(guardResource); 
  vTaskDelay(waitingToArriveDelay);
}

void goWest(int batId) 
{
  const TickType_t waitingToArriveDelay =(rand() % ARRIVE_DELAY) / portTICK_PERIOD_MS; // Generate a random delay
  const TickType_t crossingDelay =(rand() % CROSSING_DELAY + MIN_CROSSING_DELAY) / portTICK_PERIOD_MS;
  vPrintf("BAT %d from East has started to run and is on its way to the crossing\n",batId);
  vTaskDelay(waitingToArriveDelay);
  vPrintf("BAT %d from East arrives at crossing\n", batId);
  xSemaphoreGive(EQue);
  vPrintf("\nQUE LENGTHS N:%d S:%d E:%d W:%d\n",uxSemaphoreGetCount(NQue),uxSemaphoreGetCount(SQue),uxSemaphoreGetCount(EQue),uxSemaphoreGetCount(WQue));
  while (uxSemaphoreGetCount(NQue) > 0) {
  }
  xSemaphoreTake(guardResource, portMAX_DELAY);
  xSemaphoreTake(EQue, portMAX_DELAY);
  vPrintf("\nQUE LENGTHS N:%d S:%d E:%d W:%d\n",uxSemaphoreGetCount(NQue),uxSemaphoreGetCount(SQue),uxSemaphoreGetCount(EQue),uxSemaphoreGetCount(WQue));
  vPrintf("BAT %d from East enters crossing\n", batId);
  vTaskDelay(crossingDelay);
  vPrintf("BAT %d from East leaving crossing\n", batId);
  xSemaphoreGive(guardResource);
  vTaskDelay(waitingToArriveDelay);
}

void batFromNorth(void *pvParameters) {
  int batId = *(int *)pvParameters;
  while (1) // Repeat forever
  {
    goSouth(batId);
    goNorth(batId);
  }
}

void batFromSouth(void *pvParameters) {
  int batId = *(int *)pvParameters;

  while (1) {
    goNorth(batId);
    goSouth(batId);
  }
}

void batFromEast(void *pvParameters) {
  int batId = *(int *)pvParameters;

  while (1) {
    goWest(batId);
    goEast(batId);
  }
}

void batFromWest(void *pvParameters) {
  int batId = *(int *)pvParameters;

  while (1) {
    goEast(batId);
    goWest(batId);
  }
}

int main(int argc, char **argv) {
  int batId;
  int numberOfBats;
  guardResource = xSemaphoreCreateMutex();
  NQue = xSemaphoreCreateCounting(10, 0);
  SQue = xSemaphoreCreateCounting(10, 0);
  EQue = xSemaphoreCreateCounting(10, 0);
  WQue = xSemaphoreCreateCounting(10, 0);

  if (argc < 2) {
    vPrintf("Argument missing, specify sequence of arrivals. Example: ./batman "
            "nsewwewn\n");
    exit(-1);
  }

  char *commands = argv[1];
  numberOfBats = strlen(argv[1]);

  batTaskHandles = malloc(sizeof(xTaskHandle) *
                          numberOfBats); // Allocate memory for the task handles
  batIds =
      malloc(sizeof(int) * numberOfBats); // Allocate memory for the task ids

  for (batId = 0; batId < numberOfBats; batId++) {
    batIds[batId] = batId;
    char currentBatDirection = (argv[1])[batId];
    char batTaskIdentity[50];

    if (currentBatDirection == 'n') {
      sprintf(batTaskIdentity, "Bat %d from North", batId);
      xTaskCreate(batFromNorth, batTaskIdentity, configMINIMAL_STACK_SIZE,
                  (void *)&batIds[batId], 1, batTaskHandles[batId]);
      vPrintf("Created task \"Bat %d from North\"\n", batId);
    } else if (currentBatDirection == 's') {
      sprintf(batTaskIdentity, "Bat %d from South", batId);
      xTaskCreate(batFromSouth, batTaskIdentity, configMINIMAL_STACK_SIZE,
                  (void *)&batIds[batId], 1, batTaskHandles[batId]);
      vPrintf("Created task \"Bat %d from South\"\n", batId);
    } else if (currentBatDirection == 'e') {
      sprintf(batTaskIdentity, "Bat %d from East", batId);
      xTaskCreate(batFromEast, batTaskIdentity, configMINIMAL_STACK_SIZE,
                  (void *)&batIds[batId], 1, batTaskHandles[batId]);
      vPrintf("Created task \"Bat %d from East\"\n", batId);
    } else if (currentBatDirection == 'w') {
      sprintf(batTaskIdentity, "Bat %d from West", batId);
      xTaskCreate(batFromWest, batTaskIdentity, configMINIMAL_STACK_SIZE,
                  (void *)&batIds[batId], 1, batTaskHandles[batId]);
      vPrintf("Created task \"Bat %d from West\"\n", batId);
    } else {
      vPrintf("Unknown direction: %c\n", currentBatDirection);
      exit(-1);
    }
  }

  vTaskStartScheduler();
  while (1) {
    // Do nothing
  };

  free(batTaskHandles); // Return memory for the task handles
  return 0;
}
