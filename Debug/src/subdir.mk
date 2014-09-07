################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/3D\ engine.cpp 

OBJS += \
./src/3D\ engine.o 

CPP_DEPS += \
./src/3D\ engine.d 


# Each subdirectory must supply rules for building sources it contributes
src/3D\ engine.o: ../src/3D\ engine.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DGLEW_STATIC -DDEBUG -I/usr/include -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -std=c++11 -std=c++1y -MMD -MP -MF"src/3D engine.d" -MT"src/3D\ engine.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


