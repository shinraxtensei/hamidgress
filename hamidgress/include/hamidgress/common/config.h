#pragma once

#include <cstddef>

namespace hamidgress {

// Storage constants
constexpr size_t PAGE_SIZE = 8192;                    // 8KB
constexpr size_t PAGE_HEADER_SIZE = 24;               // PageHeaderData
constexpr size_t ITEM_ID_SIZE = 4;                    // ItemIdData
constexpr size_t HEAP_TUPLE_HEADER_SIZE = 23;         // HeapTupleHeaderData

// Buffer pool defaults
constexpr size_t DEFAULT_BUFFER_POOL_SIZE = 128;      // 128 pages = 1MB
constexpr size_t DEFAULT_BUFFER_POOL_INSTANCES = 1;

// WAL constants
constexpr size_t WAL_SEGMENT_SIZE = 16 * 1024 * 1024; // 16MB
constexpr size_t WAL_BUFFER_SIZE = 16 * 1024 * 1024;  // 16MB

// Limits
constexpr size_t MAX_TUPLE_SIZE = PAGE_SIZE - PAGE_HEADER_SIZE - ITEM_ID_SIZE;
constexpr size_t MAX_COLUMNS = 1600;

}
