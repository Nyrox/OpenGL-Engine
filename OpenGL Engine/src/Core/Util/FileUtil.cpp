#include "FileUtil.h"

namespace FUtil {

	std::stringstream stringstream_read_file(std::filesystem::path path) {
		std::ifstream file(path);

		std::stringstream stream;
		stream << file.rdbuf();

		file.close();
		return stream;
	}


	Json::Value json_read_file(std::filesystem::path path) {
		Json::Value out;
		std::ifstream file(path);
		file >> out;
		return out;
	}
}