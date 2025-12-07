set(TEST_SOURCES
    storage/disk_manager_test.cpp
)

foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    target_link_libraries(${test_name}
        hamidgress_storage
        hamidgress_common
        GTest::gtest
        GTest::gtest_main
        Threads::Threads
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
