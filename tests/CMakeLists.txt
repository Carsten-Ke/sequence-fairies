
SET(isoformCleanertests_src ./unitTests/isoformCleaner/isoformCleaner_tests.cpp ${BSDL_src} ../src/identifyName.cpp)
SET(isoformCleanertests_exe isoformCleaner-Test)
ADD_EXECUTABLE(${isoformCleanertests_exe} ${isoformCleanertests_src})
target_link_libraries(${isoformCleanertests_exe}
	${Boost_LIBRARIES}
)
