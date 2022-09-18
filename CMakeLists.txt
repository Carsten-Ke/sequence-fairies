cmake_minimum_required(VERSION 3.0.0)
project(sequence-fairies VERSION 0.1.0)

include(CTest)
enable_testing()


### FIND BOOST LIBRARY COMPONENTS
FIND_PACKAGE(Boost 1.70 COMPONENTS system program_options iostreams REQUIRED)
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
ADD_DEFINITIONS( "-DHAS_BOOST" )

message(${PROJECT_SOURCE_DIR})
configure_file("${PROJECT_SOURCE_DIR}/src/cmake_generated/project_version.h.in" 
               "${PROJECT_SOURCE_DIR}/src/cmake_generated/project_version.h"
)


SET(isoformCleaner_src ${PROJECT_SOURCE_DIR}/src/isoformCleaner.cpp ${PROJECT_SOURCE_DIR}/src/isoformCleanerMain.cpp)
SET(isoformCleaner_exe isoformCleaner )
ADD_EXECUTABLE(${isoformCleaner_exe} ${isoformCleaner_src})
target_link_libraries(${isoformCleaner_exe}
	${Boost_LIBRARIES} ${ZLIB_LIBRARIES}
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

