
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
	GLuint RenderProgram, RenderProgramPoints,ComputeProgramIII, ComputeSmooth, FilterProgram,ComputeProgramII,ComputeProgramI, PerFrameBuffer, ConstantBuffer, SeedBuffer, TilePositionBuffer;
	GLuint Counter, VerticesBuffer, NormalBuffer,HeightMap,heightMapID;
	GLuint DebugBuffer;
	GLuint SizeBuffer;
	GLuint ElementBuffer;
	
	mat4 ProjectionMatrix;
	int NUM,*size, sizeLocal, filterPasses;
	vec4 tilePos;
	vec4 seed;
struct HeightMaps
{
	GLuint HeightMap;
	int size;
	int index;
};

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
	vec4 Properties; //x for NUM of vertices
} *perFrameData;
struct vector
{
	float x, y,z,w;
};
struct index
{
	unsigned i;
};
public:
	DiamondSquare(vec4 tilePosition=vec4(0,0,0,0), vec4 seedLocal=vec4(0,0,0,0));
	~DiamondSquare(void);
	
	void init();
	void compute(int wspLocal=1, int flag=0);
	void computeNormalToSmooth();
	void applyFilter();
	void draw();
	void setProjectionMatrix(mat4 projectionmatrix)
	{
		ProjectionMatrix=projectionmatrix;
	};
	void updateBuffers(mat4 ViewMatrix,vec3 lightPos);
};

