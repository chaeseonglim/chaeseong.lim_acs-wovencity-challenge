set(CMAKE_CXX_STANDARD 17)

set(REPORT_DIR ${CMAKE_CURRENT_BINARY_DIR}/reports/)

# Build and set up the unit test
add_executable(gtest_Lock gtest_Lock.cpp)
target_link_libraries(gtest_Lock PRIVATE Challenge::Lock GTest::Main)

gtest_discover_tests( gtest_Lock
    XML_OUTPUT_DIR ${REPORT_DIR}
    DISCOVERY_MODE PRE_TEST
    DISCOVERY_TIMEOUT ${GTEST_TIMEOUT}
    )

# Build the test binary
add_executable(test_lock test_lock.cpp)
target_link_libraries(test_lock PRIVATE Challenge::Lock pthread)
install(TARGETS test_lock DESTINATION bin)
