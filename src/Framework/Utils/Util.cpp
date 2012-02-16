#include "Common.h"
#include "GL/glew.h"
#include <string.h>
#include <math.h>
#include "Util.h"
#include "Logger.h"
#include "Graphics/Renderer.h"

using namespace JustRenderIt;

#if defined WIN32
#include <Windows.h>
#endif

GLuint glSphere = 65533;
float  glRadius = 0.0;
unsigned frmCnt = 0;
float fps = 0;

#if defined WIN32
#include "UtilWin32.h"
#elif defined LINUX
#include "UtilLinux.h"
#endif

//void SetSphereRadius ( float r )
//{
//  if ( glRadius == r ) return;
//  glRadius = r;
//
//  // GL sphere
//  if ( glSphere != 65535 ) glDeleteLists ( glSphere, 1 );
//  glSphere = glGenLists ( 1 );
//  float x, y, z, x1, y1, z1;
//  glNewList ( glSphere, GL_COMPILE );
//  glBegin ( GL_TRIANGLE_STRIP );
//  for ( float tilt=-90; tilt <= 90; tilt += 10.0) {
//    for ( float ang=0; ang <= 360; ang += 30.0f) {
//      x = sin ( DEGREESTORADIANS( ang )) * cos ( DEGREESTORADIANS( tilt ) );
//      y = cos ( DEGREESTORADIANS( ang )) * cos ( DEGREESTORADIANS( tilt ) );
//      z = sin ( DEGREESTORADIANS( tilt ) ) ;
//      x1 = sin ( DEGREESTORADIANS( ang )) * cos ( DEGREESTORADIANS( tilt+10.0f ) ) ;
//      y1 = cos ( DEGREESTORADIANS( ang )) * cos ( DEGREESTORADIANS( tilt+10.0f ) ) ;
//      z1 = sin ( DEGREESTORADIANS( tilt+10.0f ) );
//      glNormal3f ( x, y, z );    glVertex3f ( x*r, y*r, z*r );
//      glNormal3f ( x1, y1, z1 );  glVertex3f ( x1*r, y1*r, z1*r );
//    }
//  }
//  glEnd ();
//  glEndList ();
//}

// void JustRenderIt::DrawSphere ()
// {
//   if ( glRadius == 0.0 ) SetSphereRadius ( 1.0 );
//   glCallList ( glSphere );
// }

 void JustRenderIt::DrawGrid(float width, float height, int xSeg, int zSeg)
 {
   glBindTexture(GL_TEXTURE_2D, 0);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_LIGHTING);

   float dx = width / (float)xSeg;
   float dz = height / (float)zSeg;

   float startX = - width / 2.0f;
   float startZ = - height / 2.0f;
   glBegin ( GL_LINES );
   for(int i=0; i<=xSeg; i++)
   {
     glVertex3f(startX + i*dx, 0, startZ);
     glVertex3f(startX + i*dx, 0, -startZ);
   }
   for(int i=0; i<=zSeg; i++)
   {
     glVertex3f(startX, 0, startZ + i*dz);
       glVertex3f(-startX, 0,startZ + i*dz);
   }
   glEnd ();

   glEnable(GL_TEXTURE_2D);
 }

// void JustRenderIt::DrawCube(float width /* = 1.f */, float height /* = 1.f */,
//                         float depth /* = 1.f */)
// {
//   glBegin(GL_QUADS);
//   // Front Face
//   glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height,  depth);  // Bottom Left Of The Texture and Quad
//   glTexCoord2f(1.0f, 0.0f); glVertex3f( width, -height,  depth);  // Bottom Right Of The Texture and Quad
//   glTexCoord2f(1.0f, 1.0f); glVertex3f( width,  height,  depth);  // Top Right Of The Texture and Quad
//   glTexCoord2f(0.0f, 1.0f); glVertex3f(-width,  height,  depth);  // Top Left Of The Texture and Quad
//   // Back Face
//   glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -height, -depth);  // Bottom Right Of The Texture and Quad
//   glTexCoord2f(1.0f, 1.0f); glVertex3f(-width,  height, -depth);  // Top Right Of The Texture and Quad
//   glTexCoord2f(0.0f, 1.0f); glVertex3f( width,  height, -depth);  // Top Left Of The Texture and Quad
//   glTexCoord2f(0.0f, 0.0f); glVertex3f( width, -height, -depth);  // Bottom Left Of The Texture and Quad
//   // Top Face
//   glTexCoord2f(0.0f, 1.0f); glVertex3f(-width,  height, -depth);  // Top Left Of The Texture and Quad
//   glTexCoord2f(0.0f, 0.0f); glVertex3f(-width,  height,  depth);  // Bottom Left Of The Texture and Quad
//   glTexCoord2f(1.0f, 0.0f); glVertex3f( width,  height,  depth);  // Bottom Right Of The Texture and Quad
//   glTexCoord2f(1.0f, 1.0f); glVertex3f( width,  height, -depth);  // Top Right Of The Texture and Quad
//   // Bottom Face
//   glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, -height, -depth);  // Top Right Of The Texture and Quad
//   glTexCoord2f(0.0f, 1.0f); glVertex3f( width, -height, -depth);  // Top Left Of The Texture and Quad
//   glTexCoord2f(0.0f, 0.0f); glVertex3f( width, -height,  depth);  // Bottom Left Of The Texture and Quad
//   glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -height,  depth);  // Bottom Right Of The Texture and Quad
//   // Right face
//   glTexCoord2f(1.0f, 0.0f); glVertex3f( width, -height, -depth);  // Bottom Right Of The Texture and Quad
//   glTexCoord2f(1.0f, 1.0f); glVertex3f( width,  height, -depth);  // Top Right Of The Texture and Quad
//   glTexCoord2f(0.0f, 1.0f); glVertex3f( width,  height,  depth);  // Top Left Of The Texture and Quad
//   glTexCoord2f(0.0f, 0.0f); glVertex3f( width, -height,  depth);  // Bottom Left Of The Texture and Quad
//   // Left Face
//   glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, -depth);  // Bottom Left Of The Texture and Quad
//   glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -height,  depth);  // Bottom Right Of The Texture and Quad
//   glTexCoord2f(1.0f, 1.0f); glVertex3f(-width,  height,  depth);  // Top Right Of The Texture and Quad
//   glTexCoord2f(0.0f, 1.0f); glVertex3f(-width,  height, -depth);  // Top Left Of The Texture and Quad
//   glEnd();
// }

