#ifndef JSONREADER_H
#define JSONREADER_h
#include <nlohmann/json.hpp>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <memory>

class JSONReader
{
public:
	JSONReader();
	~JSONReader();
	JSONReader(const JSONReader&) = delete; //Disable the copy constructor
	JSONReader(JSONReader&&) = delete; //Disable the move constructor
	JSONReader& operator=(const JSONReader&) = delete; //Disable operator = for copy and move
	JSONReader& operator=(JSONReader&&) = delete;
	static JSONReader* GetInstance();

	void LoadFile(std::string filePath_);

	inline nlohmann::json GetJSONVar()
	{
		return j;
	}

	template<typename T>
	T FindValue(std::string query_)
	{
		return j[query_].get<T>();
	}

	template<typename T>
	T FindNestedValue(std::string query_)
	{
		for (auto v : j.items())
		{
			if (v.key() == query_)
			{
				return j[v.key()].get<T>();
			}
			else if(v.key() != "")
			{
				for (auto v1 : j[v.key()].items())
				{

					if (v1.key() == query_)
					{
						return j[v.key()][v1.key()].get<T>();
					}
					else if(v1.key() != "")
					{
						for (auto v2 : j[v.key()][v1.key()].items())
						{
							if (v2.key() == query_)
							{
								return j[v.key()][v1.key()][v2.key()].get<T>();
							}
						}
					}
				}
			}	
		}




		return T();
	}

private:
	nlohmann::json j;

	static std::unique_ptr<JSONReader> reader;
	friend std::default_delete<JSONReader>;

};
#endif

