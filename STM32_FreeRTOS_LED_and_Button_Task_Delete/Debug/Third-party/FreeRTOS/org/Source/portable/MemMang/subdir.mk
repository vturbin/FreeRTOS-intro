################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third-party/FreeRTOS/org/Source/portable/MemMang/heap_4.c 

OBJS += \
./Third-party/FreeRTOS/org/Source/portable/MemMang/heap_4.o 

C_DEPS += \
./Third-party/FreeRTOS/org/Source/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
Third-party/FreeRTOS/org/Source/portable/MemMang/%.o: ../Third-party/FreeRTOS/org/Source/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F407G_DISC1 -DDEBUG -DSTM32F40XX -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -I"C:/Users/vturb/RTOS_Workspace/STM32_FreeRTOS_LED_and_Button_Task_Priority_Reverse/StdPeriph_Driver/inc" -I"C:/Users/vturb/RTOS_Workspace/STM32_FreeRTOS_LED_and_Button_Task_Priority_Reverse/Third-party/SEGGER/SEGGER" -I"C:/Users/vturb/RTOS_Workspace/STM32_FreeRTOS_LED_and_Button_Task_Priority_Reverse/Third-party/SEGGER/Config" -I"C:/Users/vturb/RTOS_Workspace/STM32_FreeRTOS_LED_and_Button_Task_Priority_Reverse/Third-party/SEGGER/OS" -I"C:/Users/vturb/RTOS_Workspace/STM32_FreeRTOS_LED_and_Button_Task_Priority_Reverse/Third-party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"C:/Users/vturb/RTOS_Workspace/STM32_FreeRTOS_LED_and_Button_Task_Priority_Reverse/Config" -I"C:/Users/vturb/RTOS_Workspace/STM32_FreeRTOS_LED_and_Button_Task_Priority_Reverse/Third-party/FreeRTOS/org/Source/include" -I"C:/Users/vturb/RTOS_Workspace/STM32_FreeRTOS_LED_and_Button_Task_Priority_Reverse/inc" -I"C:/Users/vturb/RTOS_Workspace/STM32_FreeRTOS_LED_and_Button_Task_Priority_Reverse/CMSIS/device" -I"C:/Users/vturb/RTOS_Workspace/STM32_FreeRTOS_LED_and_Button_Task_Priority_Reverse/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


