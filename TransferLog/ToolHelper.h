#pragma once

#include <string>
#include <unordered_map>

using namespace std;


class ToolHelper
{
public:
	ToolHelper(string file, unordered_map<string, string> hashmap);

	void Replace_all_Ditincte(string& str, const string& strFind, time_t startTime, ofstream& ofs);

	void GetInfo();

	void FindFiles();

	void DealJsonStr();

	time_t StringToDatetime(string str);

	~ToolHelper();

private:
	const unordered_map<string, string> m_hashmap;
	const string m_file;
};

