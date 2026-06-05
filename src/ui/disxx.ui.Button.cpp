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
		/*
		glColor3f(0.f, 0.f, 0.f);
		glBegin(GL_QUADS);
			glVertex2f(this->m_X + 1.f, this->m_Y + 1.f);
			glVertex2f(this->m_X + 1.f + this->m_Width, this->m_Y + 1.f);
			glVertex2f(this->m_X + this->m_Width + 1.f, this->m_Y + this->m_Height + 1.f);
           	glVertex2f(this->m_X + 1.f, this->m_Y + this->m_Height + 1.f);
		glEnd();
	
		glBegin(GL_QUADS);
			glVertex2f(this->m_X - 1.f, this->m_Y - 1.f);
			glVertex2f(this->m_X - 1.f + this->m_Width, this->m_Y - 1.f);
			glVertex2f(this->m_X + this->m_Width - 1.f, this->m_Y + this->m_Height - 1.f);
           	glVertex2f(this->m_X - 1.f, this->m_Y + this->m_Height - 1.f);
		glEnd();

		// Render the button itself
		if (this->m_IsClicked)
			glColor3f(this->m_pColor[0] + 0.2f, this->m_pColor[1] + 0.2f, this->m_pColor[2] + 0.2f);
		else
			glColor3f(this->m_pColor[0], this->m_pColor[1], this->m_pColor[2]);
    	glBegin(GL_QUADS);
    	    glVertex2f(this->m_X, this->m_Y);
   	    	glVertex2f(this->m_X + this->m_Width, this->m_Y);
   	       	glVertex2f(this->m_X + this->m_Width, this->m_Y + this->m_Height);
           	glVertex2f(this->m_X, this->m_Y + this->m_Height);
       	glEnd();
		*/

		utility::Shape s{utility::Shape::Type::RECTANGLE};
		s.Replace(this->m_X, this->m_Y);
		s.Resize(this->m_Width, this->m_Height);
		s.SetColor(1.f, 1.f, 1.f);

		this->m_Renderer.AddShape(std::move(s));
		
		/*// Render the text
        if (!this->m_Text.empty())
        {
            float textWidth{0.0f};
            for (const auto &ch : this->m_Text)
				textWidth += glutStrokeWidth(GLUT_STROKE_MONO_ROMAN, ch);

            float scaleFactor{0.1f};
            float scaledWidth{textWidth * scaleFactor};

            float textX{this->m_X + (this->m_Width - scaledWidth) / 2.0f};  
            float textY{this->m_Y + this->m_Height / 2.0f + 10.0f};

            glPushMatrix();
                glTranslatef(textX, textY, 0.0f);
                glScalef(scaleFactor, -scaleFactor, 1.0f);
                
				glColor3f(1.f, 1.f, 1.f);
                for (const auto &ch : this->m_Text)
					glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, ch);
			glPopMatrix();
        }*/

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
