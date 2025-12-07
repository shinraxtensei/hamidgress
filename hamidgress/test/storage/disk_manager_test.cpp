#include <gtest/gtest.h>
#include <filesystem>
#include <cstring>
#include "hamidgress/storage/disk/disk_manager.h"
#include "hamidgress/common/exception.h"

namespace hamidgress {

class DiskManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_file_ = "/tmp/disk_manager_test.db";
        std::filesystem::remove(test_file_);
    }

    void TearDown() override {
        std::filesystem::remove(test_file_);
    }

    std::string test_file_;
};

TEST_F(DiskManagerTest, CreateNewFile) {
    DiskManager dm(test_file_);
    EXPECT_EQ(dm.GetNumPages(), 0);
    EXPECT_TRUE(std::filesystem::exists(test_file_));
}

TEST_F(DiskManagerTest, AllocateAndWritePage) {
    DiskManager dm(test_file_);

    PageId page_id = dm.AllocatePage();
    EXPECT_EQ(page_id, 0);

    char write_data[PAGE_SIZE];
    std::memset(write_data, 'A', PAGE_SIZE);
    dm.WritePage(page_id, write_data);

    char read_data[PAGE_SIZE];
    dm.ReadPage(page_id, read_data);

    EXPECT_EQ(std::memcmp(write_data, read_data, PAGE_SIZE), 0);
}

TEST_F(DiskManagerTest, PersistenceAcrossRestart) {
    PageId page_id;

    // Write data
    {
        DiskManager dm(test_file_);
        page_id = dm.AllocatePage();

        char data[PAGE_SIZE];
        std::memset(data, 0x42, PAGE_SIZE);
        dm.WritePage(page_id, data);
    }

    // Read after restart
    {
        DiskManager dm(test_file_);
        EXPECT_EQ(dm.GetNumPages(), 1);

        char data[PAGE_SIZE];
        dm.ReadPage(page_id, data);

        for (size_t i = 0; i < PAGE_SIZE; i++) {
            EXPECT_EQ(static_cast<unsigned char>(data[i]), 0x42);
        }
    }
}

TEST_F(DiskManagerTest, MultiplePages) {
    DiskManager dm(test_file_);

    constexpr int NUM_PAGES = 100;

    // Allocate and write pages
    for (int i = 0; i < NUM_PAGES; i++) {
        PageId page_id = dm.AllocatePage();
        EXPECT_EQ(page_id, static_cast<PageId>(i));

        char data[PAGE_SIZE];
        std::memset(data, i, PAGE_SIZE);
        dm.WritePage(page_id, data);
    }

    EXPECT_EQ(dm.GetNumPages(), NUM_PAGES);

    // Verify all pages
    for (int i = 0; i < NUM_PAGES; i++) {
        char data[PAGE_SIZE];
        dm.ReadPage(i, data);

        for (size_t j = 0; j < PAGE_SIZE; j++) {
            EXPECT_EQ(static_cast<unsigned char>(data[j]),
                     static_cast<unsigned char>(i));
        }
    }
}

TEST_F(DiskManagerTest, ReadPastEndOfFileThrows) {
    DiskManager dm(test_file_);

    char data[PAGE_SIZE];
    EXPECT_THROW(dm.ReadPage(0, data), IOException);
}

TEST_F(DiskManagerTest, DeallocatePage) {
    DiskManager dm(test_file_);

    PageId page_id = dm.AllocatePage();

    char write_data[PAGE_SIZE];
    std::memset(write_data, 'X', PAGE_SIZE);
    dm.WritePage(page_id, write_data);

    // Deallocate (zeros the page)
    dm.DeallocatePage(page_id);

    char read_data[PAGE_SIZE];
    dm.ReadPage(page_id, read_data);

    // Page should be all zeros
    for (size_t i = 0; i < PAGE_SIZE; i++) {
        EXPECT_EQ(read_data[i], 0);
    }
}

TEST_F(DiskManagerTest, AllocateMultiplePagesSequential) {
    DiskManager dm(test_file_);

    PageId id1 = dm.AllocatePage();
    PageId id2 = dm.AllocatePage();
    PageId id3 = dm.AllocatePage();

    EXPECT_EQ(id1, 0);
    EXPECT_EQ(id2, 1);
    EXPECT_EQ(id3, 2);
    EXPECT_EQ(dm.GetNumPages(), 3);
}

TEST_F(DiskManagerTest, WriteAndReadDifferentPatterns) {
    DiskManager dm(test_file_);

    PageId page1 = dm.AllocatePage();
    PageId page2 = dm.AllocatePage();

    // Page 1: all 0xAA
    char data1[PAGE_SIZE];
    std::memset(data1, 0xAA, PAGE_SIZE);
    dm.WritePage(page1, data1);

    // Page 2: all 0x55
    char data2[PAGE_SIZE];
    std::memset(data2, 0x55, PAGE_SIZE);
    dm.WritePage(page2, data2);

    // Verify both pages have correct data
    char read1[PAGE_SIZE];
    char read2[PAGE_SIZE];
    dm.ReadPage(page1, read1);
    dm.ReadPage(page2, read2);

    EXPECT_EQ(std::memcmp(data1, read1, PAGE_SIZE), 0);
    EXPECT_EQ(std::memcmp(data2, read2, PAGE_SIZE), 0);
}

TEST_F(DiskManagerTest, SyncFlushesData) {
    DiskManager dm(test_file_);

    PageId page_id = dm.AllocatePage();

    char data[PAGE_SIZE];
    std::memset(data, 0x99, PAGE_SIZE);
    dm.WritePage(page_id, data);

    // Explicitly sync
    EXPECT_NO_THROW(dm.Sync());

    // Verify data is still readable
    char read_data[PAGE_SIZE];
    dm.ReadPage(page_id, read_data);
    EXPECT_EQ(std::memcmp(data, read_data, PAGE_SIZE), 0);
}

}  // namespace hamidgress
