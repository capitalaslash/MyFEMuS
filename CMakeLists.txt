CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(femus)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")

If(APPLE)
  set(CMAKE_MACOSX_RPATH 1)
ENDIF(APPLE)

#############################################################################################
### Femus library version
#############################################################################################

SET(FEMUS_VERSION_MAJOR 1)
SET(FEMUS_VERSION_MINOR 0)
SET(FEMUS_BUILD_VERSION 0)
SET(FEMUS_VERSION "${FEMTTU_MAJOR_VERSION}.${FEMTTU_MINOR_VERSION}.${FEMTTU_BUILD_VERSION}")



#############################################################################################
### Compilers
#############################################################################################

# If the user specifies -DCMAKE_BUILD_TYPE on the command line, take their definition
# and dump it in the cache along with proper documentation, otherwise set CMAKE_BUILD_TYPE
# to Debug prior to calling PROJECT()
#SET(CMAKE_BUILD_TYPE "release")

IF(CMAKE_BUILD_TYPE MATCHES "debug")
  SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE})
ELSE()
  SET(CMAKE_BUILD_TYPE "release")
ENDIF()

#message("CMAKE_BUILD_TYPE = " ${CMAKE_BUILD_TYPE})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-parameter")

#############################################################################################
### Output libraries folder
#############################################################################################

# where to place output binaries
SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib64/ CACHE INTERNAL
     "Single output directory for building all libraries.")
# SET (EXECUTABLE_OUTPUT_PATH ${MAF_BINARY_DIR}/bin/ CACHE INTERNAL
#      "Single output directory for building all executables.")

MARK_AS_ADVANCED (
  LIBRARY_OUTPUT_PATH
  #EXECUTABLE_OUTPUT_PATH
)



#############################################################################################
### Find packages
#############################################################################################

# Find Petsc Library
FIND_PACKAGE(PETSc REQUIRED)
MESSAGE(STATUS "PETSC_FOUND = ${PETSC_FOUND}")

SET(HAVE_PETSC 0)
IF(PETSC_FOUND)
  SET(HAVE_PETSC 1)
ENDIF(PETSC_FOUND)

# Find MPI (optional)
IF(APPLE)
  SET(MPI_FOUND 1)
ELSE()
  FIND_PACKAGE(MPI)
ENDIF(APPLE)
MESSAGE(STATUS "MPI_FOUND = ${MPI_FOUND}")
SET(HAVE_MPI 0)
IF(MPI_FOUND)
  SET(HAVE_MPI 1)
ENDIF(MPI_FOUND)


