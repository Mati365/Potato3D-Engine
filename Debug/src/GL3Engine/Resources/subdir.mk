################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/GL3Engine/Resources/ASCIIMeshLoader.cpp \
../src/GL3Engine/Resources/GLSLloader.cpp \
../src/GL3Engine/Resources/GlobalResourceManager.cpp \
../src/GL3Engine/Resources/INIParser.cpp \
../src/GL3Engine/Resources/OBJloader.cpp \
../src/GL3Engine/Resources/RequiredResources.cpp \
../src/GL3Engine/Resources/ResourceManager.cpp 

OBJS += \
./src/GL3Engine/Resources/ASCIIMeshLoader.o \
./src/GL3Engine/Resources/GLSLloader.o \
./src/GL3Engine/Resources/GlobalResourceManager.o \
./src/GL3Engine/Resources/INIParser.o \
./src/GL3Engine/Resources/OBJloader.o \
./src/GL3Engine/Resources/RequiredResources.o \
./src/GL3Engine/Resources/ResourceManager.o 

CPP_DEPS += \
./src/GL3Engine/Resources/ASCIIMeshLoader.d \
./src/GL3Engine/Resources/GLSLloader.d \
./src/GL3Engine/Resources/GlobalResourceManager.d \
./src/GL3Engine/Resources/INIParser.d \
./src/GL3Engine/Resources/OBJloader.d \
./src/GL3Engine/Resources/RequiredResources.d \
./src/GL3Engine/Resources/ResourceManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/GL3Engine/Resources/%.o: ../src/GL3Engine/Resources/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DGLEW_STATIC -I/usr/include -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -std=c++11 -std=c++1y -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


