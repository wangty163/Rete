#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

namespace CLog
{
	void write(const std::string& type, const std::string& infos);
	void write(const std::string& infos);
};

