#ifndef __MYCOMMON_H__
#define __MYCOMMON_H__

// Disable Windows STL check, this will improve performance
// significantlly
#ifndef _DEBUG
//#undef _SECURE_SCL
//#define _SECURE_SCL 0
#endif

// Qt headers must be included first, or it will cause conflicts
#ifdef USE_GUI_QT
#include <QtGui>
#include <QtGui/QMainWindow>
#endif

#include <string>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/swizzle.hpp>

#define PI 3.14159265358979323846f
#define DEGREESTORADIANS(degree)  (degree)*PI / 180.0f

#if _MSC_VER >= 1400
#ifdef LIBFRAMEWORK_EXPORTS
#define DLL_DECLARE_EXPORT  __declspec( dllexport )
#define DLL_DECLARE         __declspec( dllexport )
#else
#define DLL_DECLARE_EXPORT
#define DLL_DECLARE         __declspec( dllimport )
#endif
#else
#define DLL_DECLARE
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) if(p){ delete p; p = NULL;};
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) if(p){ delete [] p; p = NULL;};
#endif

#ifndef V
#define V(x) if(x==NULL) return false;
#endif

typedef std::string STRING;
#define EMPTY_STRING ""

#define BUFFER_LENGTH 1024 * 5
#define BIG_BUFFER_LENGTH 1024 * 1024

#define HashMap std::tr1::unordered_map

namespace JustRenderIt
{
  /// Exception class used by all classes in myFramework
  class JRIException
  {
  protected:
    STRING m_msg;
  public:
    JRIException(STRING msg){ m_msg = msg; };
    virtual ~JRIException(){}
    STRING GetMessage(){ return m_msg; };
  };

  /** Class which has unified create and destroy routines.
  The sub-class should be responsible to take care of the
  status of m_isOK.
  **/
  class DLL_DECLARE IObject
  {
  protected:
    bool m_isOK;
  protected:
    /// actually do the initialization and update m_isOK.
    virtual void internalCreate() = 0;

    /// actually do the destroy and update m_isOK
    virtual void internalDestroy() = 0;
  public:
    /// Return whether this object is properly initialized or not
    virtual bool IsOK(){ return m_isOK; };
  };
};

#endif
