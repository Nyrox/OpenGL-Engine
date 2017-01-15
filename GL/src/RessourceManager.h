#pragma once
#include <Texture.h>
#include <unordered_map>

class RessourceManager {
public:
	static Texture& loadTexture(const std::string& id, const std::string& file, GLenum format);

	std::unordered_map<std::string, Texture> textures;

private:
	RessourceManager() { }
	static RessourceManager& getInstance() {
		static RessourceManager instance;
		return instance;
	}
};

