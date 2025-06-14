#ifndef HALL_ENCODER_H
#define HALL_ENCODER_H

#include "main.h"
#include "tim.h"
#include "Mem.h"

#define WHEELLENGTH 66e-3 //The length of wheel
#define PPR 360 // The Pulses Per Revolution of encoder


#ifdef __cplusplus
extern "C" {
#endif

double getLeftSpeed(void);
double getRightSpeed(void);
uint32_t ReloadTime(TIM_TypeDef* tim);

void EncoderInit(TIM_HandleTypeDef *leftTimer, uint32_t chan1L, uint32_t chan2L,
	TIM_HandleTypeDef *rightTimer, uint32_t chan1R, uint32_t chan2R,
	TIM_HandleTypeDef *realTimer);

#endif // HALL_ENCODER_H
