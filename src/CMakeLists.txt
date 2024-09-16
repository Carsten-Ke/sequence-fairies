

add_subdirectory(isoformCleaner)

######## SEQEXTRACT #############

add_library(seqExtract-lib OBJECT)
target_sources(seqExtract-lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    ${PROJECT_SOURCE_DIR}/src/seqExtract.cpp 
)

SET(seqExtract_exe seqExtract )
ADD_EXECUTABLE(${seqExtract_exe} ${PROJECT_SOURCE_DIR}/src/seqExtractMain.cpp )
target_link_libraries(${seqExtract_exe} PRIVATE seqExtract-lib $<TARGET_OBJECTS:BSDL>
	Boost::program_options Boost::iostreams ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES}
)


######## CONCATENATOR #############

add_library(concatenator-lib OBJECT)
target_sources(concatenator-lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    ${PROJECT_SOURCE_DIR}/src/concatenator.cpp 
)

SET(concatenator_src 
    ${PROJECT_SOURCE_DIR}/src/concatenatorMain.cpp
)


SET(concatenator_exe concatenator)
ADD_EXECUTABLE(${concatenator_exe} ${concatenator_src})
target_link_libraries(${concatenator_exe} PRIVATE concatenator-lib $<TARGET_OBJECTS:BSDL>
    Boost::program_options Boost::iostreams ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES}
)

######## SEQCHECK #############

add_library(seqCheck-lib OBJECT)
target_sources(seqCheck-lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    ${PROJECT_SOURCE_DIR}/src/seqCheck.cpp  
)

SET(seqCheck_exe seqCheck)
ADD_EXECUTABLE(${seqCheck_exe} ${PROJECT_SOURCE_DIR}/src/seqCheckMain.cpp)
target_link_libraries(${seqCheck_exe} PRIVATE seqCheck-lib $<TARGET_OBJECTS:BSDL>
    Boost::program_options Boost::iostreams ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES}
)


######## domainExtract #############

SET(extractDomains_src 
    ${PROJECT_SOURCE_DIR}/src/extractDomains.cpp 
    ${PROJECT_SOURCE_DIR}/src/extractDomainsMain.cpp
)


SET(extractDomains_exe extractDomains)
ADD_EXECUTABLE(${extractDomains_exe} ${extractDomains_src})
target_link_libraries(${extractDomains_exe}  $<TARGET_OBJECTS:BSDL>
    Boost::program_options Boost::iostreams ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES}
)

