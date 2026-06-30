module;

#include <functional>
#include <algorithm>
#include <ranges>
#include <string>

module disxx.ui.TextInput;

import disxx.ui.backend.GLUTContext;
import disxx.ui.backend.GLRenderer;
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
	}

	void TextInput::HandleMouse(int button, int state, int x, int y)
	{
		#ifdef BACKEND_CTX_GLUT
			y = backend::GLUTContext::GetWindowSize().y - y;
		#endif

		if (button == 0 && state == 0)
			if (x >= this->m_Position.x && x <= this->m_Position.x + this->m_Size.x && y >= this->m_Position.y && y <= this->m_Position.y + this->m_Size.y)
				this->m_IsClicked = true;
	}

	void TextInput::Render(void) const noexcept
	{
		utility::Shape frame{};
		frame.Replace(utility::Vec2<float>{this->m_Position.x, this->m_Position.y});
		frame.Resize(utility::Vec2<float>{this->m_Size.x, this->m_Size.y});
		frame.SetColor(utility::Vec3<float>{this->m_pColor[0], this->m_pColor[1], this->m_pColor[2]});
		s_pRenderer->PushShape(std::move(frame));

		unsigned long int start{0};
		if (!this->m_Text.empty())
		{
			utility::Text txt{};
			txt.Replace(utility::Vec2<float>{this->m_Position.x + 10.f, this->m_Position.y + 25.f});
			txt.SetColor(utility::Vec3<float>{1.f, 1.f, 1.f});
			if ((this->m_Text.size() * 9) > this->m_Size.x - 20.f)
			{
				start = this->m_Text.size();
				int currentWidth{0};
				for (auto i{this->m_Text.size()}; i-- > 0;)
				{
					if (currentWidth + 9 > this->m_Size.x - 20.f)
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
