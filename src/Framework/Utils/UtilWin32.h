#ifndef __UTILWIN32_H__
#define __UTILWIN32_H__

#include "Utils/Logger/Logger.h"

void RayFrame::ErrorMsg(STRING msg)
{
  MessageBoxA( NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR );
  LOG_ERROR1( msg.c_str() );
}

void RayFrame::WarningMsg(STRING msg)
{
  MessageBoxA( NULL, msg.c_str(), "Warning", MB_OK | MB_ICONWARNING );
  LOG_WARNING1( msg.c_str() );
}

void RayFrame::InfoMsg(STRING msg)
{
  MessageBoxA( NULL, msg.c_str(), "Information", MB_OK | MB_ICONINFORMATION );
  LOG_CONTENT1( msg.c_str() );
}

#endif
