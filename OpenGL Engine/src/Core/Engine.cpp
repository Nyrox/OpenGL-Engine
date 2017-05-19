#include "Engine.h"
#include <Core/Util/FileUtil.h>
#include <Core/ECS/Components/MeshRenderer.h>
#include <Core/ECS/Component.h>
#include <Core/ECS/GameObject.h>
#include <Core/NShaderCompiler/ShaderCompiler.h>

#include <iostream>

void Engine::loadProject(std::filesystem::path path) {
	if (std::filesystem::is_regular_file(path)) {
		path.remove_filename();
	}

	projectBasePath = path;
	

	// Check if the project even exists
	if (!std::filesystem::exists(projectBasePath / "config.json")) {
		std::cout << "Failed to load project: config.json not found at: " << projectBasePath.string() << "\n";
		throw std::runtime_error("Failed to load project: config.json not found: " + projectBasePath.string());
	}

	Json::Value root = FUtil::json_read_file(projectBasePath / "config.json");
	for (auto& it : root["resources"]) {
		auto& rType = it["type"].asString();

		if (rType == "Texture2D") {
			loadTexture2D(it);
		}
		else if (rType == "Material") {
			loadMaterial(it);
		}
		else if (rType == "Mesh") {
			loadMesh(it);
		}
		else {
			std::cout << "Unrecognized resource type: " << rType << "\n";
		}
	}


	loadScene(projectBasePath / root.get("mainScene", "").asString());
}

void Engine::loadTexture2D(const Json::Value value) {
	TextureSettings settings;
	settings.mipmapMode = value.get("mipmapMode", MipmapModes::GenerateMipmaps).asUInt();
	settings.textureWrapMode = value.get("textureWrapMode", TextureWrapModes::Repeat).asUInt();
	settings.filteringMode = value.get("filteringMode", FilteringModes::Bilinear).asUInt();

	texture_hash_table.emplace(value["id"].asString(), textures.size());
	textures.emplace_back(std::make_unique<Texture2D>((projectBasePath / value["path"].asString()).string(), value["format"].asUInt(), settings));
}

void Engine::loadMesh(const Json::Value value) {
	meshes.emplace(value["id"].asString(), Mesh((projectBasePath / value["path"].asString()).string()));
}

void Engine::loadMaterial(const Json::Value value) {
	auto path = projectBasePath / value["path"].asString();

	if (!std::filesystem::exists(path)) {
		std::cout << "Failed to load material; File not found: " << path.string() << "\n";
		throw std::runtime_error("Failed to load Material; File not found: " + path.string());
	}

	Json::Value root = FUtil::json_read_file(path);
	materials.emplace(value["id"].asString(), Material());

	Material& material = materials.at(value["id"].asString());
	if (root["shadingModel"].isUInt()) {
		material.shadingModel = static_cast<ShadingModel>(root["shadingModel"].asUInt());
	}
	else if (root["shadingModel"].isString()) {
		std::unordered_map<std::string, ShadingModel> mappingTable = {
			{ "OPAGUE_PBR", ShadingModel::OPAGUE_PBR_DEFAULT }
		};

		material.shadingModel = mappingTable.at(root["shadingModel"].asString());
	}

	for (auto& it : root["samplers"]) {
		material.samplers.push_back(Sampler(it["binding"].asUInt()));
	}
	
	material.defaultMaterialInstance = loadMaterialInstance(root["defaultMaterialInstance"]);

	// Compile the geometry shader
	if (material.shadingModel == ShadingModel::OPAGUE_PBR_DEFAULT) {
		ShaderCompiler compiler;
		compiler.addIncludeDirectory("shaders/");

		compiler.defineMacro("__MATERIAL__", root["shaderCode"].asString());
		compiler.setVertexShaderSource(FUtil::stringstream_read_file("shaders/deferred/geometry_pass.vert").str());
		compiler.setFragmentShaderSource(FUtil::stringstream_read_file("shaders/deferred/geometry_pass.frag").str());
		material.geometryPassShader = compiler.compile();
	}
	else {
		throw std::runtime_error("Unrecognized shading model: " + std::to_string(static_cast<uint32>(material.shadingModel)));
	}
}

MaterialInstance Engine::loadMaterialInstance(const Json::Value value) {
	MaterialInstance instance(&materials.at(value["material"].asString()));

	for (auto& it : value["samplers"]) {
		instance.samplerLocations.push_back(getTextureIndexFromName(it.asString()));
	}

	for (auto& it : value["scalars"]) {

	}

	return instance;
}

void Engine::loadScene(std::filesystem::path path) {
	if (!std::filesystem::exists(path)) {
		std::cout << "Failed to load scene: file not found: " << path.string() << "\n";
		throw std::runtime_error("Failed to load scene : file not found :" + path.string());
	}

	Json::Value root = FUtil::json_read_file(path);

	for (auto& it : root["entities"]) {
		gameObjects.emplace_back(*this);
		auto& go = gameObjects.back();

		for (auto& component : it["components"]) {
			if (component["type"].asString() == "MeshRenderer") {
				go.addComponent(emplaceComponent<MeshRenderer>(go, &meshes[component["mesh"].asString()], &materials[component["material"].asString()]));
			}
		}
	}
}