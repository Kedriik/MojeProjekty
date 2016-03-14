#include "fourier_terrain.h"


FourierTerrain::FourierTerrain(FourierTerrainMgr *manager, GLuint heightMapLocal,int indexLocal,vec4 tilePosition)
{
	terrainManager=manager;
	HeightMap=heightMapLocal;
	index=indexLocal;
	sizeLocal=1;
}
FourierTerrain::FourierTerrain(FourierTerrainMgr *manager,int indexLocal,vec4 tilePosition)
{
	terrainManager=manager;
	index=indexLocal;
	sizeLocal=1;
}



FourierTerrain::~FourierTerrain(void)
{
}
void FourierTerrain::init()
{
	/////////////////////////////////////
	int sizeText=1025;
	glGenTextures(1, &HeightMap);
	glBindTexture(GL_TEXTURE_2D, HeightMap);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA32F, sizeText, sizeText, 0,GL_RGBA, GL_FLOAT, 0);
}
