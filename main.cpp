/*
#include <GLFW\glfw3.h>
#include <GL\GLU.h>
#include <GL\wglew.h>
#include <GL\GL.h>
*/
#define GLFW_DLL
#include "glew-1.13.0\include\GL\glew.h"
//#include <glew-1.13.0\include\GL\glxew.h>
//#include <glew-1.13.0\include\GL\wglew.h>
#include <iostream>
#include "glm\glm\glm.hpp"
//#include <gtx\transform.hpp>
#include "glm\glm\gtx\transform.hpp"
//#include <gtc\matrix_transform.hpp>
#include "glm\glm\gtc\matrix_transform.hpp"
//#include <gtc\quaternion.hpp>
//#include <gtx\quaternion.hpp>
#include "glm\glm\gtx\quaternion.hpp"
//#include <gtc\matrix_transform.hpp>
#include "glm\glm\gtc\quaternion.hpp"
#include <string>
#include <fstream>

#include "LoadShaders\include\LoadShaders.h"
//#include <glew-1.13.0\include\GL\wglew.h>
#include <vector>
#include "vboindexer.hpp"
#include "texture.hpp"
#include <time.h>
#include <conio.h>
#include <math.h>
#include "glfw-3.1\include\GLFW\glfw3.h"
#include "Simple OpenGL Image Library\src\SOIL.h"
GLFWwindow *window;
#include "camera.h"

#include "spaceobject.h"


GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT ; 
using namespace glm;
using namespace std;
//////////////////Functions Declarations/////////
void wczytajUstawienia (int&, int&,float&,float&,int&,int&,int&,float&);
static const GLchar* ReadShader( const char*  );
int init(float, float);
float fpsCounter(double, int&, double&);
/////////GLobal Variables///////////////
glm:: mat4 initProjectionMatrix;
///////////////////DEFINE/////////////////////
#include "structs.h"
#include "points_manager.h"
#include "points.h"
#include "terrain.h"
#include "gpu_random.h"
#include "diamond_square.h"
#include "fourier_terrain_manager.h"
//#include "myksz.h"
int main () {
	int q1, q2,  R;
	float w,h;
	int ylayers, xz;
		float dim;
	wczytajUstawienia(q1,q2,w,h, R,ylayers, xz, dim);
	vec3 camPos=vec3(0,1,0); vec3(51.2,250,51.2);
	Camera camera=Camera(h, w,camPos, vec3(0,-1,0.0), vec3(1,0,0), 0.1);
	camera.setSens(0.01f, 3.0f);
	srand(time(NULL));
	//camera.setSens(1,1);
/////////////////////////INITIALIZATION//////////////////////////////////
		init(w,h);
		GLuint vbo;
	
//////////////////////////////////////////////////////////////////////
		
		vec4 seed=vec4(0,0,0,0);//
		
		seed=vec4(500-rand()%1001, 500-rand()%1001,500-rand()%1001,500-rand()%1001);
		DiamondSquare myDiamond=DiamondSquare(vec4(0,0,0,0),seed);
		myDiamond.setProjectionMatrix(initProjectionMatrix);
		myDiamond.init();
		for(int i=0;i<10;i++)
		myDiamond.compute();

		for(int i=0;i<20;i++)
			myDiamond.applyFilter();
		

	
	FourierTerrainMgr fourierManager= FourierTerrainMgr(vec4(0), seed);;
	fourierManager.setProjectionMatrix(initProjectionMatrix);
	fourierManager.init();
	
	
		

	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	//////////////////////////////////DEPTH////////////////////////////////////////////////////////
	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT32, w, h, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	

	/////////color///////////
	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" means "empty" )
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w, h, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Poor filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);	
////////////////////////////////////////////////////////////////////////////

  // Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return 0;
