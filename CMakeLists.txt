cmake_minimum_required(VERSION 3.15.0)
project(sequence-fairies VERSION 0.1.0 LANGUAGES CXX)

### FIND BOOST LIBRARY COMPONENTS


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



find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/libs/BioSeqDataLib/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()



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
    #CMAKE_C_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE )


set(CMAKE_CXX_FLAGS "-std=c++20 -Wall -Wextra -Wunused -pedantic")

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release CACHE STRING
       "Choose the type of build, options are: Debug Coverage Release RelWithDebInfo MinSizeRel." FORCE )
endif()


message(${PROJECT_SOURCE_DIR})
configure_file("${PROJECT_SOURCE_DIR}/src/cmake_generated/project_version.h.in" 
               "${PROJECT_SOURCE_DIR}/src/cmake_generated/project_version.h"
)

#include_directories(${CMAKE_CURRENT_BINARY_DIR})
set (BSDL_PATH ${PROJECT_SOURCE_DIR}/libs/BioSeqDataLib/src/)
set (BSDL_src ${BSDL_PATH}utility/Input.cpp 
    ${BSDL_PATH}utility/Output.cpp
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

SET(isoformCleaner_src 
    ${PROJECT_SOURCE_DIR}/src/identifyName.cpp 
    ${PROJECT_SOURCE_DIR}/src/isoformCleaner.cpp 
    ${PROJECT_SOURCE_DIR}/src/isoformCleanerMain.cpp ${BSDL_src}
)


SET(isoformCleaner_exe isoformCleaner )
ADD_EXECUTABLE(${isoformCleaner_exe} ${isoformCleaner_src})
target_link_libraries(${isoformCleaner_exe}
	${Boost_LIBRARIES} ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES}
)


SET(concatenator_src 
    ${PROJECT_SOURCE_DIR}/src/concatenator.cpp 
    ${PROJECT_SOURCE_DIR}/src/concatenatorMain.cpp ${BSDL_src}
)


SET(concatenator_exe concatenator)
ADD_EXECUTABLE(${concatenator_exe} ${concatenator_src})
target_link_libraries(${concatenator_exe}
	${Boost_LIBRARIES} ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES}
)



if (WITH_UNIT_TEST)
    include(CTest)
    enable_testing()
	add_subdirectory (tests)
	enable_testing ()
	add_test (NAME isoformCleanerTest COMMAND isoformCleaner-Test)
endif ()

install(
  TARGETS
    isoformCleaner
    concatenator
  DESTINATION
    bin
  )
