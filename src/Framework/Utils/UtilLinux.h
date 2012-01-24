#ifndef __UTILLINUX_H__
#define __UTILLINUX_H__
#ifdef LINUX

#include "Common.h"

namespace RayFrame
{

  void ErrorMsg(STRING string)
  {
    printf("[Error]:\t%s\n", string.c_str());
  }

  void WarningMsg(STRING string)
  {
    printf("[Warning]:\t%s\n", string.c_str());
  }

  void InfoMsg(STRING string)
  {
    printf("[Info]:\t%s\n", string.c_str());
  };

};
#endif

#endif
