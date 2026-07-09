module;

#include <string_view>
#include <algorithm>
#include <memory>
#include <ranges>
#include <vector>
#include <string>

module disxx.ui.Menu;

import disxx.ui.backend.GLUTContext;
import disxx.ui.backend.GLRenderer;
import disxx.ui.utility.Shape;
import disxx.ui.utility.Text;

namespace disxx::ui
{
	Menu::Menu(void) noexcept
		: Widget{}
		, m_Entries{}
		, m_Text{}
	{}

	Menu::Menu(float x, float y, float width, float height) noexcept
		: Widget{x, y, width, height}
		, m_Entries{}
		, m_Text{}
	{}

	Menu::Menu(const Menu &other) noexcept
		: Widget{other}
		, m_Entries{other.m_Entries}
		, m_Text{other.m_Text}
	{}

	Menu &Menu::operator=(const Menu &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			Widget::operator=(other);
			this->m_Entries = other.m_Entries;
			this->m_Text = other.m_Text;
		}

		return *this;
	}

	Menu::Menu(Menu &&other) noexcept
		: Widget{std::forward<Menu &&>(other)}
		, m_Entries{std::move(other.m_Entries)}
		, m_Text{std::move(other.m_Text)}
	{}

	Menu &Menu::operator=(Menu &&other) noexcept
	{
		Widget::operator=(std::forward<Menu &&>(other));
		this->m_Entries = std::move(other.m_Entries);
		this->m_Text = std::move(other.m_Text);

		return *this;
	}

	std::unique_ptr<Widget> Menu::Clone(void) const noexcept
	{ return std::make_unique<std::decay<decltype(*this)>::type>(*this); }

	void Menu::HandleMouse(int button, int state, int x, int y) noexcept
	{
		if (this->m_IsClicked)
		{
			for (auto &entry : this->m_Entries)
			{
				entry.HandleMouse(button, state, x, y);
				if (entry.Clicked())
				{
					entry();
					return;
				}
			}
		}
		else if (!(x >= this->m_Position.x && x <= this->m_Position.x + this->m_Size.x && y >= this->m_Position.y && y <= this->m_Position.y + this->m_Size.y))
			return;

		if (!this->m_IsClicked && button == 0 && state == 0)
			this->m_IsClicked = true;
		else if (this->m_IsClicked && button == 0 && state == 0)
			this->m_IsClicked = false;
	}

	void Menu::Render(void) const noexcept
	{
		// Add a frame
		if (this->m_IsClicked && !this->m_Entries.empty())
		{
			const auto [x, y]{this->m_Entries.rbegin()->GetPosition()};
			const auto [width, height]{this->m_Entries.begin()->GetSize()};
			utility::Shape frame{utility::Shape::Type::TYPE_RECTANGLE};
			frame.Replace(utility::Vec2<float>{x - 1.f, y - 1.f});
			frame.Resize(utility::Vec2<float>{width + 2.f, height * this->m_Entries.size() + 2.f});
			frame.SetColor(utility::Vec3<float>{0.f, 0.f, 0.f});
			s_pRenderer->Push(std::make_unique<utility::Shape>(frame));
		}

		// Add a menu button
		utility::Shape btn{utility::Shape::Type::TYPE_RECTANGLE};
		btn.Replace(utility::Vec2<float>{this->m_Position});
		btn.Resize(utility::Vec2<float>{this->m_Size});
		if (this->m_IsClicked && this->m_pColor[0] <= 0.9f && this->m_pColor[1] <= 0.9f && this->m_pColor[2] <= 0.9f)
			btn.SetColor(utility::Vec3<float>{this->m_pColor[0] + 0.1f, this->m_pColor[1] + 0.1f, this->m_pColor[2] + 0.1f});
		else
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

		// Render all the entries
		if (this->m_IsClicked)
			for (const auto &entry : this->m_Entries)
				entry.Render();
		s_pRenderer->Render();
	}
} /* disxx::ui */
