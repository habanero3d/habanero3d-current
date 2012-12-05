################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AABB.cpp \
../src/AmbientLight.cpp \
../src/BVVisualizer.cpp \
../src/CFileStream.cpp \
../src/Camera.cpp \
../src/Color.cpp \
../src/DirectionalLight.cpp \
../src/Event.cpp \
../src/EventConverter.cpp \
../src/Exception.cpp \
../src/Frustum.cpp \
../src/GenericGeometry.cpp \
../src/HeightMap.cpp \
../src/IStream.cpp \
../src/Keyboard.cpp \
../src/LightShader.cpp \
../src/Logger.cpp \
../src/Loggers.cpp \
../src/MFCommon.cpp \
../src/Material.cpp \
../src/Mouse.cpp \
../src/Mutex.cpp \
../src/Node.cpp \
../src/OBB.cpp \
../src/OpenGL_Renderer.cpp \
../src/Pick.cpp \
../src/PointLight.cpp \
../src/RT.cpp \
../src/RenderThread.cpp \
../src/ResourceManager.cpp \
../src/SAF.cpp \
../src/SMF.cpp \
../src/SRT.cpp \
../src/Shader.cpp \
../src/ShaderFlags.cpp \
../src/ShaderProgram.cpp \
../src/SkeletalAnimatedObject.cpp \
../src/SkeletalAnimation.cpp \
../src/SkeletalAnimationState.cpp \
../src/SkeletalAnimationStateSet.cpp \
../src/SkeletalShader.cpp \
../src/Skeleton.cpp \
../src/SkeletonJoint.cpp \
../src/SkeletonJointKeyframeSequence.cpp \
../src/SkeletonVisualizer.cpp \
../src/SkinnedMesh.cpp \
../src/Spatial.cpp \
../src/Sphere.cpp \
../src/StaticMesh.cpp \
../src/StaticObject.cpp \
../src/StringUtil.cpp \
../src/SubMesh.cpp \
../src/TMF.cpp \
../src/Texture.cpp \
../src/Thread.cpp \
../src/Window.cpp \
../src/assert.cpp \
../src/matrix4.cpp \
../src/plane.cpp \
../src/psx_File.cpp \
../src/quaternion.cpp \
../src/vector2.cpp \
../src/vector3.cpp 

OBJS += \
./src/AABB.o \
./src/AmbientLight.o \
./src/BVVisualizer.o \
./src/CFileStream.o \
./src/Camera.o \
./src/Color.o \
./src/DirectionalLight.o \
./src/Event.o \
./src/EventConverter.o \
./src/Exception.o \
./src/Frustum.o \
./src/GenericGeometry.o \
./src/HeightMap.o \
./src/IStream.o \
./src/Keyboard.o \
./src/LightShader.o \
./src/Logger.o \
./src/Loggers.o \
./src/MFCommon.o \
./src/Material.o \
./src/Mouse.o \
./src/Mutex.o \
./src/Node.o \
./src/OBB.o \
./src/OpenGL_Renderer.o \
./src/Pick.o \
./src/PointLight.o \
./src/RT.o \
./src/RenderThread.o \
./src/ResourceManager.o \
./src/SAF.o \
./src/SMF.o \
./src/SRT.o \
./src/Shader.o \
./src/ShaderFlags.o \
./src/ShaderProgram.o \
./src/SkeletalAnimatedObject.o \
./src/SkeletalAnimation.o \
./src/SkeletalAnimationState.o \
./src/SkeletalAnimationStateSet.o \
./src/SkeletalShader.o \
./src/Skeleton.o \
./src/SkeletonJoint.o \
./src/SkeletonJointKeyframeSequence.o \
./src/SkeletonVisualizer.o \
./src/SkinnedMesh.o \
./src/Spatial.o \
./src/Sphere.o \
./src/StaticMesh.o \
./src/StaticObject.o \
./src/StringUtil.o \
./src/SubMesh.o \
./src/TMF.o \
./src/Texture.o \
./src/Thread.o \
./src/Window.o \
./src/assert.o \
./src/matrix4.o \
./src/plane.o \
./src/psx_File.o \
./src/quaternion.o \
./src/vector2.o \
./src/vector3.o 

CPP_DEPS += \
./src/AABB.d \
./src/AmbientLight.d \
./src/BVVisualizer.d \
./src/CFileStream.d \
./src/Camera.d \
./src/Color.d \
./src/DirectionalLight.d \
./src/Event.d \
./src/EventConverter.d \
./src/Exception.d \
./src/Frustum.d \
./src/GenericGeometry.d \
./src/HeightMap.d \
./src/IStream.d \
./src/Keyboard.d \
./src/LightShader.d \
./src/Logger.d \
./src/Loggers.d \
./src/MFCommon.d \
./src/Material.d \
./src/Mouse.d \
./src/Mutex.d \
./src/Node.d \
./src/OBB.d \
./src/OpenGL_Renderer.d \
./src/Pick.d \
./src/PointLight.d \
./src/RT.d \
./src/RenderThread.d \
./src/ResourceManager.d \
./src/SAF.d \
./src/SMF.d \
./src/SRT.d \
./src/Shader.d \
./src/ShaderFlags.d \
./src/ShaderProgram.d \
./src/SkeletalAnimatedObject.d \
./src/SkeletalAnimation.d \
./src/SkeletalAnimationState.d \
./src/SkeletalAnimationStateSet.d \
./src/SkeletalShader.d \
./src/Skeleton.d \
./src/SkeletonJoint.d \
./src/SkeletonJointKeyframeSequence.d \
./src/SkeletonVisualizer.d \
./src/SkinnedMesh.d \
./src/Spatial.d \
./src/Sphere.d \
./src/StaticMesh.d \
./src/StaticObject.d \
./src/StringUtil.d \
./src/SubMesh.d \
./src/TMF.d \
./src/Texture.d \
./src/Thread.d \
./src/Window.d \
./src/assert.d \
./src/matrix4.d \
./src/plane.d \
./src/psx_File.d \
./src/quaternion.d \
./src/vector2.d \
./src/vector3.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/akuda/Projekty/repos/habanero/HabaneroPort/src -I/home/akuda/Projekty/repos/habanero/HabaneroPort/../third-party/sfml2/include -I/home/akuda/Projekty/repos/habanero/HabaneroPort/../third-party/glew/include -O3 -g3 -Wall -c -fmessage-length=0 -include default.h -std=c++0x -Wno-unknown-pragmas -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


