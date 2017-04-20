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

namespace GUI {
	constexpr float AUTO = 0;

	class Element;
	class GUISurface;
	void renderElement(const Element&, Shader&, GUISurface* surface);

	
	struct Style {
		struct {
			float top = 0, right = 0, bottom = 0, left = 0;
		} padding;

		
		struct UnitInfo {
			enum class Unit {
				PIXEL, PERCENT
			} w = Unit::PIXEL, h = Unit::PIXEL;
		} unitInfo;

		struct Position {
			enum {
				Auto, Absolute
			} x = Auto, y = Auto;
		} position;
		

		glm::vec3 textColor;
		glm::vec3 fillColor;
		float top = 0, left = 0;
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

		void reflow(glm::vec2 position) {
			if (!this->parent) {
				goto END;
			}

			processed.top = style.position.y == Style::Position::Absolute ? style.top : position.y;
			position.x = style.position.x == Style::Position::Absolute ? style.left : position.x;
			processed.left = position.x;

			// Calculate own widths
			float availableWidth = parent->processed.size.x - parent->processed.padding.left - parent->processed.padding.right;
			availableWidth = std::max(availableWidth, 0.f);
			float desiredWidth = style.unitInfo.w == Style::UnitInfo::Unit::PERCENT ? availableWidth / 100 * style.size.x : style.size.x;
			processed.size.x = std::clamp(desiredWidth, 0.f, availableWidth);

			END:
			float lowestY = INFINITY;
			float highestY = 0;
			
			// Let children calculate their widths and heights
			for (auto& it : children) {
				it.reflow({ position.x + style.padding.left, position.y + style.padding.top });
				if (it.processed.top < lowestY) { lowestY = it.processed.top; }
				if (it.processed.top + it.processed.size.y) { highestY = it.processed.top + it.processed.size.y; }
			}

			processed.size.y = style.padding.top + style.padding.bottom + std::max(14.f, highestY - lowestY);
			
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

			for (auto& it : rootNode.getChildren()) {
				renderElement(it, elementShader, this);
			}
		}

		void reflow() {
			rootNode.processed.size = size;
			rootNode.reflow({ 0, 0 });
		}
	
		glm::vec2 size;
		Font font;

		Shader elementShader;
		Element rootNode;
	};

	void renderElement(const Element& element, Shader& shader, GUISurface* surface) {
		const Style& style = element.processed;
		RectangleShape shape(style.size, { style.left, style.top });
		shape.fillColor = style.fillColor;
		shape.transform = Transform({ style.left, style.top, 0 });

		shape.render(shader);

		if (!element.text.empty() && element.text != "") {
			Text text(&surface->font, element.text);
			text.position = { style.left + style.padding.left, style.top + style.padding.top };

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
		inspectorStyle.position.x = Style::Position::Absolute;
		inspectorStyle.left = 1280 - 400;
		inspectorStyle.fillColor = { 0.25 };
		inspectorStyle.padding = { 16, 32, 32, 32 };
		Element inspector(inspectorStyle);

		Style testStyle;
		testStyle.size = { 100, 150 };
		testStyle.unitInfo.w = Style::UnitInfo::Unit::PERCENT;
		testStyle.fillColor = { 0.5, 0.4, 0.4 };
		testStyle.padding = { 8, 8, 8, 8 };

		Element test(testStyle);
		test.text = "Transform";
		inspector.addChild(test);
		surface.rootNode.addChild(inspector);
		
		Style pes;
		pes.size = { 600, 140 };
		pes.top = 720 - 140;
		pes.position.y = Style::Position::Absolute;
		pes.fillColor = { 0.35, 0.7, 0.35 };

		Element projectExplorer(pes);
		projectExplorer.text = "Project Explorer";
		surface.rootNode.addChild(projectExplorer);


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