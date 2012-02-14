#ifndef __UTIL_H__
#define __UTIL_H__

#include <typeinfo>
#include <string>
#include "../Common.h"

#ifndef LINUX
#define snprintf _snprintf
#endif

namespace JustRenderIt
{
  const size_t BUFFER_SIZE = 1024 * 2;

  // DLL_DECLARE void DrawSphere ();
   DLL_DECLARE void DrawGrid(float width = 20, float height = 20,
                             int xSeg = 10, int zSeg = 10);
  // DLL_DECLARE void DrawCube(float width = 1.f, float height = 1.f,
  //   float depth = 1.f);
  // DLL_DECLARE void DrawScreenAlignedBox();

  // DLL_DECLARE void ErrorMsg(STRING msg);
  // DLL_DECLARE void WarningMsg(STRING msg);
  // DLL_DECLARE void InfoMsg(STRING msg);

  DLL_DECLARE void NotImplemented(bool halt = false, STRING msg = EMPTY_STRING);

  /// Check if a file exists
  DLL_DECLARE bool FileExists(STRING filename);

  /// Get the length of a file
  DLL_DECLARE unsigned FileLength(STRING filename);

  /// Trim spaces and newline
  DLL_DECLARE STRING  Trim(const char* s);
  DLL_DECLARE STRING& Trim(STRING &s);

  /// Try searching resources in current folder and specified folder
  DLL_DECLARE bool SearchResource(STRING &filename, STRING folderName);

  DLL_DECLARE float NormalizedRand();

  DLL_DECLARE float Clamp(float x, float min, float max);

  /// type convert
  int ToInt(float f);
  int ToInt(std::string s);
  int ToInt(int i);

  float ToFloat(int i);
  float ToFloat(std::string s);
  float ToFloat(float f);

  std::string ToString(int i);
  std::string ToString(float f);
  std::string ToString(std::string s);

  bool ToBool(int i);
  bool ToBool(float f);
  bool ToBool(std::string s);

  template<typename T1, typename T2>
  bool SameType(T1 v1, T2 v2)
  {
    return typeid(v1) == typeid(v2);
  }

  template<typename T>
  bool AssignValue(int& dest, T src)
  {
    std::type_info infoDest = typeid(dest);
    std::type_info infoSrc = typeid(src);
    if(infoDest == infoSrc)
    {
      dest = src;
      return true;
    }
    dest = ToInt(src);
    return false;
  }

  template<typename T>
  bool AssignValue(float& dest, T src)
  {
    std::type_info infoDest = typeid(dest);
    std::type_info infoSrc = typeid(src);
    if(infoDest == infoSrc)
    {
      dest = src;
      return true;
    }
    dest = ToFloat(src);
    return false;
  }

  template<typename T>
  bool AssignValue(std::string& dest, T src)
  {
    std::type_info infoDest = typeid(dest);
    std::type_info infoSrc = typeid(src);
    if(infoDest == infoSrc)
    {
      dest = src;
      return true;
    }
    dest = ToString(src);
    return false;
  }

  template<typename T>
  bool AssignValue(bool& dest, T src)
  {
    std::type_info infoDest = typeid(dest);
    std::type_info infoSrc = typeid(src);
    if(infoDest == infoSrc)
    {
      dest = src;
      return true;
    }
    dest = ToBool(src);
    return false;
  }
}

#ifndef min
#define min(a, b) ((a)>(b) ? (b) : (a))
#endif
#ifndef max
#define max(a, b) ((a)<(b) ? (b) : (a))
#endif
#endif
