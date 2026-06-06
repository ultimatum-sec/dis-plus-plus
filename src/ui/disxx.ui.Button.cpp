module;

#ifdef __APPLE__
#	include <OpenGL/gl.h>
#	include <GLUT/glut.h>
#else
#	include <GL/freeglut.h>
#endif

#include <filesystem>
#include <functional>
#include <string>
#include <tuple>

module disxx.ui.Button;

import disxx.ui.backend.GLRenderer;
import disxx.ui.utility.Shape;

namespace disxx::ui
{
	Button::Button(void) noexcept
		: Widget{}
		, m_Trigger{ButtonTrigger::BTN_NONE}
		, m_Image{}
		, m_Text{}	
	{}

	Button::Button(float x, float y, float width, float height) noexcept
		: Widget{x, y, width, height}
		, m_Trigger{ButtonTrigger::BTN_NONE}
		, m_Image{}
		, m_Text{}
	{}

	Button::Button(const Button &other) noexcept
		: Widget{other}
		, m_Trigger{other.m_Trigger}
		, m_Image{other.m_Image}
		, m_Text{other.m_Text}
	{}

	Button &Button::operator=(const Button &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			Widget::operator=(other);
			this->m_Trigger = other.m_Trigger;
			this->m_Image = other.m_Image;
			this->m_Text = other.m_Text;
		}

		return *this;
	}

	void Button::SetCallback(const ButtonTrigger trigger, const std::function<void(const Widget *const)> callback) noexcept
	{
		if (trigger != ButtonTrigger::BTN_NONE)
		{
			this->m_Callback = callback;
			this->m_Trigger = trigger;
		}
		else
		{
			// Don't touch the callback here!
			// It won't be used anyway...
			this->m_Trigger = ButtonTrigger::BTN_NONE;
		}
	}

	void Button::SetImage(const std::filesystem::path &rPath) noexcept(false)
	{
		utility::ImageLoader loader{};
		this->m_Image = loader.Load(rPath);
	}
	
	void Button::SetText(std::string str) noexcept(false)
	{ this->m_Text = str; }

	void Button::Render(void) const noexcept
	{
		// Render the frame
		utility::Shape s{utility::Shape::Type::RECTANGLE};
		s.Replace(this->m_X, this->m_Y);
		s.Resize(this->m_Width, this->m_Height);
		s.SetColor(this->m_pColor[0], this->m_pColor[1], this->m_pColor[2]);

		this->m_Renderer.ClearShapes();
		this->m_Renderer.AddShape(std::move(s));
		
		this->m_Renderer.Render();
	}

	void Button::HandleMouse(int button, int state, int x, int y) noexcept
	{
		//y = glutGet(GLUT_WINDOW_HEIGHT) - y;
		if (!(x >= this->m_X && x <= this->m_X + this->m_Width && y >= this->m_Y && y <= this->m_Y + this->m_Height))
			return;

		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
			this->m_IsClicked = true;
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
			this->m_IsClicked = false;

		if (this->m_IsClicked && this->m_Trigger == ButtonTrigger::BTN_CLICKED)
			(*this)();
		glutPostRedisplay();
	}

	void Button::HandleMotion(int x, int y) noexcept
	{
		//y = glutGet(GLUT_WINDOW_HEIGHT) - y;
		this->m_IsHovered = (x >= this->m_X && x <= this->m_X + this->m_Width && y >= this->m_Y && y <= this->m_Y + this->m_Height);
		if (this->m_IsHovered && this->m_Trigger == ButtonTrigger::BTN_HOVERED)
			(*this)();
		glutPostRedisplay();
	}
} /* disxx::ui */
