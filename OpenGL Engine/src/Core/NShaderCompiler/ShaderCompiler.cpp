#include "ShaderCompiler.h"
#include <regex>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>

using namespace std::experimental;

namespace {
	std::string getFileContents(const std::string& t_file) {
		std::stringstream out;
		
		try {
			std::ifstream file;
			file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			file.open(t_file);

			out << file.rdbuf();
			file.close();
		}
		catch (std::exception exception) {
			std::cout << "Couldn't open file: " << t_file << std::endl;
			std::cin.get();
		}
		
		return out.str();
	}
}

std::string compileShader(const std::string& t_source, const std::vector<std::string>& includeDirectories) {
	std::string source = t_source;	

	std::smatch match;
	std::regex includeRegex("#include\\(\"(.*)\"\\);");
	while (std::regex_search(source, match, includeRegex)) {
		bool couldFind = false;
		for (int i = 0; i < includeDirectories.size(); i++) {
			filesystem::path path = filesystem::path(includeDirectories.at(i)).append(match[1].str());
			
			if (filesystem::exists(path) && filesystem::is_regular_file(path)) {
				couldFind = true;
				source = std::regex_replace(source, includeRegex, getFileContents(path.string()));
				break;
			}
		}

		if (!couldFind) {
			std::cout << "ERROR::SHADER_COMPILER: Could not find file: " << match[1].str() << std::endl;
			std::cin.get();
		}

	}

	return source;
}