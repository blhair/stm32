#ifndef MEM_H
#define MEM_H

#include "main.h"
#include "usart.h"
#include "tim.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	double *lData;
	double *rData;
	int size;
	int div;
	int total;
	UART_HandleTypeDef *huart;
}Mem;

#ifdef __cplusplus
extern "C" {
#endif

void SpeedMemInit(int size, int div, UART_HandleTypeDef *huart);
void MemOut(void);
void destoryMem(void);

#endif // MEM_H
