#pragma once

#include <cstdint>
#include <limits>

namespace hamidgress {

// Page and block IDs
using PageId = uint32_t;
using FrameId = uint32_t;

constexpr PageId INVALID_PAGE_ID = std::numeric_limits<PageId>::max();
constexpr FrameId INVALID_FRAME_ID = std::numeric_limits<FrameId>::max();

// Transaction IDs
using TransactionId = uint32_t;
constexpr TransactionId INVALID_XID = 0;
constexpr TransactionId BOOTSTRAP_XID = 1;
constexpr TransactionId FROZEN_XID = 2;
constexpr TransactionId FIRST_NORMAL_XID = 3;

// Log Sequence Number
using LSN = uint64_t;
constexpr LSN INVALID_LSN = 0;

// Object IDs (for catalog)
using Oid = uint32_t;
constexpr Oid INVALID_OID = 0;

// Tuple ID (physical location)
struct TupleId {
    PageId page_id;
    uint16_t slot_num;

    auto operator==(const TupleId&) const -> bool = default;
    auto operator!=(const TupleId&) const -> bool = default;

    auto IsValid() const -> bool {
        return page_id != INVALID_PAGE_ID;
    }
};

constexpr TupleId INVALID_TUPLE_ID = {INVALID_PAGE_ID, 0};

// Type IDs for the type system
enum class TypeId : uint8_t {
    INVALID = 0,
    BOOLEAN = 1,
    TINYINT = 2,    // int8
    SMALLINT = 3,   // int16
    INTEGER = 4,    // int32
    BIGINT = 5,     // int64
    DECIMAL = 6,
    FLOAT = 7,
    DOUBLE = 8,
    TIMESTAMP = 9,
    DATE = 10,
    VARCHAR = 11,
    TEXT = 12,
};

}  
