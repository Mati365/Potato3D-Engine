################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/GL3Engine/Graphics/FBO.cpp \
../src/GL3Engine/Graphics/MatMatrix.cpp \
../src/GL3Engine/Graphics/Material.cpp \
../src/GL3Engine/Graphics/Matrix.cpp \
../src/GL3Engine/Graphics/MatrixStack.cpp \
../src/GL3Engine/Graphics/Mesh.cpp \
../src/GL3Engine/Graphics/Point.cpp \
../src/GL3Engine/Graphics/Primitives.cpp \
../src/GL3Engine/Graphics/Shader.cpp \
../src/GL3Engine/Graphics/Text.cpp \
../src/GL3Engine/Graphics/Texture.cpp \
../src/GL3Engine/Graphics/VAOpolygon.cpp 

OBJS += \
./src/GL3Engine/Graphics/FBO.o \
./src/GL3Engine/Graphics/MatMatrix.o \
./src/GL3Engine/Graphics/Material.o \
./src/GL3Engine/Graphics/Matrix.o \
./src/GL3Engine/Graphics/MatrixStack.o \
./src/GL3Engine/Graphics/Mesh.o \
./src/GL3Engine/Graphics/Point.o \
./src/GL3Engine/Graphics/Primitives.o \
./src/GL3Engine/Graphics/Shader.o \
./src/GL3Engine/Graphics/Text.o \
./src/GL3Engine/Graphics/Texture.o \
./src/GL3Engine/Graphics/VAOpolygon.o 

CPP_DEPS += \
./src/GL3Engine/Graphics/FBO.d \
./src/GL3Engine/Graphics/MatMatrix.d \
./src/GL3Engine/Graphics/Material.d \
./src/GL3Engine/Graphics/Matrix.d \
./src/GL3Engine/Graphics/MatrixStack.d \
./src/GL3Engine/Graphics/Mesh.d \
./src/GL3Engine/Graphics/Point.d \
./src/GL3Engine/Graphics/Primitives.d \
./src/GL3Engine/Graphics/Shader.d \
./src/GL3Engine/Graphics/Text.d \
./src/GL3Engine/Graphics/Texture.d \
./src/GL3Engine/Graphics/VAOpolygon.d 


# Each subdirectory must supply rules for building sources it contributes
src/GL3Engine/Graphics/%.o: ../src/GL3Engine/Graphics/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DGLEW_STATIC -I/usr/include -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -std=c++11 -std=c++1y -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


