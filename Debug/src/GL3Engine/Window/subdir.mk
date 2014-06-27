################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/GL3Engine/Window/GL3Renderer.cpp \
../src/GL3Engine/Window/Window.cpp 

OBJS += \
./src/GL3Engine/Window/GL3Renderer.o \
./src/GL3Engine/Window/Window.o 

CPP_DEPS += \
./src/GL3Engine/Window/GL3Renderer.d \
./src/GL3Engine/Window/Window.d 


# Each subdirectory must supply rules for building sources it contributes
src/GL3Engine/Window/%.o: ../src/GL3Engine/Window/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DGLEW_STATIC -I/usr/include -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


