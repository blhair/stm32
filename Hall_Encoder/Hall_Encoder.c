

#include "Hall_Encoder.h"

int leftSpeed, rightSpeed;// Variables to get the increment from encoders, not real speed
uint32_t reloadFre;
TIM_HandleTypeDef *leftT; //Timer you set for left encoder
TIM_HandleTypeDef *rightT; //Timer you set for right encoder
TIM_HandleTypeDef *reload_tim; //Timer you set as real-time clock
extern Mem *speedMem;

void EncoderInit(TIM_HandleTypeDef *leftTimer, uint32_t chan1L, uint32_t chan2L,
	TIM_HandleTypeDef *rightTimer, uint32_t chan1R, uint32_t chan2R,
	TIM_HandleTypeDef *realTimer)
	//set global variables for encoders
{
	leftT = leftTimer;
	rightT = rightTimer;
	reload_tim = realTimer;
	
	HAL_TIM_PWM_Start(leftTimer,chan1L);//?
  HAL_TIM_PWM_Start(leftTimer,chan2L);//?
  HAL_TIM_PWM_Start(rightTimer,chan1R);//?
  HAL_TIM_PWM_Start(rightTimer,chan2R);//?
	
  __HAL_TIM_CLEAR_FLAG(realTimer,TIM_FLAG_UPDATE);
  HAL_TIM_Base_Start_IT(realTimer);
	
	reloadFre = ReloadTime(realTimer->Instance);
	return;
}

int getTIMx_DetaCnt(TIM_HandleTypeDef *htim)
{
    int cnt;
    cnt = htim->Instance->CNT - 0x7FFF;
    htim->Instance->CNT = 0x7FFF;
    return cnt;
}

void Get_Motor_Speed(int *leftSpeed, int *rightSpeed)
{
	*leftSpeed = getTIMx_DetaCnt(leftT);
	*rightSpeed = getTIMx_DetaCnt(rightT);

	if(speedMem != NULL)
	{
		if(speedMem->total >= speedMem->size * speedMem->div)
		{
			return;
		}
		
		if(speedMem->total % speedMem->div == 0)
		{
			speedMem->lData[speedMem->total / speedMem->div] = getLeftSpeed();
			speedMem->rData[speedMem->total / speedMem->div] = getRightSpeed();
		}
		speedMem->total ++;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == reload_tim)
    {
        Get_Motor_Speed(&leftSpeed, &rightSpeed);
    }
}

//Get wheels' speed with the unit of m/s
//'+' means forwards and '-' means backwards.
//When you are using the code, please connect PE9 to E1A, PE11 to E1B, 
double getLeftSpeed()
{
	
	return leftSpeed * 
					6.6e-2 * 3.1415926 * 
					reloadFre / (360 * 2);
		//4.394215799e-3; 
		// Left number is computed by (length of the circle) * (frequence of the clock / (leftSpeed add number for every single circle
		// which is (6.6e-2 * pi * (240e6 / ((65535 + 1) * (239 + 1)) / (360 * 2))
}
double getRightSpeed()
{
	return rightSpeed * 
					6.6e-2 * 3.1415926 * 
					reloadFre / (360 * 2);
}

uint32_t ReloadTime(TIM_TypeDef* tim)
{
	uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
	uint32_t tim_clk;
	
	uint32_t ppre1_bits = (RCC->CFGR >> 10) & 0x7;

	if (ppre1_bits == 0x0) {
			tim_clk = pclk1 * 2; // APB1 ?? -> ?????
	} else {
			tim_clk = pclk1;
	}
	uint32_t psc = tim->PSC;
	uint32_t arr = tim->ARR;
	
	return tim_clk / ((psc + 1) * (arr + 1));
}
