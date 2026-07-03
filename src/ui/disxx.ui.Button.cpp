module;

#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <tuple>

module disxx.ui.Button;

import disxx.ui.backend.GLUTContext;
import disxx.ui.backend.GLRenderer;
import disxx.ui.utility.Shape;
import disxx.ui.utility.Text;
import disxx.ui.utility.Vec;

namespace disxx::ui
{
	Button::Button(void) noexcept
		: Widget{}
		, m_Callback{[](const Widget *const) -> void {}}
		, m_Text{}
		, m_Trigger{Trigger::BTN_NONE}
	{}

	Button::Button(float x, float y, float width, float height) noexcept
		: Widget{x, y, width, height}
		, m_Callback{[](const Widget *const) -> void {}}
		, m_Text{}
		, m_Trigger{Trigger::BTN_NONE}
	{}

	Button::Button(const Button &other) noexcept
		: Widget{other}
		, m_Callback{other.m_Callback}
		, m_Text{other.m_Text}
		, m_Trigger{other.m_Trigger}
	{}

	Button &Button::operator=(const Button &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			Widget::operator=(other);
			this->m_Callback = other.m_Callback;
			this->m_Text = other.m_Text;
			this->m_Trigger = other.m_Trigger;
		}

		return *this;
	}

	Button::Button(Button &&other) noexcept
		: Widget{std::forward<Button &&>(other)}
		, m_Callback{std::move(other.m_Callback)}
		, m_Text{std::move(other.m_Text)}
		, m_Trigger{std::move(other.m_Trigger)}
	{}

	Button &Button::operator=(Button &&other) noexcept
	{
		Widget::operator=(std::forward<Button &&>(other));
		this->m_Callback = std::move(other.m_Callback);
		this->m_Text = std::move(other.m_Text);
		this->m_Trigger = std::move(other.m_Trigger);

		return *this;
	}

	void Button::Render(void) const noexcept
	{
		// Add a frame
		utility::Shape frame{utility::Shape::Type::TYPE_RECTANGLE};
		frame.Replace(utility::Vec2<float>{this->m_Position.x - 1.f, this->m_Position.y - 1.f});
		frame.Resize(utility::Vec2<float>{this->m_Size.x + 2.f, this->m_Size.y + 2.f});
		frame.SetColor(utility::Vec3<float>{0.f, 0.f, 0.f});
		s_pRenderer->Push(std::make_unique<utility::Shape>(frame));

		// Add the button itself
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

	void Button::HandleMouse(int button, int state, int x, int y) noexcept
	{
		if (!(x >= this->m_Position.x && x <= this->m_Position.x + this->m_Size.x && y >= this->m_Position.y && y <= this->m_Position.y + this->m_Size.y))
			return;

		if (button == 0 && state == 0)
			this->m_IsClicked = true;
		else if (button == 0 && state == 1)
			this->m_IsClicked = false;

		if (this->m_IsClicked && this->m_Trigger == Trigger::BTN_CLICKED)
			this->m_Callback(this);
	}

	void Button::HandleMotion(int x, int y) noexcept
	{
		this->m_IsHovered = (x >= this->m_Position.x && x <= this->m_Position.x + this->m_Size.x && y >= this->m_Position.y && y <= this->m_Position.y + this->m_Size.y);
		if (this->m_IsHovered && this->m_Trigger == Trigger::BTN_HOVERED)
			this->m_Callback(this);
	}
} /* disxx::ui */
