FILE(GLOB TEST_SOURCES "*.cpp")
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

add_executable(blast_tests ${TEST_SOURCES})

target_link_libraries(blast_tests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} SDL2-2.0 GLEW GL )

add_test(UnitTests blast_tests)
