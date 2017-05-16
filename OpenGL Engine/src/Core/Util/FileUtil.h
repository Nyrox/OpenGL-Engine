#pragma once
#include <Core/JSON/json.h>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace std {
	namespace filesystem = std::experimental::filesystem;
}

namespace FUtil {

	std::stringstream stringstream_read_file(std::filesystem::path path);
	Json::Value json_read_file(std::filesystem::path path);
}