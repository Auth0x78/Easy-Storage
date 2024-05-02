#include "DataLoadSave.h"


DataLoadSave::DataLoadSave()
:m_seed(777){}

int DataLoadSave::SaveData(const std::string& filepath, void* data, uint64_t size)
{
	//Open file for writing in binary mode
	std::ofstream file(filepath, std::ios_base::binary | std::ios_base::out);
	
	//Check if failed to open file
	if (file.fail())
		return FILE_OPEN_FAIL;

	//Generate a hash
	XXHash64 hasher(m_seed);
	hasher.add(data, size);

	//Store the hash
	uint64_t hash = hasher.hash();

	//Starting the actual writing of data
	file.write((char*)data, size);
	file.write((char*)&hash, sizeof(hash));

	//Close the file
	file.close();

	//Return 0 if successful
	return SUCCESS;
}

int DataLoadSave::LoadData(const std::string& filepath, char* src, uint64_t src_size)
{
	//Open file for Loading data
	std::ifstream file(filepath, std::ios_base::binary);

	//Check if file is open
	if (file.fail())
		return FILE_OPEN_FAIL;

	//Start by calculating the file size
	file.seekg(0, std::ios::end);
	uint64_t fsize = file.tellg();

	//Reset moved file pointer to start again
	file.seekg(0, std::ios::beg);

	//if file is empty return -3
	if (fsize == 0) 
	{
		file.close();
		return FILE_EMPTY;
	}

	//Subtract the last 64 bits or 8 bytes of hash to get data struct size
	uint64_t datasize = fsize - sizeof(uint64_t);

	if (datasize > src_size)
	{
		file.close();
		return SRC_SIZE_NOT_SUFFICIENT;
	}

	//Read the file's data into address of src
	file.read(src, src_size);

	//Read the file checksum stored at the last of the file
	uint64_t expectedHash = 0;
	file.read((char*)&expectedHash, sizeof(expectedHash));

	//Generate hash of the read data
	XXHash64 hasher(m_seed);
	hasher.add(&src, datasize);

	//if checksum doesnt match then file is probably corrupted
	if (expectedHash != hasher.hash())
	{
		file.close();
		return FILE_CORRUPT;
	}

	file.close();
	return SUCCESS;
}

