set(STORAGE_SOURCES
    disk/disk_manager.cpp
)

add_library(hamidgress_storage ${STORAGE_SOURCES})
target_link_libraries(hamidgress_storage PUBLIC
    hamidgress_common
    Threads::Threads
)
target_include_directories(hamidgress_storage PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
