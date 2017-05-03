bool rightMouseButtonIsDown = false;

void update() {
	const float cameraSpeed = 15.0f * deltaTime;
	
	if (input.isKeyDown(KEY_W)) {
		camera.transform.position += camera.forwards() * cameraSpeed;
	}
	if (input.isKeyDown(KEY_S)) {
		camera.transform.position -= camera.forwards() * cameraSpeed;
	}
	if (input.isKeyDown(KEY_D)) {
		camera.transform.position += camera.right() * cameraSpeed;
	}
	if (input.isKeyDown(KEY_A)) {
		camera.transform.position -= camera.right() * cameraSpeed;
	}	
	if (input.isKeyDown(KEY_SPACE)) {
		camera.transform.position.y += cameraSpeed;
	}
	if (input.isKeyDown(KEY_SHIFT)) {
		camera.transform.position.y -= cameraSpeed;
	}
	
	
	if(input.isMouseButtonDown(MOUSE_BUTTON_2)) {
		if(rightMouseButtonIsDown == false) rightMouseButtonIsDown = true;
		else {
			Vector2 mousePosition = input.getMousePosition();
			Vector2 lastMousePosition = input.getLastMousePosition();
			camera.yaw += (lastMousePosition.x - mousePosition.x) / -3;
			camera.pitch += (lastMousePosition.y - mousePosition.y) / 4;
		}
	}
	else {
		rightMouseButtonIsDown = false;
	}
	
}