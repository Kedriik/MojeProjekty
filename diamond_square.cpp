#include "diamond_square.h"



DiamondSquare::DiamondSquare(vec4 tilePosition, vec4 seedLocal)
{
	tilePos=tilePosition;
	seed=seedLocal;
	filterPasses=0;
}
DiamondSquare::~DiamondSquare(void)
{

}
void DiamondSquare::applyFilter()
{
	if(filterPasses<7)
	{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Counter);
	GLuint a=0;
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0 , sizeof(GLuint) , &a);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,0);

	int sizeLocal=1024;
		glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, VerticesBuffer );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, NormalBuffer );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, Counter );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, SeedBuffer);
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, SizeBuffer);
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 6, TilePositionBuffer);
	glBindImageTexture(0, HeightMap, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	glUseProgram(FilterProgram);
	glUniform1i(glGetUniformLocation(FilterProgram, "HeightMap"), 0);
	glDispatchCompute(sizeLocal-2,sizeLocal-2 , 1 );
	glMemoryBarrier( GL_ALL_BARRIER_BITS );
	

	glUseProgram(ComputeProgramIII);
	glUniform1i(glGetUniformLocation(ComputeProgramIII, "HeightMap"), 0);
	glBindImageTexture(0, HeightMap, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	//wsp-=1;
	glBindBuffer( GL_SHADER_STORAGE_BUFFER, SizeBuffer );
	glBufferData( GL_SHADER_STORAGE_BUFFER,  sizeof(int), NULL, GL_DYNAMIC_COPY);
	size = (int *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER,0,  sizeof(int), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT  );
		*size=1;
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, SizeBuffer);
	glUseProgram(ComputeProgramIII);
	int wsp=512;
	glDispatchCompute(wsp, wsp, 1 );
	glMemoryBarrier( GL_ALL_BARRIER_BITS );

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Counter);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,sizeof(GLuint) , &NUM);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,0);
	
	filterPasses++;
	}
}
void DiamondSquare::computeNormalToSmooth()
{
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, VerticesBuffer );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, NormalBuffer );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, PerFrameBuffer );
	glUseProgram(ComputeSmooth);
	NUM;

	glDispatchCompute(1, NUM/1024, 1 );
	glMemoryBarrier( GL_ALL_BARRIER_BITS );
}
void DiamondSquare::init()
{
	vec4 seedLocal=seed;//vec4(rand()%1000,rand()%1000,rand()%1000,rand()%1000);
	
	int size=7000000;
	int indexSize=300000;
	sizeLocal=size-1;
	ShaderInfo  computeShaderII[] = {
	{ GL_COMPUTE_SHADER, "shaders\\terrainComputeDiamondSquareII.txt"},
	{ GL_NONE, NULL  },
 	{ GL_NONE, NULL  },
 	{ GL_NONE, NULL  },
	{ GL_NONE, NULL  }
    };
	ComputeProgramII=LoadShaders(computeShaderII);

	ShaderInfo  computeShaderIII[] = {
	{ GL_COMPUTE_SHADER, "shaders\\terrainComputeDiamondSquareIII.txt"},
	{ GL_NONE, NULL  },
 	{ GL_NONE, NULL  },
 	{ GL_NONE, NULL  },
	{ GL_NONE, NULL  }
    };
	ComputeProgramIII=LoadShaders(computeShaderIII);

	ShaderInfo  computeShaderI[] = {
	{ GL_COMPUTE_SHADER, "shaders\\terrainComputeDiamondSquareI.txt"},
	{ GL_NONE, NULL  },
 	{ GL_NONE, NULL  },
 	{ GL_NONE, NULL  },
	{ GL_NONE, NULL  }
    };
	ComputeProgramI=LoadShaders(computeShaderI);
	
	
	ShaderInfo  computeShaderSmooth[] = {
	{ GL_COMPUTE_SHADER, "shaders\\computeSmooth.txt"},
	{ GL_NONE, NULL  },
 	{ GL_NONE, NULL  },
 	{ GL_NONE, NULL  },
	{ GL_NONE, NULL  }
    };
	ComputeSmooth=LoadShaders(computeShaderSmooth);

	ShaderInfo  filterProgram[] = {
	{ GL_COMPUTE_SHADER, "shaders\\terrainFilter.txt"},
	{ GL_NONE, NULL  },
 	{ GL_NONE, NULL  },
 	{ GL_NONE, NULL  },
	{ GL_NONE, NULL  }
    };
	FilterProgram=LoadShaders(filterProgram);
	
	/////////Create Render program
	
	ShaderInfo  renderShader[] = {
	{ GL_VERTEX_SHADER,  "shaders\\diamondVert.txt" },
	{ GL_FRAGMENT_SHADER,"shaders\\diamondFrag.txt" },
 	{ GL_NONE, NULL },
 	{ GL_NONE, NULL },
	{ GL_NONE, NULL }
    };
	RenderProgram=LoadShaders(renderShader);
	
	
	ShaderInfo  renderShaderPoints[] = {
	{ GL_VERTEX_SHADER,  "shaders\\fireworksVert.txt" },
	{ GL_FRAGMENT_SHADER,"shaders\\fireworksFrag.txt" },
 	{ GL_NONE, NULL },
 	{ GL_NONE, NULL },
	{ GL_NONE, NULL }
    };
	RenderProgramPoints=LoadShaders(renderShader);
	///////////////////////////////////////////
	glGenBuffers(1, &ElementBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ElementBuffer);
	glBufferData( GL_SHADER_STORAGE_BUFFER, sizeof(struct index), NULL, GL_DYNAMIC_COPY);
	struct index *indexLocal = (struct index*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 0,indexSize* sizeof(struct index), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT  );
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

	glGenBuffers( 1, &PerFrameBuffer);
	glBindBuffer( GL_SHADER_STORAGE_BUFFER, PerFrameBuffer);
	glBufferData( GL_SHADER_STORAGE_BUFFER, sizeof(struct perFrameData), NULL, GL_DYNAMIC_COPY);
	perFrameData = (struct perFrameData *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER,0, sizeof(struct perFrameData), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT  );
	perFrameData->Properties.y=0;
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

	glGenBuffers( 1, &ConstantBuffer);
	glBindBuffer( GL_SHADER_STORAGE_BUFFER, ConstantBuffer);
	glBufferData( GL_SHADER_STORAGE_BUFFER, sizeof(struct constantData), NULL, GL_DYNAMIC_COPY);
	constantData = (struct constantData *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER,0, sizeof(struct constantData), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT  );
	constantData->ProjectionMatrix=ProjectionMatrix;
	constantData->screenSize=vec2(1920, 1080);
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER);

	glGenBuffers( 1, &VerticesBuffer);
	glBindBuffer( GL_SHADER_STORAGE_BUFFER, VerticesBuffer );
	glBufferData( GL_SHADER_STORAGE_BUFFER,size* sizeof(struct vector), NULL, GL_DYNAMIC_COPY);
	
	//cout<<"ERROR:"<<glewGetErrorString(glGetError())<<endl;
	struct vector *VerticesLocal = (struct vector*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 0,size* sizeof(struct vector), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT  );
	/*for( int i = 0; i <size; i++ )
	{
		for(int j=0;j<size;j++)
		{
			VerticesLocal[i*size+j].x=float(i)/50;
			VerticesLocal[i*size+j].z=float(j)/50;
			VerticesLocal[i*size+j].y=0;
			VerticesLocal[i*size+j].w=0;
		}
	}*/
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER);
	///////////////////////////////////////////////////
	glGenBuffers( 1, &NormalBuffer);
	glBindBuffer( GL_SHADER_STORAGE_BUFFER, NormalBuffer );
	glBufferData( GL_SHADER_STORAGE_BUFFER, size*sizeof(struct vector), NULL, GL_DYNAMIC_COPY);
	struct vector *normal = (struct vector *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 0,size*sizeof(struct vector), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT  );
	/*for(int i=0;i<size*size;i++)
	{
	normal->x=0;
	normal->y=0;
	normal->z=0;
	normal->w=0;
	}*/
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER);
	/////////////////////////////////////
	glGenBuffers( 1, &SeedBuffer);
	glBindBuffer( GL_SHADER_STORAGE_BUFFER, SeedBuffer );
	glBufferData( GL_SHADER_STORAGE_BUFFER, sizeof(struct vector), NULL, GL_DYNAMIC_COPY);
	struct vector *seed = (struct vector *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 0,sizeof(struct vector), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT  );
	
	seed->x=seedLocal.x;
	seed->y=seedLocal.y;
	seed->z=seedLocal.z;
	seed->w=seedLocal.w;
	
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER);
	/////////////////////////////////////
	/////////////////////////////////////
	glGenBuffers( 1, &TilePositionBuffer);
	glBindBuffer( GL_SHADER_STORAGE_BUFFER, TilePositionBuffer );
	glBufferData( GL_SHADER_STORAGE_BUFFER, sizeof(struct vector), NULL, GL_DYNAMIC_COPY);
	struct vector *pos = (struct vector *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 0,sizeof(struct vector), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT  );
	
	pos->x=tilePos.x;
	pos->y=tilePos.y;
	pos->z=tilePos.z;
	pos->w=tilePos.w;
	
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER);
	/////////////////////////////////////
	/////////////////////////////////////
	glGenBuffers( 1, &Counter);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Counter );
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint) , NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	/////////////////////////////////////
	/////////////////////////////////////
	glGenBuffers( 1, &SizeBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SizeBuffer );
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) , NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	/////////////////////////////////////
	int sizeText=1025;
	glGenTextures(1, &HeightMap);
	glBindTexture(GL_TEXTURE_2D, HeightMap);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA32F, sizeText, sizeText, 0,GL_RGBA, GL_FLOAT, 0);
	
	sizeLocal=1;

	
}
void DiamondSquare::compute(int wspLocal, int flag)
{
//	int flag=0;
	int maxInteration=512;
	int wsp=sizeLocal;
	if(sizeLocal>maxInteration )
	{
		return;
	}
	
	//////////reset counter////////
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Counter);
	GLuint a=0;
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0 , sizeof(GLuint) , &a);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,0);

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, VerticesBuffer );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, NormalBuffer );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, Counter );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, SeedBuffer);
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 6, TilePositionBuffer);
//	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 8, PerFrameBuffer);
	
		glUseProgram(ComputeProgramI);
		glUniform1i(glGetUniformLocation(ComputeProgramI, "HeightMap"), 0);

		glBindImageTexture(0, HeightMap, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		int j=1;
	
		glBindBuffer( GL_SHADER_STORAGE_BUFFER, SizeBuffer );
		glBufferData( GL_SHADER_STORAGE_BUFFER,  sizeof(int), NULL, GL_DYNAMIC_COPY);
		size = (int *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER,0,  sizeof(int), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT  );
		*size=maxInteration/wsp;
		glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
		glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, SizeBuffer);
	//	wsp+=1;
	
		glUseProgram(ComputeProgramI);
		glDispatchCompute(wsp, wsp, 1 );
		glMemoryBarrier( GL_ALL_BARRIER_BITS );
	//	wsp-=1;
		glUseProgram(ComputeProgramII);
		glUniform1i(glGetUniformLocation(ComputeProgramIII, "HeightMap"), 0);
		glDispatchCompute(wsp, wsp, 1 );
		glMemoryBarrier( GL_ALL_BARRIER_BITS );
	
	//cout<<wsp<<endl;
	

	
	
	glUseProgram(ComputeProgramIII);
	glUniform1i(glGetUniformLocation(ComputeProgramIII, "HeightMap"), 0);
	glBindImageTexture(0, HeightMap, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	//wsp-=1;
	
	//glUseProgram(ComputeProgramIII);
	glDispatchCompute(wsp, wsp, 1 );
	glMemoryBarrier( GL_ALL_BARRIER_BITS );
	
	//cout<<wsp<<endl;
	
	
	

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Counter);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,sizeof(GLuint) , &NUM);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,0);
	
