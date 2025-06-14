# Hall_Encoder 库说明

本库用于基于 STM32 的霍尔编码器测速，适用于双轮差速小车等场景。通过定时器和编码器接口，实时获取左右轮速度，并支持速度数据存储。

---

## 主要函数用法

### 1. EncoderInit

```c
void EncoderInit(
    TIM_HandleTypeDef *leftTimer, uint32_t chan1L, uint32_t chan2L,
    TIM_HandleTypeDef *rightTimer, uint32_t chan1R, uint32_t chan2R,
    TIM_HandleTypeDef *realTimer
);
```

**功能说明：**  
初始化编码器相关的定时器和通道，设置全局变量，启动 PWM 和定时中断。

**参数说明：**
- `leftTimer`：左轮编码器用的定时器句柄指针
- `chan1L`、`chan2L`：左轮编码器的两个通道编号（如 `TIM_CHANNEL_1`、`TIM_CHANNEL_2`）
- `rightTimer`：右轮编码器用的定时器句柄指针
- `chan1R`、`chan2R`：右轮编码器的两个通道编号
- `realTimer`：用于定时采样速度的定时器句柄指针

**使用示例：**
```c
EncoderInit(&htim1, TIM_CHANNEL_1, TIM_CHANNEL_2, &htim2, TIM_CHANNEL_1, TIM_CHANNEL_2, &htim3);
```

---

### 2. getLeftSpeed

```c
double getLeftSpeed(void);
```

**功能说明：**  
获取左轮当前速度，单位为 m/s。返回值为正表示前进，负表示后退。

**使用示例：**
```c
double left_speed = getLeftSpeed();
printf("左轮速度: %f m/s\n", left_speed);
```

---

### 3. getRightSpeed

```c
double getRightSpeed(void);
```

**功能说明：**  
获取右轮当前速度，单位为 m/s。返回值为正表示前进，负表示后退。

**使用示例：**
```c
double right_speed = getRightSpeed();
printf("右轮速度: %f m/s\n", right_speed);
```

---

## 其他函数简介

- `int getTIMx_DetaCnt(TIM_HandleTypeDef *htim)`  
  获取定时器自上次调用后的计数变化量，并重置计数器。用于计算编码器增量。

- `void Get_Motor_Speed(int *leftSpeed, int *rightSpeed)`  
  读取左右轮编码器的增量，并可将速度数据存入外部定义的 `speedMem` 缓存。

- `void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)`  
  定时器溢出中断回调，自动调用 `Get_Motor_Speed` 实现定时采样。

- `uint32_t ReloadTime(TIM_TypeDef* tim)`  
  计算定时器的实际溢出频率，用于速度换算。

---

## 注意事项

- 使用前需正确配置定时器和编码器接口，并确保 `main.h`、`tim.h`、`Mem.h` 等头文件可用。
- 速度采样依赖于定时器中断，请确保 `HAL_TIM_Base_Start_IT` 已启动。
- `speedMem` 结构体需用户自行实现和初始化（用于速度数据记录）。

---

如有疑问或需进一步扩展，请参考源码注释或联系开发者。