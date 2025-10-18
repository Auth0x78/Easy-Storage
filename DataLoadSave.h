#pragma once
#include "xxhash64.h"
#include <expected>
#include <fstream>
#include <string>

enum class ErrorType {
  Success,
  FailToOpenFile,
  FileDataCorrupted,
  FileEmpty,
  FailToWrite,
  FailToRead
};

class DataLoadSave {
public:
  DataLoadSave() = delete;

  static ErrorType SaveData(const std::string &filepath, void *data,
                            uint64_t size);

  // *** FIX: Removed the unused 'expected_size' parameter ***
  static std::expected<std::string, ErrorType>
  LoadData(const std::string &filepath);

private:
  static constexpr uint64_t m_seed = 0xFF0DAD00BAD0;

  struct FileHeader {
    uint64_t hash;
    uint64_t dataSize;
  };
};