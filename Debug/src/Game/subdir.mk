################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Game/Game.cpp 

OBJS += \
./src/Game/Game.o 

CPP_DEPS += \
./src/Game/Game.d 


# Each subdirectory must supply rules for building sources it contributes
src/Game/%.o: ../src/Game/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DGLEW_STATIC -DDEBUG -I/usr/include -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -std=c++11 -std=c++1y -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


