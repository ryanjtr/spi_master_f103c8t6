################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SPI_LIB/spi.c 

OBJS += \
./SPI_LIB/spi.o 

C_DEPS += \
./SPI_LIB/spi.d 


# Each subdirectory must supply rules for building sources it contributes
SPI_LIB/%.o SPI_LIB/%.su SPI_LIB/%.cyclo: ../SPI_LIB/%.c SPI_LIB/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32F103xB -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"F:/STM32project/modelization/spi_master_stm32f1/SPI_LIB" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-SPI_LIB

clean-SPI_LIB:
	-$(RM) ./SPI_LIB/spi.cyclo ./SPI_LIB/spi.d ./SPI_LIB/spi.o ./SPI_LIB/spi.su

.PHONY: clean-SPI_LIB