////////quad for testing///////
	
  GLuint quad_VertexArrayID;
  glGenVertexArrays(1, &quad_VertexArrayID);
  glBindVertexArray(quad_VertexArrayID);
  
  static const GLfloat g_quad_vertex_buffer_data[] = {
      -1.0f, -1.0f, 0.0f,
      1.0f, -1.0f, 0.0f,
      -1.0f,  1.0f, 0.0f,
     -1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
 };
 
 GLuint quad_vertexbuffer;
 glGenBuffers(1, &quad_vertexbuffer);
 glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
 glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
 
 // Create and compile our GLSL program from the shaders

 ShaderInfo  shadersExpRender[] = {
	{ GL_VERTEX_SHADER,  "Passthrough.vertexshader" },
	{ GL_FRAGMENT_SHADER,"WobblyTexture.fragmentshader" },
 	{ GL_NONE, NULL },
 	{ GL_NONE, NULL },
	{ GL_NONE, NULL }
    };
 GLuint quad_programID = LoadShaders( shadersExpRender);
 GLuint texIDDepth = glGetUniformLocation(quad_programID, "depthTexture");
 GLuint texIDDiffuse = glGetUniformLocation(quad_programID, "diffuseTexture");
 // Set the list of draw buffers.
	
	
  
  mat4 ViewMatrix;

	double timeFrame=0;
	int frame =0;

	glEnable(GL_DEBUG_OUTPUT);
	
	
		;
glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	float d=0;
	
	ViewMatrix=camera.getViewMatrix();
	
	

	glEnable(GL_POINT_SPRITE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_POINT_SMOOTH);
	 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	bool cam=false;
	
	//////////TEST DEPTH////////////////////
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	//// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	//glDepthFunc(GL_ALWAYS);
	
//	myDiamond.compute(256);
	bool mouseFlag=false;
	double x=0,y=0;
	
	
	double AllTime=0;
	float odl=0;
	bool flag=false;
	float wsp=1;
	float isolevel=5;
	GLuint dTexture;
	float isoMult=1;
	float freqMult=0.1;
	float amplMult=1;
	int klatki=1-1;
	float freq=0.1, ampl=0.1, iter=1;

	fourierManager.compute();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	do
	{
	
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	double deltaTime = double(currentTime - lastTime);

	AllTime+=deltaTime;

	ViewMatrix=camera.cameraPositionAutoAdaptiveCamera(deltaTime);
	
	cout<<"FPS:"<<fpsCounter( deltaTime,  frame,  timeFrame)<<endl;

	odl=sin(AllTime/wsp)+3;
	odl*=600;
	
	
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glViewport(0,0,w,h);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	if (glfwGetKey( window, GLFW_KEY_1 ) == GLFW_PRESS) 
	{
		camera.increaseSensTrans(1);
	}

	if (glfwGetKey( window, GLFW_KEY_2 ) == GLFW_PRESS) 
	{
		camera.decreaseSensTrans(1);
	}
	
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS && mouseFlag==false)
	{
		
		mouseFlag=true;
	}
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)==GLFW_RELEASE )
	{
		mouseFlag=false;
	}
	
	
	

	
	
	if (glfwGetKey( window, GLFW_KEY_V ) == GLFW_PRESS) 
	{
		flag=false;
		
	}

	if (glfwGetKey( window, GLFW_KEY_V ) == GLFW_RELEASE) 
	{
		flag=true;
		
	}
	//////////////////////////////
	if (glfwGetKey( window, GLFW_KEY_4 ) == GLFW_PRESS) 
	{
		isolevel+=deltaTime*isoMult;
	}

	if (glfwGetKey( window, GLFW_KEY_5 ) == GLFW_PRESS) 
	{
		isolevel-=deltaTime*isoMult;
	}

	if (glfwGetKey( window, GLFW_KEY_6 ) == GLFW_PRESS) 
	{
		freq+=deltaTime*freqMult;
	}

	if (glfwGetKey( window, GLFW_KEY_7 ) == GLFW_PRESS) 
	{
		freq-=deltaTime*freqMult;
	}

	if (glfwGetKey( window, GLFW_KEY_8 ) == GLFW_PRESS) 
	{
		ampl+=deltaTime*amplMult;
	}

	if (glfwGetKey( window, GLFW_KEY_9 ) == GLFW_PRESS) 
	{
		ampl-=deltaTime*amplMult;
	}
	///////////////////////
	
	////////////////////
	////TERRAIN TEST////
	////////////////////
	
	////////////////////
	////TERRAIN TEST////
	////////////////////
	//////////
////DiamondTest
	if(flag==0)
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // 5
	else
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	
	myDiamond.updateBuffers(ViewMatrix,camera.getPosition());
	myDiamond.draw();
	//myDiamond.compute();
	/*
	if(klatki % 1000==0)
	{
	//	myDiamond.compute();
		myDiamond.applyFilter();
		klatki=0;
	}
	klatki++;
	myDiamond.draw();
	*/
