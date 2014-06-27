################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/G3Engine/IO.cpp \
../src/G3Engine/Tools.cpp 

OBJS += \
./src/G3Engine/IO.o \
./src/G3Engine/Tools.o 

CPP_DEPS += \
./src/G3Engine/IO.d \
./src/G3Engine/Tools.d 


# Each subdirectory must supply rules for building sources it contributes
src/G3Engine/%.o: ../src/G3Engine/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DGLEW_STATIC -I/usr/include -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


