#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

xTaskHandle incTaskHandle;
xTaskHandle decTaskHandle;
xTaskHandle printFinalCounterTaskHandle;

SemaphoreHandle_t incTaskDone;
SemaphoreHandle_t decTaskDone;
SemaphoreHandle_t guardResource;

volatile int counter = 0;

void incCounterTask(void *pvParameters)
{
    xSemaphoreTake(guardResource, portMAX_DELAY);
    printf("upcounter \n");
    int i;
    for(i = 0; i < 1e7; i++)
    {
	counter = counter + 1;
    }
    printf("upcounter %d \n",counter);
     xSemaphoreGive(guardResource);
    xSemaphoreGive(incTaskDone); 
    vTaskDelete(incTaskHandle);
}

void decCounterTask(void *pvParameters)
{
    xSemaphoreTake(guardResource, portMAX_DELAY);
    printf("downcounter \n");
    int i;
    for(i = 0; i < 1e7; i++ )
    {
	counter = counter - 1;
    }
    printf("downcounter %d \n",counter);
     xSemaphoreGive(guardResource);
    xSemaphoreGive(decTaskDone); 
    vTaskDelete(decTaskHandle);
}

void printFinalCounterTask(void *pvParameters)
{
    xSemaphoreTake(incTaskDone, portMAX_DELAY);
    xSemaphoreTake(decTaskDone, portMAX_DELAY);
    printf("Final value: %d\n", counter);
    vTaskDelete(printFinalCounterTaskHandle);
}

int main(int argc, char **argv)
{
    incTaskDone = xSemaphoreCreateBinary();
    decTaskDone = xSemaphoreCreateBinary();
    guardResource = xSemaphoreCreateMutex();
    xTaskCreate(incCounterTask, "Increase Counter Task", configMINIMAL_STACK_SIZE, NULL, 1, &incTaskHandle);
    xTaskCreate(decCounterTask, "Decrease Counter Task", configMINIMAL_STACK_SIZE, NULL, 1, &decTaskHandle);
    xTaskCreate(printFinalCounterTask, "Print Final Counter Task", configMINIMAL_STACK_SIZE, NULL, 1, &printFinalCounterTaskHandle);
    vTaskStartScheduler();
    for( ;; );
}