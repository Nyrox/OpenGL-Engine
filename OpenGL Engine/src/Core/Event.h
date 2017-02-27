#pragma once
#include <glm\glm.hpp>

class Camera;

class Event {
public:
	struct ClickEvent {
		float x;
		float y;

		Camera* camera;
	};

	struct MouseEvent {
		float x;
		float y;

		Camera* camera;
	};

	enum EventType {
		Click, MouseMove, MouseUp, MouseDown
	};
	EventType type;

	union {
		ClickEvent click;
		MouseEvent mouse;
	};
};