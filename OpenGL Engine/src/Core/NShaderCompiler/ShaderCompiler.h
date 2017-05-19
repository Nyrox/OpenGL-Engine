#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

namespace std {
	namespace filesystem = std::experimental::filesystem;
}

#include <Core/Shader.h>


class ShaderCompiler {
public:
	Shader compile();

	void defineMacro(std::string macro, std::string contents) { macroTable.insert(std::make_pair(macro, contents)); };
	void addIncludeDirectory(std::filesystem::path path) { includeDirectories.push_back(path); }
	
	void setVertexShaderSource(std::string source) { vertexShaderSource = source; }
	void setFragmentShaderSource(std::string source) { fragmentShaderSource = source; }
private:
	void resolveIncludes();
	void resolveMacros();

	std::unordered_map<std::string, std::string> macroTable;
	std::vector<std::filesystem::path> includeDirectories;

	std::string vertexShaderSource;
	std::string fragmentShaderSource;
};