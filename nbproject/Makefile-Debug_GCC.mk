#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug_GCC
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/Color.o \
	${OBJECTDIR}/src/Keyboard.o \
	${OBJECTDIR}/src/HeightMapObject.o \
	${OBJECTDIR}/src/SkeletalAnimatedObject.o \
	${OBJECTDIR}/src/MemoryManager.o \
	${OBJECTDIR}/src/Renderer.o \
	${OBJECTDIR}/src/ShaderProgram.o \
	${OBJECTDIR}/src/Polyhedron.o \
	${OBJECTDIR}/src/IResource.o \
	${OBJECTDIR}/src/LUP.o \
	${OBJECTDIR}/src/MFCommon.o \
	${OBJECTDIR}/src/ShaderManager.o \
	${OBJECTDIR}/src/AmbientLight.o \
	${OBJECTDIR}/src/SCF.o \
	${OBJECTDIR}/src/ResourceManager.o \
	${OBJECTDIR}/src/File.o \
	${OBJECTDIR}/src/HMF.o \
	${OBJECTDIR}/src/SRT.o \
	${OBJECTDIR}/src/PointLight.o \
	${OBJECTDIR}/src/SkeletonJoint.o \
	${OBJECTDIR}/src/Skeleton.o \
	${OBJECTDIR}/src/FrameDescription.o \
	${OBJECTDIR}/src/GenericGeometry.o \
	${OBJECTDIR}/src/SubMesh.o \
	${OBJECTDIR}/src/SkeletalAnimation.o \
	${OBJECTDIR}/src/StringUtil.o \
	${OBJECTDIR}/src/SkeletalShader.o \
	${OBJECTDIR}/src/Camera.o \
	${OBJECTDIR}/src/AABB.o \
	${OBJECTDIR}/src/Spatial.o \
	${OBJECTDIR}/src/RootObject.o \
	${OBJECTDIR}/src/StaticObject.o \
	${OBJECTDIR}/src/LogManager.o \
	${OBJECTDIR}/src/RT.o \
	${OBJECTDIR}/src/MTF.o \
	${OBJECTDIR}/src/SAF.o \
	${OBJECTDIR}/src/ShaderFlags.o \
	${OBJECTDIR}/src/Frustum.o \
	${OBJECTDIR}/src/Mouse.o \
	${OBJECTDIR}/src/DirectionalLight.o \
	${OBJECTDIR}/src/SkeletonJointKeyframeSequence.o \
	${OBJECTDIR}/src/LightShader.o \
	${OBJECTDIR}/src/IStream.o \
	${OBJECTDIR}/src/StackAllocator.o \
	${OBJECTDIR}/src/line3.o \
	${OBJECTDIR}/src/RendererLightSetup.o \
	${OBJECTDIR}/src/GenericMemoryOrdersGenerator.o \
	${OBJECTDIR}/src/TMF.o \
	${OBJECTDIR}/src/StaticMesh.o \
	${OBJECTDIR}/src/Pick.o \
	${OBJECTDIR}/src/ref.o \
	${OBJECTDIR}/src/IManagedObject.o \
	${OBJECTDIR}/src/plane.o \
	${OBJECTDIR}/src/CullingVolumesSet.o \
	${OBJECTDIR}/src/matrix4.o \
	${OBJECTDIR}/src/vector3.o \
	${OBJECTDIR}/src/Window.o \
	${OBJECTDIR}/src/CFileStream.o \
	${OBJECTDIR}/src/Exception.o \
	${OBJECTDIR}/src/assert.o \
	${OBJECTDIR}/src/Texture.o \
	${OBJECTDIR}/src/vector2.o \
	${OBJECTDIR}/src/Node.o \
	${OBJECTDIR}/src/SkeletalAnimationState.o \
	${OBJECTDIR}/src/Logger.o \
	${OBJECTDIR}/src/Material.o \
	${OBJECTDIR}/src/psx_File.o \
	${OBJECTDIR}/src/Event.o \
	${OBJECTDIR}/src/HeightMapMesh.o \
	${OBJECTDIR}/src/SkinnedMesh.o \
	${OBJECTDIR}/src/quaternion.o \
	${OBJECTDIR}/src/SkeletalAnimationStateSet.o \
	${OBJECTDIR}/src/SMF.o \
	${OBJECTDIR}/src/RendererSkeletonSetup.o \
	${OBJECTDIR}/src/EventConverter.o \
	${OBJECTDIR}/src/Sphere.o \
	${OBJECTDIR}/src/ModelFactory.o \
	${OBJECTDIR}/src/DirectionalShadowMap.o \
	${OBJECTDIR}/src/Light.o \
	${OBJECTDIR}/src/IRenderTarget.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-Wall -Wextra -pedantic -std=c++0x -include src/default.h
CXXFLAGS=-Wall -Wextra -pedantic -std=c++0x -include src/default.h

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/lib/fglrx -L../third-party/sfml2/lib -lGL -lGLU -lGLEW -lsfml-audio -lsfml-graphics -lsfml-system -lsfml-network -lsfml-window

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libHabaneroFork.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libHabaneroFork.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -shared -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libHabaneroFork.${CND_DLIB_EXT} -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/src/Color.o: src/Color.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Color.o src/Color.cpp

