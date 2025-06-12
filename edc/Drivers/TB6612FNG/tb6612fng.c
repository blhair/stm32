/*
		Author Qinghan Yang
		Date 6.9.2025
		
		Program for controlling motor by tb6612fng
		Structure and enum definitions are in .h file.
		Functions are in .c files.
*/

#include "tb6612fng.h"


int leftSpeed, rightSpeed;// Variables to get the increment from encoders, not real speed
uint32_t reloadFre;
TIM_HandleTypeDef *leftT; //Timer you set for left encoder
TIM_HandleTypeDef *rightT; //Timer you set for right encoder
TIM_HandleTypeDef *reload_tim; //Timer you set as real-time clock

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

void MotorInit(Motor* M,	//In the Init function, the motor was set in sleep modle.
		GPIO_TypeDef* p_port, //The GPIO group of the positive pin. Example: GPIOA for PA1
		uint16_t p_pin,				//The number of pin. Example: GPIO_PIN_1 for PA1
		GPIO_TypeDef* n_port,
		uint16_t n_pin,
		TIM_HandleTypeDef* pwm_timer,  //The pointer of pwm_timer  
    uint32_t pwm_channel,          //The channel of pwm pin you use
    uint16_t current_duty)				 //Initial value of the duty.
{
	if(M == NULL || pwm_timer == NULL)
	{
		HAL_UART_Transmit(&huart1, (uint8_t *)"test3", 5, 1000);
		return;
	}
	M->p.port = p_port;
	M->p.pin = p_pin;
	M->n.port = n_port;
	M->n.pin = n_pin;
	M->speed.pwm_timer = pwm_timer;
	M->speed.pwm_channel = pwm_channel;
	M->speed.current_duty = current_duty;
	

	HAL_GPIO_WritePin(M->n.port, M->n.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(M->p.port, M->p.pin, GPIO_PIN_RESET);
	HAL_TIM_PWM_Start(M->speed.pwm_timer, M->speed.pwm_channel);
	__HAL_TIM_SET_COMPARE(M->speed.pwm_timer, M->speed.pwm_channel, M->speed.current_duty);
}
	
void MotorSet(MOVETYPE type, uint16_t duty, Motor* M)
	//duty is the value between 1 to 1000.
	//Regarding that the speed and the duty is not in perfect linear function, 
	//I use integer variable(duty) instead of float variable(speed) to control the speed.
{
	if (M == NULL) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"NULL POINTER DETECTED!\r\n", 25, 1000);
        return;
    }
	switch(type)
	{
		case FOR:
			HAL_GPIO_WritePin(M->p.port, M->p.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(M->n.port, M->n.pin, GPIO_PIN_RESET);
		break;
		case BACK:
			HAL_GPIO_WritePin(M->p.port, M->p.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(M->n.port, M->n.pin, GPIO_PIN_SET);
		break;
		case BREAK:
			HAL_GPIO_WritePin(M->p.port, M->p.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(M->n.port, M->n.pin, GPIO_PIN_SET);
		break;
		case SLIDE:
			HAL_GPIO_WritePin(M->p.port, M->p.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(M->n.port, M->n.pin, GPIO_PIN_RESET);
		break;
		case SLEEP:
		break;
	}
	M->speed.current_duty = duty;
	__HAL_TIM_SET_COMPARE(M->speed.pwm_timer, M->speed.pwm_channel, M->speed.current_duty);
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
