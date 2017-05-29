#pragma once
#include <Core/Definitions.h>
#include <unordered_map>
#include <Core/Libs/colony.h>
#include <filesystem>
#include <Core/Mesh.h>
#include <Core/Material.h>
#include <Core/JSON/json.h>
class Component;
#include <Core/ECS/GameObject.h>
#include <Core/Texture2D.h>

#include <Core/ECS/Components/MeshRenderer.h>

namespace std {
	namespace filesystem = std::experimental::filesystem;
}

class Engine {	
public:
	Engine() {
		components[typeid(PointLight).hash_code()] = {};
		components[typeid(MeshRenderer).hash_code()] = {};
	}


	friend class GameObject;

	void loadProject(std::filesystem::path path);
	void loadScene(std::filesystem::path path);

	
	std::filesystem::path getProjectBasePath() { return projectBasePath; }
	GameObject& createGameObject() { gameObjects.emplace_back(*this); return gameObjects.back(); }
	
	template<class T, class... Args>
	T* emplaceComponent(Args&&... args) {
		return (T*)components[typeid(T).hash_code()].insert(std::make_unique<T>(args...))->get();
	}

	template<class T>
	plf::colony<uptr<Component>>& getAllComponentsOfType() {
		return components[typeid(T).hash_code()];
	}
	
	std::unordered_map<std::string, Mesh> meshes;
	std::unordered_map<std::string, Material> materials;

	TextureBase* getTexture(std::size_t i) const { return textures[i].get(); }
	uint32 getTextureIndexFromName(std::string id) const { return texture_hash_table.at(id); }
private:
	std::vector<uptr<TextureBase>> textures;
	std::unordered_map<std::string, uint32> texture_hash_table;
	
	void loadTexture2D(const Json::Value value);
	void loadMaterial(const Json::Value value);
	void loadMesh(const Json::Value value);

	MaterialInstance loadMaterialInstance(const Json::Value value);

	std::vector<GameObject> gameObjects;
	std::unordered_map<uint32, plf::colony<uptr<Component>>> components;



	std::filesystem::path projectBasePath;
};