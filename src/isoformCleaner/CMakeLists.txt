######## ISOFORMCLEANER #############


add_library(isoformCleaner-lib OBJECT)
target_sources(isoformCleaner-lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    GFFParser.cpp
    identifyName.cpp 
    isoformCleaner.cpp 
)

ADD_EXECUTABLE(isoformCleaner isoformCleanerMain.cpp)
target_link_libraries(isoformCleaner PRIVATE isoformCleaner-lib $<TARGET_OBJECTS:BSDL>
    Boost::program_options Boost::iostreams ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES}
)