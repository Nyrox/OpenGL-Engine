#pragma once
#include <Core/Shader.h>
#include <2D/GUI/Widget.h>
#include <2D/GUI/Button.h>

#include <vector>
#include <memory>


class GUIContext {
public:
	GUIContext(uint32_t width, uint32_t height);

	void render();
	
	template<class T, class... Args>
	T* createWidget(Args&&... args);


	void handleEvent(Event event);

	std::vector<std::unique_ptr<Widget>> widgets;
	
	Shader shader;
	uint32_t contextWidth, contextHeight;
};


template<class T, class... Args>
T* GUIContext::createWidget(Args&&... args) {
	widgets.push_back(std::make_unique<T>(args...));
	return static_cast<T*>(widgets.back().get());
};
