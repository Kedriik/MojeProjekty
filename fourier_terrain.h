#pragma once
#include "fourier_terrain_manager.h"
using namespace glm;
class FourierTerrain
{
friend class FourierTerrainMgr;
private:
FourierTerrainMgr *terrainManager;
GLuint HeightMap;
GLuint  SizeBuffer;
glm::vec4 tilePos;
int *size, sizeLocal, filterPasses, index;
public:
	FourierTerrain(FourierTerrainMgr *manager, GLuint heightMapLocal,int indexLocal, vec4 tilePosition=vec4(0,0,0,0));
	FourierTerrain(FourierTerrainMgr *manager,int indexLocal, vec4 tilePosition=vec4(0,0,0,0));
	void init();
	~FourierTerrain(void);
};

