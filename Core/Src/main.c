/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#define pwm_ 350
#define time_ 8
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern int16_t encoder_cnt; 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */



 float kp_p=-0.05;
 float ki_p=0;
 float kd_p=-0.8;
 float outmax=100;
 float outmin=-100;
 uint16_t count=0;
 float error_p=0;
 float target_p=0;
 float position=0;
 float sum_p=0;
 float error_p0=0;
 
 
 
 uint32_t time=0;
 uint32_t counttime=10;
 


 float kp_a=0.7;
 float ki_a=0;
 float kd_a=1;
 float target_ang=2010;
 float error0a=0.0f;
 float error0b=0.0f;
 float suma=0.0f;
 float kp=10;
 float ki=1;
 float kd=5;
 float error0=0.0f;
 float current=0.0f;
 float target=0.0f;
 float targetold=0.0f;
 float output=0.0f;
 float error1=0.0f;
 float sum=0.0f;
 float pwm=0.0f;
 int16_t encoder_cnt = 0;
 int16_t encoder_position = 0;
 int16_t encoder_position1 = 0;
 int16_t encoder_position2 = 0;
 int16_t angle=0;
 uint8_t Z_Phase_Trigger=0;
 uint8_t yes=0;
 uint8_t flag=0;
 
 
 
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//float pidcon(PID_Handle *pid, float actual);
	
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int fputc(int ch, FILE *f)
{
  // 发�?�一个字节数据到串口USART1
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);  // 0xFFFF是超时时间，避免阻塞
  return ch;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
 HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
 HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
 HAL_TIM_Base_Start_IT(&htim3);
 HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
	
    /* USER CODE END WHILE */
		
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_5) // 检测PB8（Z相）中断
  { 
    // 消抖（避免机械抖动误触发）
    
      Z_Phase_Trigger = 1; // 置位零位触发标志
  }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==TIM3){
		if(yes==1){
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
			encoder_cnt = (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
			position +=encoder_cnt ;
			__HAL_TIM_SET_COUNTER(&htim2, 0);
			count++;
			if(count>10){
				count=0;
				error_p0 =error_p; 
				error_p=position-target_p ;
				sum_p+=error_p ;
				
				
			if(sum_p > 200) sum_p = 200;
			if(sum_p < -200) sum_p = -200;
				
			float delta_ang = kp_p * error_p + ki_p * sum_p + kd_p*(error_p - error_p0);
//			if(delta_ang > 50) delta_ang = 50;
//      if(delta_ang < -50) delta_ang = -50;
				
			target_ang=delta_ang+2010;
			
			
			
			
			
			}
		encoder_position = (int16_t)__HAL_TIM_GET_COUNTER(&htim4);
		angle+=encoder_position;
		__HAL_TIM_SET_COUNTER(&htim4, 0);
//		if(Z_Phase_Trigger==1){
//		angle=0;
//	  Z_Phase_Trigger=0;}
		
		targetold=target;
		

		error0b=error0a;
		error0a=target_ang-angle;
		if(error0a<3&&error0a>-3)//小角度
		error0a=0;
		suma+=error0a;
		if(suma > 2000) suma = 2000;
    if(suma < -2000) suma = -2000;
		if(target>0&&targetold<0)
			suma=0;
		if(target<0&&targetold>0)
			suma=0;
		
		
		
		target=kp_a*error0a+ki_a*suma+kd_a*(error0a-error0b);
		
		
		
		//速度环控�?
		//�?要写�?个if使积分项清零，不过在未接入角度传感器以前先不�?
//		
		current = (-encoder_cnt*60.0) / (44 * 30.0f * 0.01f);
			if((target>0&&targetold<0)||(target<0&&targetold>0))
				sum=0;//让积分项归零
		error1=error0;
		error0=target-current;
		sum+=error0;
		output=kp*error0+ki*sum+kd*(error0-error1);
			

			
		if(sum > 1000) sum = 1000;
    if(sum < -1000) sum = -1000;
		if (output>1000) output = 1000;
		if (output<-1000) output = -1000;	
    if (output<0){
			pwm=-output;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		}else{
			pwm=output;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		}
   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1 , (uint16_t)pwm);
//		
	 printf("%d,%.2f,%.2f,%.2f,%d,%d,%f,%f\r\n",
               encoder_cnt,       // 编码�?10ms计数
               current,           // 当前转�?�（rpm�?
               error0,            // 目标-实际误差
               output,            // PID原始输出
               (uint16_t)output,
		           angle,
							 target,
		           position 
		); 
		if(error0a>800||error0a<-800){
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
			
		}
		
		
	}
		////
	//////
		if(yes==0){
		   if(flag==0){
			 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1 ,pwm_ );
		
			 flag=1;
			 }
		  encoder_position2=encoder_position1;
		  encoder_position1=encoder_position; 
			encoder_position = (int16_t)__HAL_TIM_GET_COUNTER(&htim4);
		  angle+=encoder_position;
		  __HAL_TIM_SET_COUNTER(&htim4, 0);
			

			 
			 
			 if(angle-target_ang<250&&angle-target_ang>-250){
				 yes=1;
				   __HAL_TIM_SET_COUNTER(&htim2, 0);
					encoder_cnt = 0;
				sum_p = 0.0f;   // 位置环积分项清零（最关键）
				error_p = 0.0f; // 位置环误差清零
				error_p0 = 0.0f;// 位置环上一次误差清零
				count = 0;      // 位置环计数器重置，重新计时100ms
				// 角度环变量（清零积分+误差，解决角度环输出异常）
				suma = 0.0f;    // 角度环积分项清零
				error0a = 0.0f; // 角度环当前误差清零
				error0b = 0.0f; // 角度环上一次误差清零
				// 速度环变量（清零积分+误差，解决速度骤增）
				sum = 0.0f;     // 速度环积分项清零
				error0 = 0.0f;  // 速度环当前误差清零
				error1 = 0.0f;  // 速度环上一次误差清零
				// 转向目标变量（清零，保证积分清零逻辑正常触发）
				targetold = 0.0f;
				target = 0.0f;  // 速度环目标转速清零，从0开始加速 
				 
				 
				
		 }
			 if(angle+target_ang<250&&angle+target_ang>-250){
				 yes=1;
				angle=target_ang+angle+target_ang ;
				 __HAL_TIM_SET_COUNTER(&htim2, 0);
				encoder_cnt = 0;
				sum_p = 0.0f;
				error_p = 0.0f;
				error_p0 = 0.0f;
				count = 0;
				suma = 0.0f;
				error0a = 0.0f;
				error0b = 0.0f;
				sum = 0.0f;
				error0 = 0.0f;
				error1 = 0.0f;
				targetold = 0.0f;
				target = 0.0f;
				 
				 
				 
			
				 
				 
		 }
		
		 //
		 if(flag==1){
			 counttime--;
			 if(counttime==0){
				 counttime=time_;
				 flag=2;}}
		 //
		 if(flag==2){

			 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1 ,0);
		 if(encoder_position2<0&&encoder_position>0){
			 flag=4; 
		 }
		if(encoder_position2>0&&encoder_position<0){
			flag=3;
		}
		 
		 
		 }
		 //
		 if(flag==3){
			 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
			 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1 ,pwm_);
			 flag=31;
		 }
		 if(flag==31){
			 counttime--;
			 if(counttime==0){
				 counttime=time_ ;
				 flag=2;}}
			 if(flag==32){
		   
			 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
			 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1 ,pwm_);
			 flag=33;
			 }
		   if(flag==33){
			 counttime--;
			 if(counttime==0){
				 counttime=time_;
				 flag=2;}}
		 if(flag==4){
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
			 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1 ,pwm_);
		   flag=41;
		 }
		 if(flag==41){
			  counttime--;
			 if(counttime==0){
				 counttime=time_;
				 flag=2;}}
		 if(flag==42){
			 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
			 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1 ,pwm_);
			 flag=43;}
		 if(flag==43){
			  counttime--;
			 if(counttime==0){
				 counttime=time_;
				 flag=2;}}
		 printf("%d,%.2f,%.2f,%.2f,%d,%d,%f,%f\r\n",
               encoder_cnt,       // 编码�?10ms计数
               current,           // 当前转�?�（rpm�?
               error0,            // 目标-实际误差
               output,            // PID原始输出
               (uint16_t)output,
		           angle,
							 target,
		           position 
		); 

		
	 }
		
		
		}
	
	
	
	
	
	
	
	
	
	
}
 
	
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
