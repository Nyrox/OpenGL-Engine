#include "Engine.h"
#include <Core/Util/FileUtil.h>
#include <Core/ECS/Components/MeshRenderer.h>
#include <Core/ECS/Component.h>
#include <Core/ECS/GameObject.h>

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

	textures.emplace(value["id"].asString(), std::make_unique<Texture2D>((projectBasePath / value["path"].asString()).string(), value["format"].asUInt(), settings));
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

	Material material;

	for (auto& sampler : root["samplers"]) {
		material.textures.emplace(sampler["id"].asString(), (Texture2D*)textures[sampler["resource"].asString()].get());
	}

	materials.emplace(value["id"].asString(), material);
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
				go.addComponent<MeshRenderer>(go, &meshes[component["mesh"].asString()], &materials[component["material"].asString()]);
			}
		}
	}
}