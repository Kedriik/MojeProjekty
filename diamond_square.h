
#pragma once
#include "glew-1.13.0\include\GL\glew.h"
#include "glfw-3.1\include\GLFW\glfw3.h"
#include <iostream>
#include "glm\glm\glm.hpp"
#include <string>
#include <fstream>
#include "LoadShaders\include\LoadShaders.h"
#include <vector>
#include "vboindexer.hpp"
#include "texture.hpp"
#include <time.h>
#include <conio.h>
#include <vector>
#include <chrono>
#include "windows.h"
using namespace std::chrono;
using namespace glm;
using namespace std;

class DiamondSquare
{

private:
	GLuint RenderProgram, RenderProgramPoints,ComputeProgramIII,ComputeProgramII,ComputeProgramI, PerFrameBuffer, ConstantBuffer, SeedBuffer;
	GLuint Counter, VerticesBuffer, NormalBuffer,HeightMap,heightMapID;
	GLuint DebugBuffer;
	GLuint SizeBuffer;
	mat4 ProjectionMatrix;
	int NUM,*size, sizeLocal;
struct constantData
{
	mat4 ProjectionMatrix;
	vec2 screenSize;
} *constantData;
struct perFrameData
{
	mat4 ViewMatrix;
	vec4 cameraPosition;
	vec4 cameraForward;
} *perFrameData;
struct vector
{
	float x, y,z,w;
};

public:
	DiamondSquare(void);
	~DiamondSquare(void);
	
	void init();
	void compute(int wspLocal=1);
	void draw();
	void setProjectionMatrix(mat4 projectionmatrix)
	{
		ProjectionMatrix=projectionmatrix;
	};
	void updateBuffers(mat4 ViewMatrix,vec3 camPos);
};

