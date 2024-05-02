#pragma once
#include "xxhash64.h"
#include <string>
#include <fstream>

#define SRC_SIZE_NOT_SUFFICIENT -4
#define FILE_EMPTY -3
#define FILE_CORRUPT -2
#define FILE_OPEN_FAIL -1
#define SUCCESS 0

class DataLoadSave
{
public:
	DataLoadSave();
	
	int SaveData(const std::string& filepath, void* data, uint64_t size);

	int LoadData(const std::string& filepath, char* src, uint64_t src_size);
private:
	uint64_t m_seed = 777;
};