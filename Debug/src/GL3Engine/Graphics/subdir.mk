################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/GL3Engine/Graphics/MatrixStack.cpp \
../src/GL3Engine/Graphics/MeshLoader.cpp \
../src/GL3Engine/Graphics/Primitives.cpp \
../src/GL3Engine/Graphics/Shader.cpp \
../src/GL3Engine/Graphics/Shape.cpp \
../src/GL3Engine/Graphics/Texture.cpp 

OBJS += \
./src/GL3Engine/Graphics/MatrixStack.o \
./src/GL3Engine/Graphics/MeshLoader.o \
./src/GL3Engine/Graphics/Primitives.o \
./src/GL3Engine/Graphics/Shader.o \
./src/GL3Engine/Graphics/Shape.o \
./src/GL3Engine/Graphics/Texture.o 

CPP_DEPS += \
./src/GL3Engine/Graphics/MatrixStack.d \
./src/GL3Engine/Graphics/MeshLoader.d \
./src/GL3Engine/Graphics/Primitives.d \
./src/GL3Engine/Graphics/Shader.d \
./src/GL3Engine/Graphics/Shape.d \
./src/GL3Engine/Graphics/Texture.d 


# Each subdirectory must supply rules for building sources it contributes
src/GL3Engine/Graphics/%.o: ../src/GL3Engine/Graphics/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DGLEW_STATIC -I/usr/include -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


