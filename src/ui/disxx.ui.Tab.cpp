module;

#include <utility>
#include <memory>
#include <string>

module disxx.ui.Tab;

import disxx.ui.utility.Shape;
import disxx.ui.utility.Text;

namespace disxx::ui
{
	Tab::Tab(void) noexcept
		: Widget{}
		, m_TextArea{}
		, m_Text{}
	{}

	Tab::Tab(float x, float y, float width, float height) noexcept
		: Widget{x, y, width, height}
		, m_TextArea{}
		, m_Text{}
	{}

	Tab::Tab(const Tab &other) noexcept
		: Widget{other}
		, m_TextArea{other.m_TextArea}
		, m_Text{other.m_Text}
	{}

	Tab &Tab::operator=(const Tab &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			Widget::operator=(other);
			this->m_TextArea = other.m_TextArea;
			this->m_Text = other.m_Text;
		}

		return *this;
	}

	Tab::Tab(Tab &&other) noexcept
		: Widget{std::forward<Tab &&>(other)}
		, m_TextArea{std::move(other.m_TextArea)}
		, m_Text{std::move(other.m_Text)}
	{}

	Tab &Tab::operator=(Tab &&other) noexcept
	{
		Widget::operator=(std::forward<Tab &&>(other));
		this->m_TextArea = std::move(other.m_TextArea);
		this->m_Text = std::move(other.m_Text);
		
		return *this;
	}

	std::unique_ptr<Widget> Tab::Clone(void) const noexcept
	{ return std::make_unique<std::decay<decltype(*this)>::type>(*this); }

	void Tab::Render(void) const noexcept
	{
		float heightModifier{this->m_IsClicked ? 5.f : 0.f};

		// Add a frame
		utility::Shape frame{utility::Shape::Type::TYPE_RECTANGLE};
		frame.Replace(utility::Vec2<float>{this->m_Position.x - 1.f, this->m_Position.y});
		frame.Resize(utility::Vec2<float>{this->m_Size.x + 2.f, this->m_Size.y + 1.f + heightModifier});
		frame.SetColor(utility::Vec3<float>{0.f, 0.f, 0.f});
		s_pRenderer->Push(std::make_unique<utility::Shape>(frame));

		// Add the tab itself
		utility::Shape tab{utility::Shape::Type::TYPE_RECTANGLE};
		tab.Replace(utility::Vec2<float>{this->m_Position.x, this->m_Position.y});
		tab.Resize(utility::Vec2<float>{this->m_Size.x, this->m_Size.y + heightModifier});
		tab.SetColor(utility::Vec3<float>{this->m_pColor[0], this->m_pColor[1], this->m_pColor[2]});
		s_pRenderer->Push(std::make_unique<utility::Shape>(tab));
		
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

	void Tab::HandleMouse(int button, int state, int x, int y) noexcept(false)
	{
		this->m_TextArea.HandleMouse(button, state, x, y);

		if (!(x >= this->m_Position.x && x <= this->m_Position.x + this->m_Size.x && y >= this->m_Position.y && y <= this->m_Position.y + this->m_Size.y))
			return;

		if (!this->m_IsClicked && button == 0 && state == 0)
			this->m_IsClicked = true;
	}

	void Tab::HandleMotion(int x, int y) noexcept(false)
	{ this->m_TextArea.HandleMotion(x, y); }
} /* disxx::ui */
