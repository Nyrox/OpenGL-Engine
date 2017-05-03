#pragma once
#include <Core/Texture2D.h>
#include <unordered_map>

class Shader;


struct Material {
	enum class ShadingModel {
		PBR, Transparent
	} const shadingModel;


	Material() : shadingModel(ShadingModel::PBR) { }
	Material(ShadingModel t_shadingModel) : shadingModel(t_shadingModel) { }
	Material(const Material& other) = default;
	Material& operator=(const Material& other) = default;

	std::unordered_map<std::string, Texture2D*> textures;

	Texture2D*& operator[](const std::string& id) {
		return textures[id];
	}

	float uvScale = 1.0;
	
};