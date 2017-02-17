#include "MeshLoaders.h"
#include <Core\Mesh.h>

#include <sstream>
#include <fstream>
#include <vector>
#include <array>

namespace MeshLoaders {

	glm::vec3 calculateTangent(const Vertex& vx, const Vertex& vy, const Vertex& vz) {
		// Calculate edges
		glm::vec3 edge1 = vy.position - vx.position;
		glm::vec3 edge2 = vz.position - vx.position;

		// Calculate delta UV
		glm::vec2 deltaUV1 = vy.uv - vx.uv;
		glm::vec2 deltaUV2 = vz.uv - vx.uv;

		// Some pythagoras shit dunno
		float f = 1.f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		glm::vec3 tangent;
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = glm::normalize(tangent);

		return tangent;
	}

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

			Vertex vertex { glm::vec3(values[0], values[1], values[2]), // Position
							glm::vec3(values[3], values[4], values[5]), // Normal
							glm::vec2(values[6], values[7]) };			// UV

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
			glm::vec3 face;
			switch (values[0]) {
			case 3:
			{
				/* Calculate the tangents now that we have all the data we need */
				glm::uvec3 face(values[1], values[2], values[3]);
				Vertex& vx = vertices.at(face.x);
				Vertex& vy = vertices.at(face.y);
				Vertex& vz = vertices.at(face.z);

				glm::vec3 tangent = calculateTangent(vx, vy, vz);

				vx.tangent = tangent;
				vy.tangent = tangent;
				vz.tangent = tangent;

				indices.insert(indices.end(), { face.x, face.y, face.z });
				break;
			}
			case 4:
				std::array<uint32_t, 6> face = { values[1], values[2], values[3], values[1], values[3], values[4] };
				std::array<Vertex*, 6> vs = { &vertices[face[0]], &vertices[face[1]], &vertices[face[2]], &vertices[face[3]], &vertices[face[4]], &vertices[face[5]] };

				// calculate tangents for face 1
				glm::vec3 tangent = calculateTangent(*vs[0], *vs[1], *vs[2]);
				for (int i = 0; i < 3; i++) vs[i]->tangent = tangent;

				// calculate tangents for face 2
				tangent = calculateTangent(*vs[3], *vs[4], *vs[5]);
				for (int i = 3; i < 6; i++) vs[i]->tangent = tangent;

				// add indices
				for (auto& it : face)
					indices.insert(indices.end(), it);
				break;
			}
			
			face_index++;
		}

		mesh.initRenderData(vertices, indices);
		return;
	}

	/* */

}