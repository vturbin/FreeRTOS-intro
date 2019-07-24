/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <string.h>
			

static void prvSetupHardware(void);
static void prvSetupUart(void);
static void prvSetupGPIO(void);
void printmsg(char *msg);
char usr_msg[250];
void vTask1_handler(void* params);
void vTask2_handler(void* params);

//global space for variables
#define TRUE 1
#define FALSE 0

#define NOT_PRESSED FALSE
#define PRESSED TRUE

TaskHandle_t xTaskHandle1 = NULL;
TaskHandle_t xTaskHandle2 = NULL;

int main(void)
{
	DWT->CTRL |= (1<<0);  // enable cyccnt in dwt_ctrl

	// HSI = 16 MHz, hence System clock = 16 MHz
	RCC_DeInit();

	// Updates SystemCoreClock variable
	SystemCoreClockUpdate();

	prvSetupHardware();

	// Start Recording
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	// Let's create a led_task
	xTaskCreate(vTask1_handler, "Print_LEDSTATUS_ON_UART", 500, NULL, 2, &xTaskHandle1);
	xTaskCreate(vTask2_handler, "TOGGLE_LED", 500, NULL, 3, &xTaskHandle2);

	vTaskStartScheduler();

	for(;;);
}


static void prvSetupHardware(void)
{
	prvSetupUart();
	prvSetupGPIO();
}

void printmsg(char *msg)
{
	for(int i=0; i <strlen(msg); i++)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET);
		USART_SendData(USART2, msg[i]);
	}
}

void vTask1_handler(void* params)
{
	while(1)
	{
		sprintf(usr_msg,"Status of the LED is %d\n",GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13));
		printmsg(usr_msg);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void vTask2_handler(void* params)
{
	while(1)
	{
		GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void vApplicationIdleHook(void)
{
	__WFI();
}

static void prvSetupGPIO(void)
{
	GPIO_InitTypeDef led_init, button_init;

	// Enable GPIOD peripheral clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);


	//Enable clock for

	// initialize 0 to each element of the structure
	//initialize LED GPIO
	memset(&led_init,0,sizeof(led_init));
	led_init.GPIO_Pin = GPIO_Pin_13;
	led_init.GPIO_Mode = GPIO_Mode_OUT;
	led_init.GPIO_OType = GPIO_OType_PP;
	led_init.GPIO_Speed =  GPIO_Low_Speed;
	led_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD,&led_init);

	//initialize Button GPIO
	memset(&button_init,0,sizeof(led_init));
	button_init.GPIO_Pin = GPIO_Pin_0;
	button_init.GPIO_Mode = GPIO_Mode_IN;
	button_init.GPIO_OType = GPIO_OType_PP;
	button_init.GPIO_Speed =  GPIO_Low_Speed;
	button_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&button_init);
	// Interrupt configuration
/*
	//1. system configuration for exti line (SYSCFG settings)
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);

	//2. EXTI line configuration 0,falling edge, interrupt mode
	EXTI_InitTypeDef exti;
	exti.EXTI_Line = EXTI_Line0;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger =EXTI_Trigger_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	//3. Enable NVIC
	NVIC_SetPriority(EXTI0_IRQn, 5);
	NVIC_EnableIRQ(EXTI0_IRQn);
*/
}
/*
void EXTI0_IRQHandler(void)
{

	traceISR_ENTER();
	//1. clear the interrupt pending bit of the EXTI line (0)
	EXTI_ClearITPendingBit(EXTI_Line0);
	button_handler(NULL);
	traceISR_EXIT();
}
*/
static void prvSetupUart(void)
{
	GPIO_InitTypeDef gpio_uart_pins;
	USART_InitTypeDef uart2_init;

	// Enable UART and GPIOA peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	// PA2 is UART2_TX, PA3 is UART2_RX
	// Alternate function configuration of MCU pins to behave as UART2 TX and RX

	//initialize 0 to each element of the structure
	memset(&gpio_uart_pins,0, sizeof(gpio_uart_pins));
	gpio_uart_pins.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	gpio_uart_pins.GPIO_Mode = GPIO_Mode_AF;
	gpio_uart_pins.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &gpio_uart_pins);

	// AF mode seettings for the pins
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); // TX -- PA2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); // RX -- PA3

	// UART parameter initializations
	//initialize 0 to each element of the structure
	memset(&uart2_init,0, sizeof(uart2_init));
	uart2_init.USART_BaudRate = 115200;
	uart2_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart2_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	uart2_init.USART_Parity = USART_Parity_No;
	uart2_init.USART_StopBits = USART_StopBits_1;
	uart2_init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &uart2_init);

	// Enable the UART peripheral
	USART_Cmd(USART2, ENABLE);
}
