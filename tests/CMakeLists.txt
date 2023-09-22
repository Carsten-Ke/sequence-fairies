
SET(isoformCleanertests_src ./unitTests/isoformCleaner/isoformCleaner_tests.cpp ${BSDL_src} ../src/identifyName.cpp ../src/GFFParser.cpp 
../src/isoformCleaner.cpp ../src/GFFParser.cpp
)
SET(isoformCleanertests_exe isoformCleaner-Test)
ADD_EXECUTABLE(${isoformCleanertests_exe} ${isoformCleanertests_src})
target_link_libraries(${isoformCleanertests_exe}
	${Boost_LIBRARIES}
)

SET(concatenatortests_src ./unitTests/concatenator/concatenator_tests.cpp ${BSDL_src} ../src/concatenator.cpp)
SET(concatenatortests_exe concatenator-Test)
ADD_EXECUTABLE(${concatenatortests_exe} ${concatenatortests_src})
target_link_libraries(${concatenatortests_exe}
	${Boost_LIBRARIES}
)
