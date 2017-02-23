#pragma once

class RenderNode {
public:
	virtual class Model* getModel() = 0;
	class Renderer* renderer;
};