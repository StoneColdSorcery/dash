################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/STM32Cube/STM32Cube_FW_F4_V1.1.0/Drivers/BSP/STM32F401-Discovery/stm32f401_discovery.c \
D:/STM32Cube/STM32Cube_FW_F4_V1.1.0/Drivers/BSP/STM32F401-Discovery/stm32f401_discovery_accelerometer.c \
D:/STM32Cube/STM32Cube_FW_F4_V1.1.0/Drivers/BSP/STM32F401-Discovery/stm32f401_discovery_gyroscope.c 

OBJS += \
./STM32F4_StdPeriphLibrary/stm32f401_discovery.o \
./STM32F4_StdPeriphLibrary/stm32f401_discovery_accelerometer.o \
./STM32F4_StdPeriphLibrary/stm32f401_discovery_gyroscope.o 

C_DEPS += \
./STM32F4_StdPeriphLibrary/stm32f401_discovery.d \
./STM32F4_StdPeriphLibrary/stm32f401_discovery_accelerometer.d \
./STM32F4_StdPeriphLibrary/stm32f401_discovery_gyroscope.d 


# Each subdirectory must supply rules for building sources it contributes
STM32F4_StdPeriphLibrary/stm32f401_discovery.o: D:/STM32Cube/STM32Cube_FW_F4_V1.1.0/Drivers/BSP/STM32F401-Discovery/stm32f401_discovery.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F401xC -I"C:/Users/Crazyfingers/ARM Workspaces/STM32F4disc/Template_Project/inc" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Drivers\CMSIS\Include" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Utilities\CPU" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Drivers\STM32F4xx_HAL_Driver\Inc" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Drivers\BSP\STM32F401-Discovery" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Middlewares\ST\STM32_USB_Device_Library\Core\Inc" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Middlewares\ST\STM32_USB_Device_Library\Class" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Middlewares\ST\STM32_USB_Device_Library\Class\HID\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

STM32F4_StdPeriphLibrary/stm32f401_discovery_accelerometer.o: D:/STM32Cube/STM32Cube_FW_F4_V1.1.0/Drivers/BSP/STM32F401-Discovery/stm32f401_discovery_accelerometer.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F401xC -I"C:/Users/Crazyfingers/ARM Workspaces/STM32F4disc/Template_Project/inc" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Drivers\CMSIS\Include" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Utilities\CPU" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Drivers\STM32F4xx_HAL_Driver\Inc" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Drivers\BSP\STM32F401-Discovery" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Middlewares\ST\STM32_USB_Device_Library\Core\Inc" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Middlewares\ST\STM32_USB_Device_Library\Class" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Middlewares\ST\STM32_USB_Device_Library\Class\HID\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

STM32F4_StdPeriphLibrary/stm32f401_discovery_gyroscope.o: D:/STM32Cube/STM32Cube_FW_F4_V1.1.0/Drivers/BSP/STM32F401-Discovery/stm32f401_discovery_gyroscope.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F401xC -I"C:/Users/Crazyfingers/ARM Workspaces/STM32F4disc/Template_Project/inc" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Drivers\CMSIS\Include" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Utilities\CPU" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Drivers\STM32F4xx_HAL_Driver\Inc" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Drivers\BSP\STM32F401-Discovery" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Middlewares\ST\STM32_USB_Device_Library\Core\Inc" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Middlewares\ST\STM32_USB_Device_Library\Class" -I"D:\STM32Cube\STM32Cube_FW_F4_V1.1.0\Middlewares\ST\STM32_USB_Device_Library\Class\HID\Inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