# Find MPI (optional)
if(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
     set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
endif()


# Find FParser
FIND_PACKAGE(FParser)
MESSAGE(STATUS "FPARSER_FOUND = ${FPARSER_FOUND}")
SET (HAVE_FPARSER 0)
IF(FPARSER_FOUND)
  SET(HAVE_FPARSER 1)
ENDIF(FPARSER_FOUND)

# Find HDF5 (optional)
FIND_PACKAGE(HDF5)
MESSAGE(STATUS "HDF5_FOUND = ${HDF5_FOUND}")
SET(HAVE_HDF5 0)
IF(HDF5_FOUND)
  SET(HAVE_HDF5 1)
ENDIF(HDF5_FOUND)

# Find Metis (optional)
IF(APPLE)
  FIND_PACKAGE(METISApple)
ELSE()
  FIND_PACKAGE(METIS)
ENDIF(APPLE)
MESSAGE(STATUS "METIS_FOUND = ${METIS_FOUND}")
SET (HAVE_METIS 0)
IF(METIS_FOUND)
  SET(HAVE_METIS 1)
ENDIF(METIS_FOUND)

# Find Slepc Library (optional)
IF(APPLE)
  FIND_PACKAGE(SLEPcApple)
ELSE()
  FIND_PACKAGE(SLEPc)
ENDIF(APPLE)
MESSAGE(STATUS "SLEPC_FOUND = ${SLEPC_FOUND}")
SET(HAVE_SLEPC 0)
IF(SLEPC_FOUND)
  SET(HAVE_SLEPC 1)
ENDIF(SLEPC_FOUND)

# Find Libmesh (optional)
# FIND_PACKAGE(LIBMESH)
# MESSAGE(STATUS "LIBMESH_FOUND = ${LIBMESH_FOUND}")
# SET (HAVE_LIBMESH 0)
# IF(LIBMESH_FOUND)
#   SET(HAVE_LIBMESH 1)
# ENDIF(LIBMESH_FOUND)

find_package(Eigen3 REQUIRED NO_MODULE)

SET(EIGEN_SAFE_TO_USE_STANDARD_ASSERT_MACRO FALSE)

find_package(Ceres NO_MODULE REQUIRED)

find_package(glog NO_MODULE REQUIRED)

set(CMAKE_CXX_STANDARD 14)

#############################################################################################
### Build options
#############################################################################################

#-- Add an Option to toggle the generation of the API documentation
OPTION(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)

#-- build Adept library
OPTION(BUILD_B64 "Build Adept library for automatic differentiation" ON)

#-- build Adept library
OPTION(BUILD_JSONCPP "Build JsonCPP library for json input parsing" ON)

#-- build b64 library
OPTION(BUILD_ADEPT "Build b64 library for 64-encoding" ON)

# Ask the user which executable have to be built
OPTION(BUILD_NAVIERSTOKESAPPS "Build the NavierStokes-line applications" OFF)

# Ask the user which executable have to be built
OPTION(BUILD_FSIAPPS "Build the FSI applications" OFF)

# OPTION(BUILD_OPTCONTROLAPPS "Build the optimal control applications" ON)

#--
OPTION(BUILD_POISSON "Build the Poisson problem" OFF)

# --
OPTION(BUILD_POISSON_AMR "Build the Poisson_AMR problem" OFF)

# --
OPTION(BUILD_WILLMORE "Build the Willmore problem" OFF)

# --
OPTION(BUILD_TUTORIAL "Build the tutorial problems" OFF)

# --
OPTION(BUILD_MGAMR "Build the MGAMR" OFF)

OPTION(BUILD_CUTFEM "Build the cut FEM" OFF)

# --
OPTION(BUILD_PCFIELDSPLIT "Build the PCFIELDSPLIT apps" OFF)

OPTION(BUILD_AMR "Build the AMR apps" OFF)

OPTION(BUILD_ISM "Build the Intersection Marker apps" OFF)

OPTION(BUILD_MPM_FEM "Build the MPM FEM apps" OFF)

OPTION(BUILD_MPM_FSI "Build the MPM FSI apps" OFF)

OPTION(BUILD_REFINEMENT "Build the Refiniment apps" OFF)

OPTION(BUILD_PETSC_DEBUG "Build the PETSC debug apps" OFF)

OPTION(BUILD_UQ "Build the UQ apps" OFF)

OPTION(BUILD_NONLOCAL "Build the NONLOCAL apps" ON)

OPTION(BUILD_SW "Build the Shallow Water apps" OFF)

OPTION(BUILD_TUMOR "Build the Tumor apps" OFF)

OPTION(BUILD_RK "Build the Runge-Kutta apps" OFF)

OPTION(BUILD_CONFORMAL "Build the Conformal mesh apps" OFF)

OPTION(BUILD_QUASICONFORMAL "Build the QuasiConformal mesh apps" OFF)

OPTION(BUILD_NITSCHE "Build the Nitsche apps" OFF)

OPTION(BUILD_MULTI_PHASE "Build the Multi Phase apps" ON)

OPTION(BUILD_TIME_OPTIMAL_CONTROL "Build Time Dependent Optimal Control apps" ON)

#############################################################################################
### Build documentation (Doxygen)
#############################################################################################

IF(BUILD_DOCUMENTATION)
  FIND_PACKAGE(Doxygen)
  IF(NOT DOXYGEN_FOUND)
    MESSAGE(FATAL_ERROR
      "Doxygen is needed to build the documentation. Please install it correctly")
  ENDIF(NOT DOXYGEN_FOUND)
  #-- Configure the Template Doxyfile for our specific project
  CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/doc/doxygen/DoxyfileFemuslib.in"
    "${PROJECT_BINARY_DIR}/doc/doxygen/DoxyfileFemuslib" @ONLY IMMEDIATE
  )

  #-- Add a custom target to run Doxygen when ever the project is built
  ADD_CUSTOM_TARGET (Docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/doc/doxygen/DoxyfileFemuslib
    SOURCES ${PROJECT_BINARY_DIR}/doc/doxygen/DoxyfileFemuslib)
  # IF you do NOT want the documentation to be generated EVERY time you build the project
  # then leave out the 'ALL' keyword from the above command.
