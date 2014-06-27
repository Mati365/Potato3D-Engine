################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/G3Engine/Graphics/MatrixStack.cpp \
../src/G3Engine/Graphics/Shader.cpp \
../src/G3Engine/Graphics/Shape.cpp \
../src/G3Engine/Graphics/Texture.cpp 

OBJS += \
./src/G3Engine/Graphics/MatrixStack.o \
./src/G3Engine/Graphics/Shader.o \
./src/G3Engine/Graphics/Shape.o \
./src/G3Engine/Graphics/Texture.o 

CPP_DEPS += \
./src/G3Engine/Graphics/MatrixStack.d \
./src/G3Engine/Graphics/Shader.d \
./src/G3Engine/Graphics/Shape.d \
./src/G3Engine/Graphics/Texture.d 


# Each subdirectory must supply rules for building sources it contributes
src/G3Engine/Graphics/%.o: ../src/G3Engine/Graphics/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DGLEW_STATIC -I/usr/include -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


