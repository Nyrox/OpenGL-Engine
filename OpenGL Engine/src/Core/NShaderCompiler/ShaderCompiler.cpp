#include "ShaderCompiler.h"
#include <regex>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>

using namespace std::experimental;

#include <Core/Util/FileUtil.h>

Shader ShaderCompiler::compile() {
	Shader shader;

	resolveIncludes();
	resolveMacros();

	shader.compile(vertexShaderSource, fragmentShaderSource, {});
	return shader;
}

void ShaderCompiler::resolveIncludes() {
	auto resolveFile = [&](std::string& file) {
		std::smatch match;
		std::regex includeRegex("#include\\(\"(.*)\"\\);");
		while (std::regex_search(file, match, includeRegex)) {
			bool foundFile = false;

			for (auto& it : includeDirectories) {
				auto filePath = it / match[1].str();

				if (std::filesystem::is_regular_file(filePath)) {
					foundFile = true;
					file = std::regex_replace(file, includeRegex, FUtil::stringstream_read_file(filePath).str());
				}
			}

			if (!foundFile) {
				throw std::runtime_error("Shader compiler| Couldn't resolve file include: " + match[1].str());
			}
		}
	};

	resolveFile(vertexShaderSource);
	resolveFile(fragmentShaderSource);
}

void ShaderCompiler::resolveMacros() {
	auto resolveFile = [&](std::string& file) {
		std::smatch match;
		std::regex macroRegex("\\$\\((.*)\\);");
		while (std::regex_search(file, match, macroRegex)) {
			auto macro = macroTable.find(match[1].str());

			file = std::regex_replace(file, macroRegex, macroTable.at(match[1].str()));
		}
	};

	resolveFile(vertexShaderSource);
	resolveFile(fragmentShaderSource);
}