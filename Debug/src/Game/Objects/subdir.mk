################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Game/Objects/Map.cpp 

OBJS += \
./src/Game/Objects/Map.o 

CPP_DEPS += \
./src/Game/Objects/Map.d 


# Each subdirectory must supply rules for building sources it contributes
src/Game/Objects/%.o: ../src/Game/Objects/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DGLEW_STATIC -I/usr/include -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -std=c++11 -std=c++1y -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


