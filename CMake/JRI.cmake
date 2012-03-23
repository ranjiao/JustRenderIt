MACRO(ADD_PRECOMPILED_HEADER
    Target PrecompiledHeader PrecompiledSource SourcesVar)
  SET(_sources ${${SourcesVar}})

  IF(MSVC)
    ADD_MSVC_PRECOMPILED_HEADER(${PrecompiledHeader}
      ${PrecompiledSource} _sources)
  ENDIF(MSVC)

  IF(CMAKE_COMPILER_IS_GNUCXX)
    ADD_GCC_PRECOMPILED_HEADER(${Target} ${PrecompiledHeader} _sources)
  ENDIF(CMAKE_COMPILER_IS_GNUCXX)
ENDMACRO(ADD_PRECOMPILED_HEADER)

MACRO(ADD_MSVC_PRECOMPILED_HEADER
    PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary
      "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(
      ${PrecompiledSource}
      PROPERTIES COMPILE_FLAGS
      "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
      OBJECT_OUTPUTS "${PrecompiledBinary}")
    SET_SOURCE_FILES_PROPERTIES(${Sources}
      PROPERTIES COMPILE_FLAGS
      "/Yu\"${PrecompiledBinary}\" /FI\"${PrecompiledBinary}\" /Fp\"${PrecompiledBinary}\""
      OBJECT_DEPENDS "${PrecompiledBinary}")
    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
  ENDIF(MSVC)
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)

IF(CMAKE_COMPILER_IS_GNUCXX)
  EXEC_PROGRAM(
    ${CMAKE_CXX_COMPILER}
    ARGS                    --version
    OUTPUT_VARIABLE _compiler_output)
  STRING(REGEX REPLACE ".* ([0-9]\\.[0-9]\\.[0-9]) .*" "\\1"
    gcc_compiler_version ${_compiler_output})
  #MESSAGE("GCC Version: ${gcc_compiler_version}")
  IF(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
    SET(PCHSupport_FOUND TRUE)
  ELSE(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
    IF(gcc_compiler_version MATCHES "3\\.4\\.[0-9]")
      SET(PCHSupport_FOUND TRUE)
    ENDIF(gcc_compiler_version MATCHES "3\\.4\\.[0-9]")
  ENDIF(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

MACRO(ADD_GCC_PRECOMPILED_HEADER _targetName _input _sources)
  GET_FILENAME_COMPONENT(_name ${_input} NAME)
  GET_FILENAME_COMPONENT(_path ${_input} PATH)
  SET(_outdir "${CMAKE_CURRENT_BINARY_DIR}/${_name}.gch")
  IF(NOT CMAKE_BUILD_TYPE)
    SET(_output "${_outdir}/${CMAKE_BUILD_TYPE}.c++")
  ELSE(NOT CMAKE_BUILD_TYPE)
    SET(_output "${_outdir}/default.c++")
  ENDIF(NOT CMAKE_BUILD_TYPE)

  ADD_CUSTOM_COMMAND(
    OUTPUT ${_outdir}
    COMMAND mkdir ${_outdir} # TODO: {CMAKE_COMMAND} -E ...
    )
  #MAKE_DIRECTORY(${_outdir})

  SET(_compile_FLAGS ${CMAKE_CXX_FLAGS})

  GET_DIRECTORY_PROPERTY(_directory_flags INCLUDE_DIRECTORIES)

  SET(_CMAKE_CURRENT_BINARY_DIR_included_before_path FALSE)
  FOREACH(item ${_directory_flags})
    IF(${item}  STREQUAL ${_path} AND NOT
        _CMAKE_CURRENT_BINARY_DIR_included_before_path )
      MESSAGE(FATAL_ERROR
        "This is the ADD_PRECOMPILED_HEADER macro. "
        "CMAKE_CURREN_BINARY_DIR has to mentioned at INCLUDE_DIRECTORIES's argument list before ${_path}, where ${_name} is located"
        )
    ENDIF(${item}  STREQUAL ${_path} AND NOT
      _CMAKE_CURRENT_BINARY_DIR_included_before_path )

    IF(${item}  STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
      SET(_CMAKE_CURRENT_BINARY_DIR_included_before_path TRUE)
    ENDIF(${item}  STREQUAL ${CMAKE_CURRENT_BINARY_DIR})

    LIST(APPEND _compile_FLAGS "-I${item}")
  ENDFOREACH(item)

  GET_DIRECTORY_PROPERTY(_directory_flags DEFINITIONS)
  LIST(APPEND _compile_FLAGS "-fPIC")
  LIST(APPEND _compile_FLAGS ${_directory_flags})

  SEPARATE_ARGUMENTS(_compile_FLAGS)
  #MESSAGE("_compiler_FLAGS: ${_compile_FLAGS}")

  ADD_CUSTOM_COMMAND(
    OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${_name}
    COMMAND ${CMAKE_COMMAND} -E copy  ${_input}
    ${CMAKE_CURRENT_BINARY_DIR}/${_name} # ensure same directory! Required by gcc
    )
  #MESSAGE("command : ${CMAKE_COMMAND} -E copy  ${_input}
  #  ${CMAKE_CURRENT_BINARY_DIR}/${_name}")

  ADD_CUSTOM_COMMAND(
    OUTPUT ${_output}
    COMMAND ${CMAKE_CXX_COMPILER}
    ${_compile_FLAGS}
    -x c++-header
    -o ${_output}
    ${_input}
    DEPENDS ${_input} ${_outdir} ${CMAKE_CURRENT_BINARY_DIR}/${_name}
    )
  ADD_CUSTOM_TARGET(${_targetName}_gch
    DEPENDS ${_output}
    )
  ADD_DEPENDENCIES(${_targetName} ${_targetName}_gch )

  SET(_sourcesVar ${${_sources}})
  FOREACH(source ${_sourcesVar})
    SET_SOURCE_FILES_PROPERTIES(${source}
      PROPERTIES
      COMPILE_FLAGS "-include ${_name} -Winvalid-pch"
      OBJECT_DEPENDS "${_output}")
  ENDFOREACH(source)
ENDMACRO(ADD_GCC_PRECOMPILED_HEADER)
