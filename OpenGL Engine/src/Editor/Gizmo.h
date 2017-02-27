#pragma once
#include <Core\Mesh.h>
#include <Core\SceneNode.h>
#include <Core\Shader.h>

class Gizmo {
public:
	Gizmo();

	void render() const;

	SceneNode* sceneNode;
private:
	Shader shader;
	Mesh translationGizmo;

};