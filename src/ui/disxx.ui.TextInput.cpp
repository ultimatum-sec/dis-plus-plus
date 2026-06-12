module;

#ifdef __APPLE__
#	include <OpenGL/gl.h>
#	include <GLUT/glut.h>
#else
#	include <GL/freeglut.h>
#	include <GL/gl.h>
#endif

#include <functional>
#include <algorithm>
#include <ranges>
#include <string>

module disxx.ui.TextInput;

import disxx.ui.utility.Shape;
import disxx.ui.utility.Text;
import disxx.ui.utility.Vec;

namespace disxx::ui
{
	TextInput::TextInput(void) noexcept
		: Widget{}
		, m_Callback{[](const Widget *const) -> void {}}
		, m_Text{""}
	{}

	TextInput::TextInput(float x, float y, float width, float height) noexcept
		: Widget{x, y, width, height}
		, m_Callback{[](const Widget *const) -> void {}}
		, m_Text{""}
	{}

	TextInput::TextInput(const TextInput &other) noexcept
		: Widget{other}
		, m_Callback{other.m_Callback}
		, m_Text{other.m_Text}
	{}

	TextInput &TextInput::operator=(const TextInput &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			Widget::operator=(other);
			this->m_Callback = other.m_Callback;
			this->m_Text = other.m_Text;
		}

		return *this;
	}

	void TextInput::SetText(std::string_view text) noexcept
	{ this->m_Text = text; }

	std::string_view TextInput::GetText(void) const noexcept
	{ return this->m_Text; }

	void TextInput::HandleKeyboard(unsigned char key, [[maybe_unused]] int x, [[maybe_unused]] int y)
	{
		if (this->m_IsClicked)
		{
			if (key == 0x7F && !this->m_Text.empty())
				this->m_Text.pop_back();
			else if (key == '\n')
				this->m_Callback(this);
			else if (key >= 32 && key <= 126)
				this->m_Text += key;
		}

		glutPostRedisplay();
	}

	void TextInput::HandleMouse(int button, int state, int x, int y)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if (x >= this->m_X && x <= this->m_X + this->m_Width && y >= this->m_Y && y <= this->m_Y + this->m_Height)
				this->m_IsClicked = true;
		}
	}

	void TextInput::Render(void) const noexcept
	{
		utility::Shape frame{};
		frame.Replace(utility::Vec2<float>{this->m_X, this->m_Y});
		frame.Resize(utility::Vec2<float>{this->m_Width, this->m_Height});
		frame.SetColor(utility::Vec3<float>{this->m_pColor[0], this->m_pColor[1], this->m_pColor[2]});
		s_pRenderer->PushShape(std::move(frame));

		unsigned long int start{0};
		if (!this->m_Text.empty())
		{
			utility::Text txt{};
			txt.Replace(utility::Vec2<float>{this->m_X + 10.f, this->m_Y + 25.f});
			txt.SetColor(utility::Vec3<float>{1.f, 1.f, 1.f});
			if ((this->m_Text.size() * 9) > this->m_Width - 20.f)
			{
				start = this->m_Text.size();
				int currentWidth{0};
				for (auto i{this->m_Text.size()}; i-- > 0;)
				{
					if (currentWidth + 9 > this->m_Width - 20.f)
					{
						start = i + 1;
						break;
					}

					currentWidth += 9;
				}
			}
			txt.SetText(this->m_Text.substr(start));
			s_pRenderer->PushText(std::move(txt));
		}

		s_pRenderer->Render();
	}
} /* disxx::ui */
