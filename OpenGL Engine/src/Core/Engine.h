#pragma once
#include <Core/Definitions.h>
#include <unordered_map>
#include <Core/Libs/colony.h>
#include <filesystem>
#include <Core/Mesh.h>
#include <Core/Material.h>
#include <Core/JSON/json.h>
class Component;

namespace std {
	namespace filesystem = std::experimental::filesystem;
}

class Engine {	
public:
	friend class GameObject;

	void loadProject(std::filesystem::path path);
	void loadScene(std::filesystem::path path);

	
	
	
	
	template<class T, class... Args>
	T* emplaceComponent(Args&&... args) {
		return (T*)components[typeid(T).hash_code()].insert(std::make_unique<T>(args...))->get();
	}

	template<class T>
	plf::colony<uptr<Component>>& getAllComponentsOfType() {
		return components[typeid(T).hash_code()];
	}
	
	std::unordered_map<std::string, uptr<TextureBase>> textures;
	std::unordered_map<std::string, Mesh> meshes;
	std::unordered_map<std::string, Material> materials;
private:
	void loadTexture2D(const Json::Value value);
	void loadMaterial(const Json::Value value);
	void loadMesh(const Json::Value value);

	std::vector<GameObject> gameObjects;
	std::unordered_map<uint32, plf::colony<uptr<Component>>> components;



	std::filesystem::path projectBasePath;
};