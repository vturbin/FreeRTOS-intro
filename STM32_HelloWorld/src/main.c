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
#include <stdint.h>

TaskHandle_t xTaskHandle1 = NULL;
TaskHandle_t xTaskHandle2 = NULL;

void vTask1_handler(void *params);
void vTask2_handler(void *params);

//used for semihosting
#ifdef USE_SEMIHOSTING
extern void initialise_monitor_handles();
#endif

#define TRUE 1
#define FALSE 0
#define AVAILABLE 1
#define NON_AVAILABLE 0

static void prvSetupHardware(void);
static void prvSetupUart(void);
void printmsg(char *msg);
char usr_msg[250];
uint8_t UART_ACCESS_KEY = AVAILABLE;

int main(void)
{

#ifdef USE_SEMIHOSTING
	initialise_monitor_handles();
	printf("This is hello world example code\n");
#endif

	DWT->CTRL |= (1<<0);  // enable cyccnt in dwt_ctrl
	/**
	  * @brief  Resets the RCC clock configuration to the default reset state.
	  * @note   The default reset state of the clock configuration is given below:
	  *            - HSI ON and used as system clock source
	  *            - HSE, PLL and PLLI2S OFF
	  *            - AHB, APB1 and APB2 prescaler set to 1.
	  *            - CSS, MCO1 and MCO2 OFF
	  *            - All interrupts disabled
	  * @note   This function doesn't modify the configuration of the
	  *            - Peripheral clocks
	  *            - LSI, LSE and RTC clocks
	  * @param  None
	  * @retval None
	  */
	// HSI = 16 MHz, hence System clock = 16 MHz
	RCC_DeInit();

	// Updates SystemCoreClock variable
	SystemCoreClockUpdate();

	prvSetupHardware();

	sprintf(usr_msg, "This is hello world application starting\r\n");
	printmsg(usr_msg);

	// Start Recording
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	// Create 2 tasks, task-1 and task-2

	xTaskCreate(vTask1_handler, "Task-1", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle1);
	xTaskCreate(vTask2_handler, "Task-2", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle2);

	/*	BaseType_t xTaskCreate(	TaskFunction_t pxTaskCode,					// Mentions the name of the task function here
								const char * const pcName,					// Give a descriptive name for the task e.g. "sensorDG"
								const configSTACK_DEPTH_TYPE usStackDepth,	// Stack size for this tasks(in words)
								void * const pvParameters,					// Parameter to pass the pointer of the data to the task function
								UBaseType_t uxPriority,						// Priority of the task
								TaskHandle_t * const pxCreatedTask )*/		// Handle of the task. Pointer to the created task in the memory


	// Start the scheduler
	vTaskStartScheduler();


	// you will never get here
	for(;;);


}

void vTask1_handler(void *params)
{
	while(1){
		if(UART_ACCESS_KEY == AVAILABLE)
		{
			UART_ACCESS_KEY = NON_AVAILABLE;
			printmsg("This is task 1\r\n");
			UART_ACCESS_KEY = AVAILABLE;
			taskYIELD();
		}

	}
}

void vTask2_handler(void *params)
{
	while(1)
	{
		if(UART_ACCESS_KEY == AVAILABLE)
		{
		UART_ACCESS_KEY = NON_AVAILABLE;
		printmsg("This is task 2\r\n");
		UART_ACCESS_KEY = AVAILABLE;
		taskYIELD();
		}
	}
}

void printmsg(char *msg)
{
	for(int i=0; i <strlen(msg); i++)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET);
		USART_SendData(USART2, msg[i]);
	}
}

static void prvSetupHardware(void)
{
	prvSetupUart();
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
