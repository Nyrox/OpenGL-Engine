#pragma once
#include <glm\glm.hpp>

class Event {
public:
	struct ClickEvent {
		float x;
		float y;
	};

	enum EventType {
		Click
	};
	EventType type;

	union {
		ClickEvent click;
	};
};