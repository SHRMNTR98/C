#ifndef OPENGL_H
#define OPENGL_H

//This creates a variable called glhandle_g (_g for global)
//You need opengl for this
// sudo apt-get install freeglut3-dev
// For windows
// download glut
// On cygwin install the package

/////////OPENGL LIBRARIES/////////
#if (__WIN32__) || (__WIN64__) || (__CYGWIN__)
#include <GL/glut.h>
//#include <GL/glaux.h>
#endif 
#if (__linux__)
#include <GL/gl.h> 
#include <GL/glut.h>
#include <GL/glu.h>
#endif
#if (__macintosh__) || (__Macintosh__) || (__APPLE__) || (__MACH__)
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#endif

#include "MATLAB.h" //Included in BlackBox/C++ directory

#define MAX_TEXTURE 40

void error(char*);

////MATERIAL CLASS

class MTL
{
public:
  char file[256];
  char texname[256];
  char texfile[256];
  bool exist;
  bool texexist;
};

///VISUALIZER OBJECT CLASS - Holds all the state information
class VISOBJECT
{
 public:
  void Initialize(double,int);
  void setTime(double);
  void setState(MATLAB,MATLAB,int);
  void getState(MATLAB,MATLAB);
  double getTime();
 private:
  int numobjects_;
  double simtime_;
  MATLAB cg_,ptp_;
};

///////////STRUCTURES FOR OBJLOADER/////

#pragma pack(1)
struct BITMAPINFOHEADER
{
uint32_t biSize;
uint32_t biWidth;
uint32_t biHeight;
uint16_t biPlanes;
uint16_t biBitCount;
uint32_t biCompression;
uint32_t biSizeImage;
uint32_t biPxmeter;
uint32_t biPymeter;
uint32_t biColors;
uint32_t biImportantColors;
};

struct BITMAPFILEHEADER
{
uint16_t bfType;
uint32_t bfSize;
uint16_t bfReserved1;
uint16_t bfReserved2;
uint32_t bfOffBits;
};
#pragma pack()
struct ObjVertex
{
  GLfloat x, y, z;
};
typedef ObjVertex ObjNormal;
struct ObjTexCoord
{
  GLfloat u, v;
};
struct ObjTriangle
{
  int Vertex[3];
  int Normal[3];
  int TexCoord[3];
};
struct ObjModel
{
  int nVertex, nNormal, nTexCoord, nTriangle;
  ObjVertex* VertexArray;
  ObjNormal* NormalArray;
  ObjTexCoord* TexCoordArray;
  ObjTriangle* TriangleArray;
  MTL material;
};

/////////////ALL OTHER CLASSES/////////////////

class StateHistory
{
public:
  int advance,objects,filesize,rowdx,ok;
  double freq,maxval,**data,T,scale,timestep,**finaldata;
  MATLAB cg,ptp,positionscale;
  void Initialize(int);
};

class CameraControl
{
public:
  int objects,ctype;
  double rearview;
  double cg[3],ptp[3],freepos[3],freetarget[3],targetdistance;
  double pos[3],target[3],up[3],xcamera[3],psi,theta;
  void Initialize(int,MATLAB,MATLAB,int);
  void ComputeRange();
  void ComputeUp();
  void Update(StateHistory);
  void SwitchCamera();
  void ComputeForward();
  void zoom(double);
  void ComputeNewTarget();
  void pitch(double);
  void yaw(double);
};

class MouseControl
{
public:
  int mousex,mousey;
  double prevTime,GetNewMouse;
  void Initialize();
};

struct Image {
  //unsigned long sizeX;
  //unsigned long sizeY;
  uint32_t sizeX;
  uint32_t sizeY;
  char *data;
};

class OBJSETUP
{
public:
  int objects,ok;
  GLuint texture[MAX_TEXTURE];
  GLuint gl_list[MAX_TEXTURE];
  void Initialize(int);
  void Load(char*,int,double,double);
  void MTL(char*);
  size_t ObjLoadFile(char* szFileName, char** memory);
  ObjModel* ObjLoadModel(char* memory, size_t size,int number,char* directory);
  int ImageLoad(char*,Image*);
};

class KeyboardControl
{
public:
  int lightflag;
  void Initialize();
};


class OPENGL {
 public:
  int figure,Width,Height;
  int savepics,counter,itime;
  int ok;
  int Farplane;
  double simtime,initial;
  Image *framebuffer;
  //MainWindow window;
  MouseControl mouse;
  CameraControl camera;
  StateHistory state;
  OBJSETUP objs;
  //GLDraw draw;
  KeyboardControl keyboard;
  VISOBJECT* statetime_objects;

  //OpenGL Methods
  void Initialize(VISOBJECT*,int,char**,int,int,int,int);
  //GLDraw Methods
  //void drawInitialize();
  //Main Window Methods
  void WindowInitialize(bool,int,char**);
  //Statehistory functions
  void GetNewState();

 //Constructor
 OPENGL();
};

//Create OPENGL variable but make it extern so it isn't created more than once
extern OPENGL glhandle_g;
//Create OPENGL static member functions
void DrawGLScene();
void ResizeGLScene(int,int);
void MainLoop();
void MouseMotion(int x,int y);
void SaveMouse(int x,int y);
void Mouse(int button,int state,int x,int y);
void KeyPressed(unsigned char key,int x,int y);

#endif
