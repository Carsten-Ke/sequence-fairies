cmake_minimum_required(VERSION 3.0.0)
project(sequence-fairies VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
enable_testing()


### FIND BOOST LIBRARY COMPONENTS
FIND_PACKAGE(Boost 1.70 COMPONENTS log log_setup system program_options iostreams filesystem REQUIRED)
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
ADD_DEFINITIONS( "-DHAS_BOOST" )

message(${PROJECT_SOURCE_DIR})
configure_file("${PROJECT_SOURCE_DIR}/src/cmake_generated/project_version.h.in" 
               "${PROJECT_SOURCE_DIR}/src/cmake_generated/project_version.h"
)

#include_directories(${CMAKE_CURRENT_BINARY_DIR})
set (BSDL_PATH ${PROJECT_SOURCE_DIR}/libs/BioSeqDataLib/src/)
set (BSDL_src ${BSDL_PATH}utility/Input.cpp ${BSDL_PATH}utility/Output.cpp ${BSDL_PATH}domain/Domain.cpp
${BSDL_PATH}domain/PfamDomain.cpp ${BSDL_PATH}domain/DomainExt.cpp ${BSDL_PATH}domain/SFDomain.cpp
${BSDL_PATH}domain/DomainArrangement.cpp ${BSDL_PATH}domain/DomainArrangementSet.cpp ${BSDL_PATH}utility/Settings.cpp
${BSDL_PATH}utility/DSM.cpp ${BSDL_PATH}utility/stringHelpers.cpp ${BSDL_PATH}utility/algorithm.cpp 
${BSDL_PATH}bio/GeneticCode.cpp ${BSDL_PATH}bio/Alphabet.cpp
)

SET(isoformCleaner_src ${PROJECT_SOURCE_DIR}/src/identifyName.cpp ${PROJECT_SOURCE_DIR}/src/isoformCleaner.cpp ${PROJECT_SOURCE_DIR}/src/isoformCleanerMain.cpp ${BSDL_src})
SET(isoformCleaner_exe isoformCleaner )
ADD_EXECUTABLE(${isoformCleaner_exe} ${isoformCleaner_src})
target_link_libraries(${isoformCleaner_exe}
	${Boost_LIBRARIES} ${ZLIB_LIBRARIES}
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

