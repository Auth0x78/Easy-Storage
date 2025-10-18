#include "DataLoadSave.h"
#include <vector>

ErrorType DataLoadSave::SaveData(const std::string &filepath, void *data,
                                 uint64_t size) {
  if (size == 0 || data == nullptr) {
    return ErrorType::FileEmpty;
  }

  XXHash64 hasher(m_seed);
  hasher.add(data, size);

  FileHeader header;
  header.dataSize = size;
  header.hash = hasher.hash();

  std::ofstream file(filepath, std::ios::binary | std::ios::trunc);
  if (!file) {
    return ErrorType::FailToOpenFile;
  }

  file.write(reinterpret_cast<const char *>(&header), sizeof(FileHeader));
  if (!file) {
    file.close();
    return ErrorType::FailToWrite;
  }

  file.write(reinterpret_cast<const char *>(data), header.dataSize);
  if (!file) {
    file.close();
    return ErrorType::FailToWrite;
  }

  file.close();
  return ErrorType::Success;
}

std::expected<std::string, ErrorType>
DataLoadSave::LoadData(const std::string &filepath) {
  // Open file for reading
  std::ifstream file(filepath, std::ios::binary);
  if (!file) {
    return std::unexpected(ErrorType::FailToOpenFile);
  }

  // Read the header
  FileHeader header;
  file.read(reinterpret_cast<char *>(&header), sizeof(FileHeader));
  if (file.gcount() != sizeof(FileHeader)) {
    // File is too small to even contain the header
    file.close();
    return std::unexpected(ErrorType::FileEmpty);
  }

  // Create a string of the correct size.
  std::string loaded_data;
  loaded_data.resize(header.dataSize);

  // Read the rest of the file (the data) directly into the string
  file.read(loaded_data.data(), header.dataSize);
  if (file.gcount() != header.dataSize) {
    // File was shorter than the header promised
    file.close();
    return std::unexpected(ErrorType::FailToRead);
  }

  file.close();

  // Verify the hash
  XXHash64 hasher(m_seed);
  hasher.add(loaded_data.data(), loaded_data.size());

  if (hasher.hash() != header.hash) {
    return std::unexpected(ErrorType::FileDataCorrupted);
  }

  // The compiler will optimize this return using NRVO
  return loaded_data;
}