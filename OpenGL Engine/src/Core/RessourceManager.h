#pragma once
#include <Core/Texture.h>
#include <unordered_map>

#include <Core\Texture.h>


class RessourceManager {
public:
	static Texture& loadTexture(const std::string& id, const std::string& file, GLenum format);
	static Texture& getTexture(const std::string& id);


	template<class T, class... Args> 
	static std::shared_ptr<T> create(const std::string& id, Args&&... args);


	std::unordered_map<std::string, std::shared_ptr<Refactor::Texture2D>> textures_2d;

	std::unordered_map<std::string, Texture> textures;

private:
	RessourceManager() { }
	static RessourceManager& getInstance() {
		static RessourceManager instance;
		return instance;
	}
};


template<class T, class... Args>
static std::shared_ptr<T> RessourceManager::create<Refactor::Texture2D>(const std::string& id, Args&&... args) {
	auto texture = std::make_shared<Refactor::Texture2D>(args...);
	getInstance().textures_2d.insert(std::make_pair(id, texture));
	return texture;
}