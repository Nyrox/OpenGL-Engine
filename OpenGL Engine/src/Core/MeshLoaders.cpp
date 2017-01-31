#include "MeshLoaders.h"
#include <Core\Mesh.h>

#include <sstream>
#include <fstream>
#include <vector>

namespace MeshLoaders {

	/*
		Splits a string into tokens using a seperator
		@[params]
		@haystack: The string to split
		@delim: The character on which to split. The character if found will be discarded.
	*/
	std::vector<std::string> string_split (const std::string& haystack, char delim) {
		std::vector<std::string> tokens;

		uint32_t prev = 0, pos = 0;
		do {
			pos = haystack.find(delim, prev);
			if (pos == std::string::npos) pos = haystack.length();
			std::string token = haystack.substr(prev, pos - prev);
			if (!token.empty()) tokens.push_back(token);
			prev = pos + 1;
		} while (pos < haystack.length() && prev < haystack.length());

		return tokens;
	}

	/* */
	void loadFromFile (Mesh& mesh, const std::string& file, void(*loader)(Mesh&, const std::string&)) {
		std::stringstream string_stream;
		string_stream << std::ifstream(file).rdbuf();

		loader(mesh, string_stream.str());
	}

	/* */
	void loadPly (Mesh& mesh, const std::string& ply) {
		std::stringstream stream;
		stream << ply;

		std::string line;

		// Parse the .ply header
		bool finished_parsing_header = false;
		while (!finished_parsing_header && std::getline(stream, line)) {
			auto tokens = string_split(line, ' ');

			// Lookout for the element definition
			if (tokens[0] == "element") {
				if (tokens[1] == "vertex") {
					mesh.vertex_count = std::stoi(tokens[2]);
				} 
				else if (tokens[1] == "face") {
					mesh.face_count = std::stoi(tokens[2]);
				}
				continue;
			}

			// Lookout for the end of the header
			if (tokens[0] == "end_header") {
				finished_parsing_header = true;
				continue;
			}
		}

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		// Parse vertex data
		uint32_t vertex_index = 0;
		while (vertex_index < mesh.vertex_count && std::getline(stream, line)) {
			auto tokens = string_split(line, ' ');

			std::vector<float> values;
			// Parse values
			for (int i = 0; i < tokens.size(); i++) {
				values.push_back(std::stof(tokens[i]));
			}

			Vertex vertex({ values[0], values[1], values[2] }, { values[3], values[4], values[5] }, { values[6], values[7] });
			vertices.push_back(vertex);

			vertex_index++;
		}

		// Parse face data
		uint32_t face_index = 0;
		while (std::getline(stream, line)) {
			auto tokens = string_split(line, ' ');

			std::vector<uint32_t> values;
			// Parse values
			for (int i = 0; i < tokens.size(); i++) {
				values.push_back(std::stoul(tokens[i]));
			}

			// Parse the faces
			// Switch on vertex count
			switch (values[0]) {
			case 3:
				// TODO: Implement parsing triangles
				break;
			case 4:
				// TODO: Benchmark how much this reserve helps, if at all
				indices.reserve(indices.size() + 6);

				indices.push_back(values[1]);
				indices.push_back(values[2]);
				indices.push_back(values[3]);
				indices.push_back(values[1]);
				indices.push_back(values[3]);
				indices.push_back(values[4]);
				break;
			}
			
			face_index++;
		}

		mesh.initRenderData(vertices, indices);
		return;
	}

	/* */

}