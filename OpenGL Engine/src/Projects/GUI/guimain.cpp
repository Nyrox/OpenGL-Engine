#include <Projects/GUI/guimain.h>

#include <gl_core_4_3.hpp>
#include <Core/Window.h>
#include <glm/glm.hpp>

#include <vector>
#include <2D/RectangleShape.h>
#include <Core/Shader.h>
#include <glm/gtx/transform.hpp>
#include <2D/Text.h>

namespace GUI {
	constexpr float AUTO = 0;

	class Element;
	class GUISurface;
	void renderElement(const Element&, Shader&, GUISurface* surface);

	
	struct Style {
		struct Padding {
			Padding() = default;
			Padding(float t) : left(t), top(t), right(t), bottom(t) { }
			Padding(float l, float t, float r, float b) : left(l), top(t), right(r), bottom(b) { }

			float left = 0, top = 0, right = 0, bottom = 0;
		} padding;

		
		struct UnitInfo {
			enum class Unit {
				PIXEL, PERCENT
			} w = Unit::PIXEL, h = Unit::PIXEL;
		} unitInfo;

		glm::vec3 textColor;
		glm::vec3 fillColor;
		glm::vec2 position;
		glm::vec2 size;
	};

	class Element {
	public:
		Element(Style t_style = Style()) : style(t_style), processed(t_style) { }

		Element* getParent() { return parent; }
		
		void addChild(Element element) {
			children.push_back(element);
			children.back().parent = this;
		}

		void reflow() {
			if (!this->parent) {
				goto END;
			}

			// Percentage widths
			if (this->style.unitInfo.w == Style::UnitInfo::Unit::PERCENT) this->processed.size.x = this->parent->processed.size.x;
			else this->processed.size.x = this->style.size.x - this->style.padding.left - this->style.padding.right;
			// Percentage heights
			if (this->style.unitInfo.h == Style::UnitInfo::Unit::PERCENT) this->processed.size.y = this->parent->processed.size.y;
			else this->processed.size.y = this->style.size.y - this->style.padding.top - this->style.padding.bottom;

			END:
			for (auto& it : children) {
				it.reflow();
			}
		}

		const std::vector<Element>& getChildren() const { return children; }

		std::string text;		

		Style style;
		Style processed;
	private:
		Element* parent;
		std::vector<Element> children;
	};

	class GUISurface {
	public:
		GUISurface(glm::vec2 t_size) : size(t_size), rootNode() {
			rootNode.style.size = size;

			elementShader.loadFromFile("shaders/gui/element.vert", "shaders/gui/element.frag");
			font.loadFromFile("assets/fonts/arial.ttf", 14);
		}

		void render() {
			elementShader.bind();

			glm::mat4 projection = glm::ortho(0.f, size.x, size.y, 0.0f, -100.f, 100.f);
			elementShader.setUniform("projection", projection);
			
			gl::Disable(gl::CULL_FACE);

			renderElement(rootNode, elementShader, this);
		}

		void reflow() {
			rootNode.reflow();
		}

		glm::vec2 size;

		Font font;

		Shader elementShader;
		Element rootNode;
	};

	void renderElement(const Element& element, Shader& shader, GUISurface* surface) {
		const Style& style = element.processed;
		RectangleShape shape(style.size,style.position);
		shape.fillColor = style.fillColor;
		shape.transform = Transform({ style.position.x, style.position.y, 0 });

		shape.render(shader);

		if (!element.text.empty()) {
			Text text(&surface->font, element.text);
			text.position = style.position;

			text.render();
		}

		for (auto& it : element.getChildren()) {
			renderElement(it, shader, surface);
		}
	}

	void _main() {		
		Window window(1280, 720, "GUI Test Project");

		GUISurface surface({ 1280, 720 });

		Style inspectorStyle;
		inspectorStyle.size = { 400, 720 };
		inspectorStyle.position = { 1280 - 400, 0 };
		inspectorStyle.fillColor = { 0.25 };

		Element inspector(inspectorStyle);

		Style testStyle;
		testStyle.size = { 100, 150 };
		testStyle.unitInfo.w = Style::UnitInfo::Unit::PERCENT;
		testStyle.position = { 1280 - 380, 20 };
		testStyle.fillColor = { 0.4, 0.4, 0.4 };

		Element test(testStyle);
		test.text = "Transform";
		inspector.addChild(test);
		surface.rootNode.addChild(inspector);
		
		surface.reflow();

		while (window.isOpen()) {
			window.pollEvents();

			gl::ClearColor(0.05, 0.05, 0.05, 1.0);
			gl::Clear(gl::COLOR_BUFFER_BIT);

			surface.render();

			window.display();
		}

		window.close();
	}

}