// void JustRenderIt::DrawScreenAlignedBox()
// {
//   glBegin(GL_QUADS);
//   glTexCoord2f(0.f, 0.f); glVertex3f(-1.f, -1.f, 0);
//   glTexCoord2f(1.f, 0.f); glVertex3f( 1.f, -1.f, 0);
//   glTexCoord2f(1.f, 1.f); glVertex3f( 1.f,  1.f, 0);
//   glTexCoord2f(0.f, 1.f); glVertex3f(-1.f,  1.f, 0);
//   glEnd();
// }

void JustRenderIt::NotImplemented(bool halt, STRING msg)
{
  STRING str = msg + " Not implemented!";

  if(!halt)
    LOG_WARNING1(str.c_str());
  else
  {
    LOG_FATAL1(str.c_str());
  }
}

bool JustRenderIt::FileExists(STRING filename)
{
  FILE *file = fopen(filename.c_str(), "rb");
  if(file != NULL)
  {
    fclose(file);
    return true;
  }
  else
  {
    return false;
  }
}

unsigned JustRenderIt::FileLength(STRING filename)
{
  FILE *file = fopen(filename.c_str(), "rb");
  unsigned length;

  if (file != NULL)
  {
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);

    fclose(file);
  }
  else
    throw new Exception("File does not exists while trying to get its length");

  return length;
}

STRING JustRenderIt::Trim(const char* s)
{
  STRING str(s);
  return Trim(str);
}

STRING& JustRenderIt::Trim(STRING &s)
{
  if (s.empty())
    return s;

  unsigned start, end;

  start = end = 0;
  for(; end<s.length()-1;)
  {
    if(s[end] == ' ')
      end++;
    else if(s[end] == '\r' && s[end+1] == '\n')
      end += 2;
    else if(s[end] == '\n')
      end ++;
    else if(s[end] == '\t')
	    end ++;
    else
      break;
  }
  if(start != end)
    s = s.substr(end, s.length() - end);

  start = end = s.length() -1;
  for(; start>0;)
  {
    if(s[start] == ' ')
      start--;
    else if(s[start] == '\t')
      start--;
    else if(s[start] == '\n')
    {
      if(s[start-1] == '\r')
        start -= 2;
      else
        start --;
    }
    else
      break;
  }
  if(start != end)
    s = s.substr(0, start+1);
  return s;
};

DLL_DECLARE float JustRenderIt::NormalizedRand()
{
  return rand() / (float)RAND_MAX;
}

DLL_DECLARE float JustRenderIt::Clamp( float x, float min, float max )
{
  x = (x+max-fabsf(x-max))*0.5f;
  x = (x+min+fabsf(x-min))*0.5f;
  return x;
}

int ToInt(float f)
{
  return int(f);
};

int ToInt(std::string s)
{
  return atoi(s.c_str());
}

int ToInt(int i)
{
  return i;
}

float ToFloat(int i)
{
  return float(i);
};

float ToFloat(std::string s)
{
  return (float)atof(s.c_str());
};

float ToFloat(float f)
{
  return f;
};

std::string ToString(int i)
{
  char buffer[BUFFER_SIZE];
  snprintf(&buffer[0], BUFFER_SIZE, "%d", i);
  return std::string(buffer);
};

std::string ToString(float f)
{
  char buffer[BUFFER_SIZE];
  snprintf(&buffer[0], BUFFER_SIZE, "%f", f);
  return std::string(buffer);
}

std::string ToString(std::string s)
{
  return s;
}

bool ToBool(int i)
{
  return i > 0;
};

bool ToBool(float f)
{
  return f > 0;
};

bool ToBool(std::string s)
{
  std::string temp = s;
  return s == "true";
};
