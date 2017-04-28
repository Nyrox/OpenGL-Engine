#include <Projects/GUI/guimain.h>

#include <gl_core_4_3.hpp>
#include <Core/Window.h>
#include <glm/glm.hpp>

#include <vector>
#include <2D/RectangleShape.h>
#include <Core/Shader.h>
#include <glm/gtx/transform.hpp>
#include <2D/Text.h>
#include <algorithm>
#include <Math/Rect.h>


namespace GUI {
	
	class Element {
	public:

		virtual void layout() = 0;
		virtual void render(Shader& shader) = 0;


		template<class T, class... Args>
		T& emplace(Args&&... args) {
			children.emplace_back<T>(args...);
			children.back()->parent = this;
			return *children.back();
		}

	protected:
		Element* parent = nullptr;
		std::vector<std::unique_ptr<Element>> children;
	};

	class Canvas : public Element {
	public:
		
		virtual void layout() override {
			for (auto& it : children) {
				it->layout();
			}
		}

		virtual void render(Shader& shader) override {
			shape.setSize(size);
			shape.transform = Transform(glm::vec3(position.x, position.y, 0.f));
			shape.fillColor = glm::vec4(fillColor, 1);

			shape.render(shader);
		}

		RectangleShape shape = RectangleShape({});

		glm::vec3 fillColor;
		glm::vec2 position;
		glm::vec2 size;
	};

	class FluidContainer : public Element {
	public:
		
		virtual void layout() override {
			for (auto& it : children) {
				it->layout();
			}
		}
		
		virtual void render(Shader& shader) override {
			shape.fillColor = glm::vec4(fillColor, 1.0);


		}

		RectangleShape shape = RectangleShape({});
		
		glm::vec3 fillColor;
		
	};



	void _main() {		
		Window window(1280, 720, "GUI Test Project");

		Shader shader("assets/shaders/gui/element.vert", "assets/shaders/gui/element.frag");


		Canvas inspector;
		inspector.position = { 1280 - 380, 720 };
		inspector.size = { 380, 720 };
		inspector.fillColor = glm::vec3(0.2, 0.2, 0.2);

		auto& transform = inspector.emplace<FluidContainer>();
		transform.fillColor = glm::vec3(0.3, 0.3, 0.3);

		while (window.isOpen()) {
			window.pollEvents();

			gl::ClearColor(0.05, 0.05, 0.05, 1.0);
			gl::Clear(gl::COLOR_BUFFER_BIT);

			inspector.render();

			window.display();
		}

		window.close();
	}

}