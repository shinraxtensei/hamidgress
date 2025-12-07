#include "hamidgress/storage/disk/disk_manager.h"

#include <cstring>
#include <filesystem>

#include "hamidgress/common/exception.h"

namespace hamidgress {

DiskManager::DiskManager(const std::string& db_file) : file_name_(db_file) {
  // Open file, create if not exists
  db_file_.open(db_file, std::ios::in | std::ios::out | std::ios::binary);

  if (!db_file_.is_open()) {
    // File doesn't exist, create it
    db_file_.clear();
    db_file_.open(db_file, std::ios::in | std::ios::out | std::ios::binary |
                               std::ios::trunc);

    if (!db_file_.is_open()) {
      throw IOException("Cannot open database file: " + db_file);
    }
  }

  // Determine number of existing pages
  db_file_.seekg(0, std::ios::end);
  auto file_size = db_file_.tellg();
  next_page_id_ = static_cast<PageId>(file_size / PAGE_SIZE);
}

DiskManager::~DiskManager() {
  if (db_file_.is_open()) {
    Sync();
    db_file_.close();
  }
}

DiskManager::DiskManager(DiskManager&& other) noexcept
    : file_name_(std::move(other.file_name_)),
      db_file_(std::move(other.db_file_)),
      next_page_id_(other.next_page_id_.load()) {}

DiskManager& DiskManager::operator=(DiskManager&& other) noexcept {
  if (this != &other) {
    if (db_file_.is_open()) {
      Sync();
      db_file_.close();
    }
    file_name_ = std::move(other.file_name_);
    db_file_ = std::move(other.db_file_);
    next_page_id_ = other.next_page_id_.load();
  }
  return *this;
}

void DiskManager::ReadPage(PageId page_id, char* page_data) {
  std::shared_lock lock(latch_);

  if (page_id >= next_page_id_) {
    throw IOException("Read past end of file: page " + std::to_string(page_id));
  }

  auto offset = GetFileOffset(page_id);
  db_file_.seekg(offset);

  if (!db_file_.good()) {
    throw IOException("Failed to seek to page " + std::to_string(page_id));
  }

  db_file_.read(page_data, PAGE_SIZE);

  if (db_file_.gcount() != PAGE_SIZE) {
    throw IOException("Failed to read page " + std::to_string(page_id));
  }
}

void DiskManager::WritePage(PageId page_id, const char* page_data) {
  std::unique_lock lock(latch_);

  auto offset = GetFileOffset(page_id);
  db_file_.seekp(offset);

  if (!db_file_.good()) {
    throw IOException("Failed to seek for write: page " +
                      std::to_string(page_id));
  }

  db_file_.write(page_data, PAGE_SIZE);

  if (!db_file_.good()) {
    throw IOException("Failed to write page " + std::to_string(page_id));
  }

  // Update page count if necessary
  if (page_id >= next_page_id_) {
    next_page_id_ = page_id + 1;
  }
}

auto DiskManager::AllocatePage() -> PageId {
  return next_page_id_.fetch_add(1, std::memory_order_acq_rel);
}

void DiskManager::DeallocatePage(PageId page_id) {
  // For now, just zero out the page
  // Real implementation would use a free list or FSM
  char zeros[PAGE_SIZE] = {0};
  WritePage(page_id, zeros);
}

auto DiskManager::GetNumPages() const -> size_t {
  return next_page_id_.load(std::memory_order_acquire);
}

void DiskManager::Sync() {
  std::unique_lock lock(latch_);
  db_file_.flush();

#ifdef __linux__
  // Also fsync for durability on Linux
  // Note: This requires getting file descriptor from fstream
  // For full implementation, we'd use POSIX open/read/write instead of fstream
#endif
}

}  // namespace hamidgress
