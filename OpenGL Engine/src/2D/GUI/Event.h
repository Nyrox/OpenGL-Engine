#pragma once
#include <glm\glm.hpp>

class Event {
public:
	struct ClickEvent {
		float x;
		float y;
	};

	struct MouseEvent {
		float x;
		float y;
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