//	cout<<"CameraX="<<camera.getPosition().x<<endl;
	/*
		if(wsp<=128){
			if(klatki% 3  ==0)
			{
				
		wsp*=2;
		for(int i=0;i<diamonds.size();i++)
		{
			diamonds[i]->updateBuffers(ViewMatrix, vec3(0,1000,1000));
			diamonds[i]->compute();
		}
			}
		}
		klatki++;
	//	myDiamond.compute(wsp);
		for(int i=0;i<diamonds.size();i++)
		{
			diamonds[i]->updateBuffers(ViewMatrix, vec3(0,1000,0));
			diamonds[i]->draw();
			if(wsp<=512){
			if(klatki% 10000  ==0)
			{
			//	diamonds[i]->compute();
			
			}
		}
		}
		if(klatki% 10000==0)
		{
		wsp*=2;
		}
		*/
//	fourierManager.updateBuffers(ViewMatrix, vec3(0,10,10));

	
	//if(klatki>1000)
	//	fourierManager.applyFilter();
	klatki++;
	fourierManager.draw();
		
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

		////////////////////////texture testing/////////
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0,w,h); // Render on the whole framebuffer, complete from the lower left corner to the upper right
	// Clear the screen

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		// Use our shader
		glUseProgram(quad_programID);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		// Set our "renderedTexture" sampler to user Texture Unit 0
		glUniform1i(texIDDepth, 0);
		// Set our "renderedTexture" sampler to user Texture Unit 0
		glUniform1i(texIDDiffuse, 1);

		

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices stwwwwarting at 0 -> 2 triangles

		glDisableVertexAttribArray(0);
		;
		////////
	;
		lastTime = currentTime;
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	//	system("cls");
		}while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
			   glfwWindowShouldClose(window) == 0 );
		
return 0;

}

void wczytajUstawienia (int &q1, int &q2, float &w,float &h, int &R,int &ylayers, int &xz, float &dim) {
    string temp1;
    int  temp2;
    ifstream file;
    file.open ("ustawienia.dat");
    if(file.is_open()){

    }
    file>>temp1;
    file>>temp2;
    q1=temp2;

    file>>temp1;
    file>>temp2;
    q2=temp2;

    file>>temp1;
    file>>temp2;
    w=temp2;

    file>>temp1;
    file>>temp2;
    h=temp2;
	
	file>>temp1;
    file>>temp2;
    R=temp2;

	file>>temp1;
    file>>temp2;
    ylayers=temp2;

	file>>temp1;
    file>>temp2;
    xz=temp2;

	file>>temp1;
    file>>temp2;
    dim=temp2;

    file.close();

}
static const GLchar*ReadShader( const char* filename )
{
#ifdef WIN32
	FILE* infile;
	fopen_s( &infile, filename, "rb" );
#else
    FILE* infile = fopen( filename, "rb" );
#endif // WIN32

    if ( !infile ) {
#ifdef _DEBUG
        std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
        return NULL;
    }

    fseek( infile, 0, SEEK_END );
    int len = ftell( infile );
    fseek( infile, 0, SEEK_SET );

    GLchar* source = new GLchar[len+1];

    fread( source, 1, len, infile );
    fclose( infile );

    source[len] = 0;

    return const_cast<const GLchar*>(source);
}
int init(float w, float h) 
{
	if( !glfwInit() )
		{
			fprintf( stderr, "Failed to initialize GLFW\n" );
			return -1;
		}
	cout<<"GLFW init completed"<<endl;



		glfwWindowHint(GLFW_SAMPLES,4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);



window = glfwCreateWindow( w, h, "Kedrowski Dream", NULL, NULL);
glfwMakeContextCurrent(window);

		// Initialize GLEW
		glewExperimental = true; // Needed for core profile
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			return -1;
		}
	cout<<"GLEW init completed"<<endl;
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
		initProjectionMatrix = glm::perspective(45.0f,w/h, 0.1f, 10000000.0f);

		return 1;
}
float fpsCounter(double deltaTime, int &frame, double &timeFrame)
{
	timeFrame+=deltaTime;
	frame++;
	if(timeFrame>1) 
	{
	return frame/timeFrame;
	frame=0;
	timeFrame=0;
	}
	else
	return NULL;
}