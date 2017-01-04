#pragma once
#include <fstream>
#include <filesystem>

namespace std {
	using namespace experimental;
}

class Log {
public:

	~Log() {
		file.close();
	}

	static void write(std::string message) {
		if (!Log::getInstance().file.is_open()) return;

		Log::getInstance().file.write(message.c_str(), message.size());
	}

private:
	std::ofstream file;

	Log() {
		file.open("logs/main.log", std::ios::trunc);
		file << "Info: Starting Logfile" << std::endl;
		file << "Info: Working directory: " << std::filesystem::current_path() << std::endl;
	}

	static Log& getInstance() {
		static Log instance;
		return instance;
	}

};