ENDIF(BUILD_DOCUMENTATION)



#############################################################################################
### Add external libraries
#############################################################################################

# Build the b64 library
SET(HAVE_B64 0)
IF(BUILD_B64)
  SET(HAVE_B64 1)
  INCLUDE(external/External_b64.cmake)
ENDIF(BUILD_B64)

# Build the jsoncpp library
SET(HAVE_JSONCPP 0)
IF(BUILD_JSONCPP)
  SET(HAVE_JSONCPP 1)
  INCLUDE(external/External_jsoncpp.cmake)
ENDIF(BUILD_JSONCPP)

# Build the Adept external library
SET(HAVE_ADEPT 0)
IF(BUILD_ADEPT)
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DADEPT_RECORDING_PAUSABLE")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DADEPT_RECORDING_PAUSABLE")
  SET(HAVE_ADEPT 1)
  INCLUDE(external/External_adept.cmake)
ENDIF(BUILD_ADEPT)

#message(${CMAKE_CXX_FLAGS_RELEASE})
#message(${CMAKE_CXX_FLAGS_DEBUG})

#############################################################################################
### Configuration project file
#############################################################################################

# configure a header file to pass some of the CMake settings to the source code
CONFIGURE_FILE(
  "${PROJECT_SOURCE_DIR}/src/utils/FemusConfig.hpp.in"
  "${PROJECT_BINARY_DIR}/include/FemusConfig.hpp" @ONLY IMMEDIATE
  )

set(USEFEMUS_INCLUDES )
list(APPEND USEFEMUS_INCLUDES
  ${CMAKE_BINARY_DIR}/include
  ${CMAKE_SOURCE_DIR}/src/algebra
  ${CMAKE_SOURCE_DIR}/src/enums
  ${CMAKE_SOURCE_DIR}/src/equations
  ${CMAKE_SOURCE_DIR}/src/fe
  ${CMAKE_SOURCE_DIR}/src/ism
  ${CMAKE_SOURCE_DIR}/src/mesh
  ${CMAKE_SOURCE_DIR}/src/meshGencase
  ${CMAKE_SOURCE_DIR}/src/parallel
  ${CMAKE_SOURCE_DIR}/src/physics
  ${CMAKE_SOURCE_DIR}/src/quadrature
  ${CMAKE_SOURCE_DIR}/src/quadrature/cutfem
  ${CMAKE_SOURCE_DIR}/src/solution
  ${CMAKE_SOURCE_DIR}/src/solution/output
  ${CMAKE_SOURCE_DIR}/src/uq
  ${CMAKE_SOURCE_DIR}/src/utils
  ${CMAKE_SOURCE_DIR}/src/utils/eigen
  ${CMAKE_SOURCE_DIR}/src/utils/input_parser
  ${ADEPT_INCLUDE_DIRS}
  ${B64_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIRS}
  ${PETSC_INCLUDES}
  ${SLEPC_INCLUDE_DIRS}
)

set(USEFEMUS_LIBRARIES ${LIBRARY_OUTPUT_PATH}/libfemus.so)
list(APPEND USEFEMUS_LIBRARIES
  ${ADEPT_LIBRARIES}
  ${B64_LIBRARIES}
  ${HDF5_LIBRARIES}
  ${JSONCPP_LIBRARIES}
  ${PETSC_LIBRARIES}
  # this does not work properly with the current FindSLEPc
  # ${SLEPC_LIBRARIES}
  ${LIB_slepc}
)

if (HAVE_FPARSER)
  list(APPEND USEFEMUS_INCLUDES ${FPARSER_INCLUDE_DIR})
  list(APPEND USEFEMUS_LIBRARIES ${FPARSER_LIBRARY})
endif()

message(STATUS "MPI_EXTRA_LIBRARY = ${MPI_EXTRA_LIBRARY}")
if (${MPI_EXTRA_LIBRARY})
  list(APPEND USEFEMUS_LIBRARIES ${MPI_EXTRA_LIBRARY})
