#pragma once

#include <atomic>
#include <fstream>
#include <shared_mutex>
#include <string>
#include "hamidgress/common/config.h"
#include "hamidgress/common/types.h"

namespace hamidgress {

class DiskManager {
public:
    explicit DiskManager(const std::string& db_file);
    ~DiskManager();

    // Disable copy
    DiskManager(const DiskManager&) = delete;
    DiskManager& operator=(const DiskManager&) = delete;

    // Move is OK
    DiskManager(DiskManager&&) noexcept;
    DiskManager& operator=(DiskManager&&) noexcept;

    /**
     * @brief Read a page from disk into the buffer.
     * @param page_id ID of the page to read.
     * @param page_data Buffer to read into (must be PAGE_SIZE bytes).
     */
    void ReadPage(PageId page_id, char* page_data);

    /**
     * @brief Write a page to disk.
     * @param page_id ID of the page to write.
     * @param page_data Buffer containing page data (PAGE_SIZE bytes).
     */
    void WritePage(PageId page_id, const char* page_data);

    /**
     * @brief Allocate a new page on disk.
     * @return The ID of the newly allocated page.
     */
    auto AllocatePage() -> PageId;

    /**
     * @brief Deallocate a page (mark as free).
     * @param page_id ID of the page to deallocate.
     */
    void DeallocatePage(PageId page_id);

    /**
     * @brief Get the number of pages in the file.
     */
    auto GetNumPages() const -> size_t;

    /**
     * @brief Flush all writes to disk.
     */
    void Sync();

private:
    auto GetFileOffset(PageId page_id) const -> std::streampos {
        return static_cast<std::streampos>(page_id) * PAGE_SIZE;
    }

    std::string file_name_;
    std::fstream db_file_;
    std::atomic<PageId> next_page_id_{0};
    mutable std::shared_mutex latch_;
};

}  // namespace hamidgress
