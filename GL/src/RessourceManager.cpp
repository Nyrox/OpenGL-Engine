#include "RessourceManager.h"

Texture& RessourceManager::loadTexture(const std::string& id, const std::string& file, GLenum format) {
	RessourceManager& _this = RessourceManager::getInstance();

	_this.textures.emplace(std::make_pair(id, Texture()));
	_this.textures[id].loadFromFile(file, format);
	return _this.textures[id];
}