//	cout<<"Iteracje"<<sizeLocal<<"NUM:"<<NUM<<endl;
	sizeLocal*=2;

	glBindBuffer( GL_SHADER_STORAGE_BUFFER, PerFrameBuffer );
	glBufferData( GL_SHADER_STORAGE_BUFFER,  sizeof(struct perFrameData), NULL, GL_DYNAMIC_COPY);
	perFrameData = (struct perFrameData *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER,0,  sizeof(struct perFrameData), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT  );
	perFrameData->Properties.x=NUM;
	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
	//computeNormalToSmooth();
}
void DiamondSquare::draw()
{
//	glUseProgram(RenderProgramPoints);
	glUseProgram(RenderProgram);

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, PerFrameBuffer );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, ConstantBuffer );

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
	glVertexAttribPointer(
		2,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
						);
	//Draw array
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, VerticesBuffer);
	glVertexAttribPointer(
		3,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
						);
		
	glDrawArrays(GL_TRIANGLES, 0, NUM);
}
void DiamondSquare::updateBuffers(mat4 ViewMatrix, vec3 camPos)
{
			glBindBuffer( GL_SHADER_STORAGE_BUFFER, PerFrameBuffer );
			glBufferData( GL_SHADER_STORAGE_BUFFER,  sizeof(struct perFrameData), NULL, GL_DYNAMIC_COPY);
			perFrameData = (struct perFrameData *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER,0,  sizeof(struct perFrameData), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT  );
			perFrameData->ViewMatrix=ViewMatrix;
			perFrameData->cameraForward=vec4(0,0,0,0);
	//	perFrameData->cameraPosition=vec4(10,110,50,0);//vec4(camPos.x,camPos.y, camPos.z,0);//vec4(0,10,0,0);
			perFrameData->cameraPosition=vec4(camPos.x,camPos.y, camPos.z,0);//vec4(0,10,0,0);
			perFrameData->Properties.x=10;
			glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

			glBindBuffer( GL_SHADER_STORAGE_BUFFER, SizeBuffer );
			glBufferData( GL_SHADER_STORAGE_BUFFER,  sizeof(int), NULL, GL_DYNAMIC_COPY);
			size = (int *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER,0,  sizeof(int), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT  );
			*size=512;
			glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

}