${OBJECTDIR}/src/Keyboard.o: src/Keyboard.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Keyboard.o src/Keyboard.cpp

${OBJECTDIR}/src/HeightMapObject.o: src/HeightMapObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/HeightMapObject.o src/HeightMapObject.cpp

${OBJECTDIR}/src/SkeletalAnimatedObject.o: src/SkeletalAnimatedObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkeletalAnimatedObject.o src/SkeletalAnimatedObject.cpp

${OBJECTDIR}/src/MemoryManager.o: src/MemoryManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/MemoryManager.o src/MemoryManager.cpp

${OBJECTDIR}/src/Renderer.o: src/Renderer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Renderer.o src/Renderer.cpp

${OBJECTDIR}/src/ShaderProgram.o: src/ShaderProgram.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ShaderProgram.o src/ShaderProgram.cpp

${OBJECTDIR}/src/Polyhedron.o: src/Polyhedron.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Polyhedron.o src/Polyhedron.cpp

${OBJECTDIR}/src/IResource.o: src/IResource.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/IResource.o src/IResource.cpp

${OBJECTDIR}/src/LUP.o: src/LUP.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/LUP.o src/LUP.cpp

${OBJECTDIR}/src/MFCommon.o: src/MFCommon.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/MFCommon.o src/MFCommon.cpp

${OBJECTDIR}/src/ShaderManager.o: src/ShaderManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ShaderManager.o src/ShaderManager.cpp

${OBJECTDIR}/src/AmbientLight.o: src/AmbientLight.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AmbientLight.o src/AmbientLight.cpp

${OBJECTDIR}/src/SCF.o: src/SCF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SCF.o src/SCF.cpp

${OBJECTDIR}/src/ResourceManager.o: src/ResourceManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ResourceManager.o src/ResourceManager.cpp

${OBJECTDIR}/src/File.o: src/File.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/File.o src/File.cpp

${OBJECTDIR}/src/HMF.o: src/HMF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/HMF.o src/HMF.cpp

${OBJECTDIR}/src/SRT.o: src/SRT.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SRT.o src/SRT.cpp

${OBJECTDIR}/src/PointLight.o: src/PointLight.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/PointLight.o src/PointLight.cpp

${OBJECTDIR}/src/SkeletonJoint.o: src/SkeletonJoint.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkeletonJoint.o src/SkeletonJoint.cpp

${OBJECTDIR}/src/Skeleton.o: src/Skeleton.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Skeleton.o src/Skeleton.cpp

${OBJECTDIR}/src/FrameDescription.o: src/FrameDescription.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/FrameDescription.o src/FrameDescription.cpp

${OBJECTDIR}/src/GenericGeometry.o: src/GenericGeometry.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/GenericGeometry.o src/GenericGeometry.cpp

${OBJECTDIR}/src/SubMesh.o: src/SubMesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SubMesh.o src/SubMesh.cpp

${OBJECTDIR}/src/SkeletalAnimation.o: src/SkeletalAnimation.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkeletalAnimation.o src/SkeletalAnimation.cpp

${OBJECTDIR}/src/StringUtil.o: src/StringUtil.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/StringUtil.o src/StringUtil.cpp

${OBJECTDIR}/src/SkeletalShader.o: src/SkeletalShader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkeletalShader.o src/SkeletalShader.cpp

${OBJECTDIR}/src/Camera.o: src/Camera.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Camera.o src/Camera.cpp

${OBJECTDIR}/src/AABB.o: src/AABB.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AABB.o src/AABB.cpp

${OBJECTDIR}/src/Spatial.o: src/Spatial.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Spatial.o src/Spatial.cpp

${OBJECTDIR}/src/RootObject.o: src/RootObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/RootObject.o src/RootObject.cpp

${OBJECTDIR}/src/StaticObject.o: src/StaticObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/StaticObject.o src/StaticObject.cpp

${OBJECTDIR}/src/LogManager.o: src/LogManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/LogManager.o src/LogManager.cpp

${OBJECTDIR}/src/RT.o: src/RT.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/RT.o src/RT.cpp

${OBJECTDIR}/src/MTF.o: src/MTF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/MTF.o src/MTF.cpp

${OBJECTDIR}/src/SAF.o: src/SAF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SAF.o src/SAF.cpp

${OBJECTDIR}/src/ShaderFlags.o: src/ShaderFlags.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ShaderFlags.o src/ShaderFlags.cpp

${OBJECTDIR}/src/Frustum.o: src/Frustum.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Frustum.o src/Frustum.cpp

${OBJECTDIR}/src/Mouse.o: src/Mouse.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Mouse.o src/Mouse.cpp

${OBJECTDIR}/src/DirectionalLight.o: src/DirectionalLight.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/DirectionalLight.o src/DirectionalLight.cpp

