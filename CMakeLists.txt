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

INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
ADD_DEFINITIONS( "-DHAS_BOOST" )


include(FetchContent)
FetchContent_Declare(
  BioSeqDataLib
  GIT_REPOSITORY https://zivgitlab.uni-muenster.de/domain-world/BioSeqDataLib.git
  GIT_TAG        ac97ba15646e90c4ebf63487fe34ab5e7d528cbc
  SOURCE_DIR     ${PROJECT_SOURCE_DIR}/libs/BioSeqDataLib
)

FetchContent_GetProperties(BioSeqDataLib)
if(NOT BioSeqDataLib_POPULATED)
    FetchContent_MakeAvailable(BioSeqDataLib)
endif()
message("Done")

SET(CMAKE_CXX_FLAGS_COVERAGE
    "-g -O0 --coverage -fprofile-arcs -ftest-coverage -fno-inline -fno-inline-small-functions -fno-default-inline"
    CACHE STRING "Flags used by the C++ compiler during coverage builds."
    FORCE )
SET(CMAKE_CXX_FLAGS_PROFILE
    "-O3 -pg"
    CACHE STRING "Flags used by the C++ compiler during coverage builds."
    FORCE )
SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE
    "--coverage"
    CACHE STRING "Flags used for linking binaries during coverage builds."
    FORCE )
SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
    "--coverage"
    CACHE STRING "Flags used by the shared libraries linker during coverage builds."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE )
    
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

######## SEQEXTRACT #############
add_library(seq OBJECT)
target_sources(seq
  PUBLIC
    FILE_SET CXX_MODULES FILES
    ${PROJECT_SOURCE_DIR}/src/seqExtract.cpp 
)

add_library(seq-header OBJECT  ${BSDL_src})
#target_sources(seq-header
#  PUBLIC
#    FILE_SET HEADERS FILES
#    ${BSDL_src}
#)


#set_property(TARGET seq PROPERTY CXX_STANDARD 20)
#set_property(TARGET seq PROPERTY CXX_STANDARD_REQUIRED ON)
#set_property(TARGET seq PROPERTY CXX_EXTENSIONS OFF)
set_property(TARGET seq-header PROPERTY CXX_STANDARD 20)
set_property(TARGET seq-header PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET seq-header PROPERTY CXX_EXTENSIONS OFF)

#target_compile_features(seq PUBLIC cxx_std_20)

SET(seqExtract_exe seqExtract )
ADD_EXECUTABLE(${seqExtract_exe} ${PROJECT_SOURCE_DIR}/src/seqExtractMain.cpp )
target_link_libraries(${seqExtract_exe} PRIVATE seq $<TARGET_OBJECTS:seq-header>
	${Boost_LIBRARIES} ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES}
)


######## ISOFORMCLEANER #############

SET(isoformCleaner_src 
    ${PROJECT_SOURCE_DIR}/src/GFFParser.cpp
    ${PROJECT_SOURCE_DIR}/src/identifyName.cpp 
    ${PROJECT_SOURCE_DIR}/src/isoformCleaner.cpp 
    ${PROJECT_SOURCE_DIR}/src/isoformCleanerMain.cpp 
)



SET(isoformCleaner_exe isoformCleaner )
ADD_EXECUTABLE(${isoformCleaner_exe} ${isoformCleaner_src})
target_sources(${isoformCleaner_exe} PRIVATE $<TARGET_OBJECTS:seq-header>)

target_link_libraries(${isoformCleaner_exe}
	${Boost_LIBRARIES} ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES}
)

######## CONCATENATOR #############

SET(concatenator_src 
    ${PROJECT_SOURCE_DIR}/src/concatenator.cpp 
    ${PROJECT_SOURCE_DIR}/src/concatenatorMain.cpp
)


SET(concatenator_exe concatenator)
ADD_EXECUTABLE(${concatenator_exe} ${concatenator_src})
target_link_libraries(${concatenator_exe}  $<TARGET_OBJECTS:seq-header>
	${Boost_LIBRARIES} ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES}
)

######## SEQCHECK #############

SET(seqCheck_src 
    ${PROJECT_SOURCE_DIR}/src/seqCheck.cpp  
    ${PROJECT_SOURCE_DIR}/src/seqCheckMain.cpp
)


SET(seqCheck_exe seqCheck)
ADD_EXECUTABLE(${seqCheck_exe} ${seqCheck_src})
target_link_libraries(${seqCheck_exe}  $<TARGET_OBJECTS:seq-header>
	${Boost_LIBRARIES} ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES}
)


######## domainExtract #############

SET(extractDomains_src 
    ${PROJECT_SOURCE_DIR}/src/extractDomains.cpp 
    ${PROJECT_SOURCE_DIR}/src/extractDomainsMain.cpp
)


SET(extractDomains_exe extractDomains)
ADD_EXECUTABLE(${extractDomains_exe} ${extractDomains_src})
target_link_libraries(${extractDomains_exe}  $<TARGET_OBJECTS:seq-header>
	${Boost_LIBRARIES} ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES}
)



######## TESTING #############


if (WITH_UNIT_TEST)
    include(CTest)
    enable_testing()
	add_subdirectory (tests)
	enable_testing ()
	add_test (NAME isoformCleanerTest COMMAND isoformCleaner-Test)
    add_test (NAME concatenatorTest COMMAND concatenator-Test)
endif ()



install(
  TARGETS
    concatenator
    extractDomains
    isoformCleaner
    seqCheck
 #   seqExtract
  DESTINATION
    bin
  )
