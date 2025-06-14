
#include "Mem.h"

Mem *speedMem = NULL;

void SpeedMemInit(int size, int div, UART_HandleTypeDef *huart)
{
	speedMem = (Mem *)malloc(sizeof(Mem));//Pay attention to you minimum heap setting. If it's too small, you'll get NULL
	speedMem->huart = huart;
	
	speedMem->lData = (double *)malloc(sizeof(double) * size);
	speedMem->rData = (double *)malloc(sizeof(double) * size);

	speedMem->size = size;
	
	speedMem->total = 0;
	speedMem->div = div;
	return;
}


void MemOut()
{
	char message[50];
	for(int i = 0; i < speedMem->total / speedMem->div; i ++)
	{
		sprintf(message, "%lf\t", speedMem->lData[i]);
		HAL_UART_Transmit(speedMem->huart, (uint8_t *)message, strlen(message), 1000);
	}
	for(int i = 0; i < speedMem->total / speedMem->div; i ++)
	{
		sprintf(message, "%lf\t", speedMem->rData[i]);
		HAL_UART_Transmit(speedMem->huart, (uint8_t *)message, strlen(message), 1000);
	}
}

void destoryMem()
{
    if (speedMem != NULL) {
        if (speedMem->lData != NULL) {
            free(speedMem->lData);
            speedMem->lData = NULL;
        }
        if (speedMem->rData != NULL) {
            free(speedMem->rData);
            speedMem->rData = NULL;
        }
        free(speedMem);
        speedMem = NULL;
    }
}