endif()

CONFIGURE_FILE(
  "${PROJECT_SOURCE_DIR}/UseFemus.CMake.in"
  "${PROJECT_BINARY_DIR}/UseFemus.CMake" @ONLY IMMEDIATE
  )

#############################################################################################
### Include files
#############################################################################################

# Include petsc files
INCLUDE_DIRECTORIES(${PETSC_INCLUDES})
ADD_DEFINITIONS(${PETSC_DEFINITIONS})

IF(SLEPC_FOUND)
 INCLUDE_DIRECTORIES(${SLEPC_INCLUDE_DIRS})
ENDIF(SLEPC_FOUND)

#include femus include files
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/algebra)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/mesh)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/mesh/geom_elements/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/mesh/input/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/mesh/partitioning/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/mesh/refinement/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/mesh/gencase/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/utils)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/utils/input_parser/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/utils/eigen)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/quadrature)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/quadrature/cutfem)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/quadrature/cutfem/old)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/parallel)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/equations)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/equations/assemble/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/solution)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/solution/output/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/enums)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/fe)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/ism)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/physics)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/uq)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include)

# HPCC TTU
INCLUDE_DIRECTORIES(/opt/ohpc/pub/mpi/openmpi-gnu/1.10.6/include/)
INCLUDE_DIRECTORIES(/opt/ohpc/pub/libs/gnu/openmpi/boost/1.63.0/include/)

# Include Fparser files
IF(FPARSER_FOUND)
  INCLUDE_DIRECTORIES(${FPARSER_INCLUDE_DIR})
ENDIF(FPARSER_FOUND)

# add femus macro
INCLUDE(${CMAKE_SOURCE_DIR}/cmake-modules/femusMacroBuildApplication.cmake)


# Libmesh includes and flags
# IF(LIBMESH_FOUND)
# execute_process(COMMAND $ENV{FM_BASEPATH_TO_LM}/$ENV{FM_LM_FOLDER}/bin/libmesh-config --cppflags
#                 COMMAND tr "\n" " " OUTPUT_VARIABLE LIBMESH_CPP)
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LIBMESH_CPP}")
# execute_process(COMMAND $ENV{FM_BASEPATH_TO_LM}/$ENV{FM_LM_FOLDER}/bin/libmesh-config --include
#                 COMMAND tr "\n" " " OUTPUT_VARIABLE LIBMESH_INC)
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LIBMESH_INC}")
# execute_process(COMMAND $ENV{FM_BASEPATH_TO_LM}/$ENV{FM_LM_FOLDER}/bin/libmesh-config --libs
#                 COMMAND tr "\n" " " OUTPUT_VARIABLE LIBMESH_LIBS)
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LIBMESH_LIBS}")
# ENDIF(LIBMESH_FOUND)
# old Make syntax:
# libmesh_INCLUDE  := $(shell ${FM_LM_DIR}/bin/libmesh-config --include)
# libmesh_CPPFLAGS := $(shell ${FM_LM_DIR}/bin/libmesh-config --cppflags)
# libmesh_LIBS     := $(shell ${FM_LM_DIR}/bin/libmesh-config --libs)


#############################################################################################
### Add sources
#############################################################################################

# Add the femus library sources
ADD_SUBDIRECTORY(src)



#############################################################################################
### Add Applications
#############################################################################################

# Add applications
ADD_SUBDIRECTORY(applications)



#############################################################################################
### Unit tests
#############################################################################################

INCLUDE(CTest)
# Add unit tests
IF(BUILD_TESTING)
  ENABLE_TESTING()
  ADD_SUBDIRECTORY(unittests)
ENDIF(BUILD_TESTING)

# message(petsc library = ${PETSC_LIBRARIES})
# message(derp library = ${METIS_LIBRARIES})
# message(b64 library = ${B64_LIBRARIES})
# message(json library = ${JSONCPP_LIBRARIES})
# message(adept library = ${ADEPT_LIBRARIES})
# message(fparser library = ${FPARSER_LIBRARY})
# message(mpi extra library = ${MPI_EXTRA_LIBRARY})
# message(hdf5 extra library = ${HDF5_LIBRARIES})
# message(slepc library = ${SLEPC_LIB})
# message(slepc library = SLEPC::slepc)
