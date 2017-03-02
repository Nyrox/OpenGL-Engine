#pragma once
#include <Core/Texture2D.h>
#include <unordered_map>

class Shader;


struct Material {
	enum class ShadingModel {
		Opague, Transparent
	} const shadingModel;


	Material(Shader& t_shader, ShadingModel t_shadingModel = ShadingModel::Opague, float t_shininess = 64) : shader(t_shader), shadingModel(t_shadingModel), shininess(t_shininess) {

	}

	Shader& shader;

	std::unordered_map<std::string, Texture2D*> textures;

	Texture2D*& operator[](const std::string& id) {
		return textures[id];
	}


	float shininess;
};