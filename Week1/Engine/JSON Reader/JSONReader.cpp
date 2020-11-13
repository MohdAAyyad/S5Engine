#include "JSONReader.h"

std::unique_ptr<JSONReader> JSONReader::reader = nullptr;

JSONReader::JSONReader()
{
}

JSONReader::~JSONReader()
{
}

JSONReader* JSONReader::GetInstance()
{
	if (reader.get() == nullptr)
	{
		reader.reset(new JSONReader());
	}

	return reader.get();
}

void JSONReader::LoadFile(std::string filePath_)
{
	std::ifstream file(filePath_);

	file >> j;

	file.close();
}