${OBJECTDIR}/src/SkeletonJointKeyframeSequence.o: src/SkeletonJointKeyframeSequence.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkeletonJointKeyframeSequence.o src/SkeletonJointKeyframeSequence.cpp

${OBJECTDIR}/src/LightShader.o: src/LightShader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/LightShader.o src/LightShader.cpp

${OBJECTDIR}/src/IStream.o: src/IStream.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/IStream.o src/IStream.cpp

${OBJECTDIR}/src/StackAllocator.o: src/StackAllocator.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/StackAllocator.o src/StackAllocator.cpp

${OBJECTDIR}/src/line3.o: src/line3.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/line3.o src/line3.cpp

${OBJECTDIR}/src/RendererLightSetup.o: src/RendererLightSetup.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/RendererLightSetup.o src/RendererLightSetup.cpp

${OBJECTDIR}/src/GenericMemoryOrdersGenerator.o: src/GenericMemoryOrdersGenerator.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/GenericMemoryOrdersGenerator.o src/GenericMemoryOrdersGenerator.cpp

${OBJECTDIR}/src/TMF.o: src/TMF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/TMF.o src/TMF.cpp

${OBJECTDIR}/src/StaticMesh.o: src/StaticMesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/StaticMesh.o src/StaticMesh.cpp

${OBJECTDIR}/src/Pick.o: src/Pick.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Pick.o src/Pick.cpp

${OBJECTDIR}/src/ref.o: src/ref.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ref.o src/ref.cpp

${OBJECTDIR}/src/IManagedObject.o: src/IManagedObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/IManagedObject.o src/IManagedObject.cpp

${OBJECTDIR}/src/plane.o: src/plane.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/plane.o src/plane.cpp

${OBJECTDIR}/src/CullingVolumesSet.o: src/CullingVolumesSet.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/CullingVolumesSet.o src/CullingVolumesSet.cpp

${OBJECTDIR}/src/matrix4.o: src/matrix4.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/matrix4.o src/matrix4.cpp

${OBJECTDIR}/src/vector3.o: src/vector3.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/vector3.o src/vector3.cpp

${OBJECTDIR}/src/Window.o: src/Window.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Window.o src/Window.cpp

${OBJECTDIR}/src/CFileStream.o: src/CFileStream.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/CFileStream.o src/CFileStream.cpp

${OBJECTDIR}/src/Exception.o: src/Exception.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Exception.o src/Exception.cpp

${OBJECTDIR}/src/assert.o: src/assert.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/assert.o src/assert.cpp

${OBJECTDIR}/src/Texture.o: src/Texture.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Texture.o src/Texture.cpp

${OBJECTDIR}/src/vector2.o: src/vector2.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/vector2.o src/vector2.cpp

${OBJECTDIR}/src/Node.o: src/Node.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Node.o src/Node.cpp

${OBJECTDIR}/src/SkeletalAnimationState.o: src/SkeletalAnimationState.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkeletalAnimationState.o src/SkeletalAnimationState.cpp

${OBJECTDIR}/src/Logger.o: src/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Logger.o src/Logger.cpp

${OBJECTDIR}/src/Material.o: src/Material.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Material.o src/Material.cpp

${OBJECTDIR}/src/psx_File.o: src/psx_File.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/psx_File.o src/psx_File.cpp

${OBJECTDIR}/src/Event.o: src/Event.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Event.o src/Event.cpp

${OBJECTDIR}/src/HeightMapMesh.o: src/HeightMapMesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/HeightMapMesh.o src/HeightMapMesh.cpp

${OBJECTDIR}/src/SkinnedMesh.o: src/SkinnedMesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkinnedMesh.o src/SkinnedMesh.cpp

${OBJECTDIR}/src/quaternion.o: src/quaternion.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/quaternion.o src/quaternion.cpp

${OBJECTDIR}/src/SkeletalAnimationStateSet.o: src/SkeletalAnimationStateSet.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkeletalAnimationStateSet.o src/SkeletalAnimationStateSet.cpp

${OBJECTDIR}/src/SMF.o: src/SMF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SMF.o src/SMF.cpp

${OBJECTDIR}/src/RendererSkeletonSetup.o: src/RendererSkeletonSetup.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/RendererSkeletonSetup.o src/RendererSkeletonSetup.cpp

${OBJECTDIR}/src/EventConverter.o: src/EventConverter.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/EventConverter.o src/EventConverter.cpp

${OBJECTDIR}/src/Sphere.o: src/Sphere.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Sphere.o src/Sphere.cpp

${OBJECTDIR}/src/ModelFactory.o: src/ModelFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ModelFactory.o src/ModelFactory.cpp

${OBJECTDIR}/src/DirectionalShadowMap.o: src/DirectionalShadowMap.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/DirectionalShadowMap.o src/DirectionalShadowMap.cpp

${OBJECTDIR}/src/Light.o: src/Light.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Light.o src/Light.cpp

${OBJECTDIR}/src/IRenderTarget.o: src/IRenderTarget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../third-party/glew/include -I../third-party/sfml2/include -I/usr/lib -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/IRenderTarget.o src/IRenderTarget.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libHabaneroFork.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
