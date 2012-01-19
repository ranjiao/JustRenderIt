# rayframe
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# setup global path
SET(GLOBAL_ROOT ${CMAKE_CURRENT_LIST_DIR})
SET(GLOBAL_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/bin)

PROJECT(RayFrame)

#SET(CMAKE_VERBOSE_MAKEFILE true)

INCLUDE_DIRECTORIES(${GLOBAL_ROOT}/Dependencies/Include)
INCLUDE_DIRECTORIES(${GLOBAL_ROOT}/src/Framework)
LINK_DIRECTORIES(${GLOBAL_ROOT}/Dependencies/Lib)

# OS detection
IF(UNIX)
  MESSAGE("Building in Linux like system")
  ADD_DEFINITIONS(-DLINUX)
ENDIF()

IF(WIN32)
  MESSAGE("Building in windows")
  ADD_DEFINITIONS(-DWIN32 -D_WINDOWS)
ENDIF()

IF(APPLE)
  ERROR("Apple is not supported right now")
ENDIF()

#find qt
FIND_PACKAGE(Qt4 REQUIRED COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
SET(QT_USE_OPENGL TRUE)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS} -DUSE_GUI_QT)

# disable some warnings for visual studio
IF(MSVC)
    ADD_DEFINITIONS(/wd4819 /wd4996)
ENDIF()

# Force to use warning level.3
if(MSVC)
  # Force to always compile with W3
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W3"
      CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long")
endif()

ADD_SUBDIRECTORY(src/Framework)
ADD_SUBDIRECTORY(src/Editor)
ADD_SUBDIRECTORY(src/Demos)
ADD_SUBDIRECTORY(src/Tests)