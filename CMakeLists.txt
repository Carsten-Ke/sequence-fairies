cmake_minimum_required(VERSION 3.30.0)
project(
  sequence-fairies 
  VERSION 1.0.0 
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (WITH_UNIT_TEST)
  FIND_PACKAGE(Boost 1.70 COMPONENTS log log_setup system program_options iostreams unit_test_framework REQUIRED)
else (WITH_UNIT_TEST)
  FIND_PACKAGE(Boost 1.70 COMPONENTS log log_setup system program_options iostreams REQUIRED)
endif(WITH_UNIT_TEST)

find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)

include(FetchContent)
FetchContent_Declare(
  BioSeqDataLib
  GIT_REPOSITORY https://zivgitlab.uni-muenster.de/domain-world/BioSeqDataLib.git
  GIT_TAG        ac97ba15646e90c4ebf63487fe34ab5e7d528cbc
  SOURCE_DIR     ${PROJECT_SOURCE_DIR}/libs/BioSeqDataLib
  EXCLUDE_FROM_ALL
)

FetchContent_GetProperties(BioSeqDataLib)
if(NOT BioSeqDataLib_POPULATED)
    FetchContent_MakeAvailable(BioSeqDataLib)
endif()
add_compile_options(-Wall -Wextra -Wpedantic)

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release CACHE STRING
       "Choose the type of build, options are: Debug Coverage Release RelWithDebInfo MinSizeRel." FORCE )
endif()

configure_file("${PROJECT_SOURCE_DIR}/src/cmake_generated/project_version.h.in" 
               "${PROJECT_SOURCE_DIR}/src/cmake_generated/project_version.h"
)

set (BSDL_PATH ${PROJECT_SOURCE_DIR}/libs/BioSeqDataLib/src/)
set (BSDL_src 
    ${BSDL_PATH}utility/Input.cpp 
    ${BSDL_PATH}utility/Output.cpp
    ${BSDL_PATH}gff/GFFRecord.cpp
    ${BSDL_PATH}domain/Domain.cpp
    ${BSDL_PATH}domain/PfamDomain.cpp
    ${BSDL_PATH}domain/DomainExt.cpp
    ${BSDL_PATH}domain/DomainArrangement.cpp 
    ${BSDL_PATH}domain/DomainArrangement_util.cpp 
    ${BSDL_PATH}domain/DomainArrangementSet.cpp 
    ${BSDL_PATH}utility/Settings.cpp
    ${BSDL_PATH}utility/DSM.cpp 
    ${BSDL_PATH}utility/stringHelpers.cpp 
    ${BSDL_PATH}/sequence/IOStrategies/ClustalReader.cpp
    ${BSDL_PATH}/sequence/IOStrategies/FASTAReader.cpp
    ${BSDL_PATH}/sequence/IOStrategies/FASTAWriter.cpp
    ${BSDL_PATH}/sequence/IOStrategies/IndexingFASTAReader.cpp
    ${BSDL_PATH}/sequence/IOStrategies/MSFReader.cpp
    ${BSDL_PATH}/sequence/IOStrategies/PhylipReader.cpp
    ${BSDL_PATH}/sequence/IOStrategies/StockholmReader.cpp
    ${BSDL_PATH}bio/GeneticCode.cpp
    ${BSDL_PATH}bio/Alphabet.cpp
    ${BSDL_PATH}/sequence/Sequence.cpp
    ${BSDL_PATH}utility/WeightedInterval_util.cpp
)

add_library(BSDL OBJECT  ${BSDL_src})


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_subdirectory(src)

######## TESTING #############


if (WITH_UNIT_TEST)
  include(CTest)
  enable_testing()
	add_subdirectory (tests)
	enable_testing ()
	add_test (NAME isoformCleanerTest COMMAND isoformCleaner-Test)
  add_test (NAME concatenatorTest COMMAND concatenator-Test)
endif ()



######## INSTALLING #############

install(
  TARGETS
    concatenator
    extractDomains
    isoformCleaner
    seqCheck
    seqExtract
  DESTINATION
    bin
  )
