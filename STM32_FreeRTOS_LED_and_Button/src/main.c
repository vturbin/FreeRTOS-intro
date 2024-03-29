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
void led_task_handler(void* params);
void button_task_handler(void* params);

//global space for variables
#define TRUE 1
#define FALSE 0

#define NOT_PRESSED FALSE
#define PRESSED TRUE

uint8_t button_status_flag = NOT_PRESSED;
int main(void)
{

	// HSI = 16 MHz, hence System clock = 16 MHz
	RCC_DeInit();

	// Updates SystemCoreClock variable
	SystemCoreClockUpdate();

	prvSetupHardware();



	// Let's create a led_task
	xTaskCreate(led_task_handler, "LED_TASK", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(button_task_handler, "LED_TASK", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

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

void led_task_handler(void* params)
{
	while(1)
	{
		if(button_status_flag == PRESSED)
		{
			GPIO_WriteBit(GPIOD, GPIO_Pin_13,Bit_SET);
		}
		else {
			GPIO_WriteBit(GPIOD, GPIO_Pin_13,Bit_RESET);
		}
	}
}

void button_task_handler(void* params)
{
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
		{
			button_status_flag = PRESSED;
		}
		else
		{
			button_status_flag = NOT_PRESSED;
		}
	}
}

static void prvSetupGPIO(void)
{
	GPIO_InitTypeDef led_init, button_init;

	// Enable GPIOD peripheral clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

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

}

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

