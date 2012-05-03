# JustRenderIt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# setup global path
SET(GLOBAL_ROOT ${CMAKE_CURRENT_LIST_DIR})
SET(GLOBAL_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/bin)
SET(CMAKE_MODULE_PATH "${GLOBAL_ROOT}/CMake" ${CMAKE_MODULE_PATH})

INCLUDE(PrecompiledHeader)

PROJECT(JustRenderIt)

# Setup output path for visual studio
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
  string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
  set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
    ${GLOBAL_OUTPUT_PATH} )
  set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
    ${GLOBAL_OUTPUT_PATH} )
  set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
    ${GLOBAL_OUTPUT_PATH} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
#SET(CMAKE_VERBOSE_MAKEFILE true)

INCLUDE_DIRECTORIES(${GLOBAL_ROOT}/Dependencies/Include)
LINK_DIRECTORIES(${GLOBAL_ROOT}/Dependencies/Lib)
LINK_DIRECTORIES(${GLOBAL_ROOT}/bin)

# OS detection
IF(CMAKE_COMPILER_IS_GNUCXX)
  MESSAGE("Building in Linux like system")
  ADD_DEFINITIONS(-DLINUX)
  ADD_DEFINITIONS('-std=c++0x')

  # Since most windows prerequisites are included in folder Dependencies,
  # we only need to find these package in linux.
  # Find freeimage
  find_package(FreeImage REQUIRED)
  INCLUDE_DIRECTORIES(${LibFreeImage_INCLUDE_DIRS})

  # Find gtest
  find_package(GTest REQUIRED)
  INCLUDE_DIRECTORIES(${LibGTest_INCLUDE_DIRS})

ENDIF()

IF(WIN32)
  MESSAGE("Building in windows")
  ADD_DEFINITIONS(-DWIN32 -D_WINDOWS)
ENDIF()

IF(APPLE)
  ERROR("Apple is not supported right now")
ENDIF()

# Find qt
FIND_PACKAGE(Qt4 REQUIRED COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
SET(QT_USE_OPENGL TRUE)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS} -DUSE_GUI_QT)

# Force to use warning level.3
if(MSVC)
  # disable anoying warning
  ADD_DEFINITIONS(/wd4819 /wd4996)

  # Force to always compile with W3
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W3"
      CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # setup gcc warning settings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long")
endif()

ADD_SUBDIRECTORY(src/Framework)
ADD_SUBDIRECTORY(src/Editor)
ADD_SUBDIRECTORY(src/Demos)
ADD_SUBDIRECTORY(src/Tests)
