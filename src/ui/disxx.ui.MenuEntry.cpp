module;

#include <utility>
#include <memory>

module disxx.ui.MenuEntry;

import disxx.ui.backend.GLUTContext;
import disxx.ui.utility.Shape;
import disxx.ui.utility.Text;

namespace disxx::ui
{
	MenuEntry::MenuEntry(void) noexcept
		: Widget{}
		, m_Callback{}
		, m_Text{}
	{}

	MenuEntry::MenuEntry(std::string_view str, std::function<void(void)> f) noexcept
		: Widget{}
		, m_Callback{f}
		, m_Text{str}
	{}

	MenuEntry::MenuEntry(const MenuEntry &other) noexcept
		: Widget{other}
		, m_Callback{other.m_Callback}
		, m_Text{other.m_Text}
	{}

	MenuEntry &MenuEntry::operator=(const MenuEntry &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			Widget::operator=(other);
			this->m_Callback = other.m_Callback;
			this->m_Text = other.m_Text;
		}

		return *this;
	}

	MenuEntry::MenuEntry(MenuEntry &&other) noexcept
		: Widget{std::forward<MenuEntry &&>(other)}
		, m_Callback{std::move(other.m_Callback)}
		, m_Text{std::move(other.m_Text)}
	{}

	MenuEntry &MenuEntry::operator=(MenuEntry &&other) noexcept
	{
		Widget::operator=(std::forward<MenuEntry &&>(other));
		this->m_Callback = std::move(other.m_Callback);
		this->m_Text = std::move(other.m_Text);

		return *this;
	}

	void MenuEntry::HandleMouse(int button, int state, int x, int y) noexcept
	{
		//#ifdef BACKEND_CTX_GLUT
		//	y = backend::GLUTContext::GetWindowSize().y - y;
		//#endif

		if (!(x >= this->m_Position.x && x <= this->m_Position.x + this->m_Size.x && y >= this->m_Position.y && y <= this->m_Position.y + this->m_Size.y))
			return;

		if (button == 0 && state == 0)
			this->m_IsClicked = true;
		else if (button == 0 && state == 1)
			this->m_IsClicked = false;
	}

	void MenuEntry::Render(void) const noexcept
	{
		// Add a button
		utility::Shape btn{utility::Shape::Type::TYPE_RECTANGLE};
		btn.Replace(utility::Vec2<float>{this->m_Position.x, this->m_Position.y});
		btn.Resize(utility::Vec2<float>{this->m_Size.x, this->m_Size.y});
		btn.SetColor(utility::Vec3<float>{this->m_pColor[0], this->m_pColor[1], this->m_pColor[2]});
		s_pRenderer->Push(std::make_unique<utility::Shape>(btn));
		
		// Add a text
        if (!this->m_Text.empty())
        {
			utility::Text txt{};
			txt.Replace
			(
            	utility::Vec2<float>
				{
					this->m_Position.x + (this->m_Size.x - (9.f * this->m_Text.size())) / 2.0f, 
            		this->m_Position.y + this->m_Size.y / 3.0f - 4.5f
				}
			);
			txt.SetColor(utility::Vec3<float>{1.f, 1.f, 1.f});
			txt.SetText(this->m_Text);
			s_pRenderer->Push(std::make_unique<utility::Text>(txt));
        }

		s_pRenderer->Render();
	}
} /* disxx::ui::utility */
