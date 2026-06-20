module;

#include <filesystem>
#include <functional>
#include <string>
#include <tuple>

module disxx.ui.Button;

import disxx.ui.backend.GLRenderer;
import disxx.ui.utility.Shape;
import disxx.ui.utility.Text;
import disxx.ui.utility.Vec;

namespace disxx::ui
{
	Button::Button(void) noexcept
		: Widget{}
		, m_Callback{[](const Widget *const) -> void {}}
		, m_Trigger{ButtonTrigger::BTN_NONE}
		, m_Image{}
		, m_Text{}	
	{}

	Button::Button(float x, float y, float width, float height) noexcept
		: Widget{x, y, width, height}
		, m_Callback{[](const Widget *const) -> void {}}
		, m_Trigger{ButtonTrigger::BTN_NONE}
		, m_Image{}
		, m_Text{}
	{}

	Button::Button(const Button &other) noexcept
		: Widget{other}
		, m_Callback{other.m_Callback}
		, m_Trigger{other.m_Trigger}
		, m_Image{other.m_Image}
		, m_Text{other.m_Text}
	{}

	Button &Button::operator=(const Button &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			Widget::operator=(other);
			this->m_Callback = other.m_Callback;
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
		// Add a shape
		utility::Shape s{utility::Shape::Type::RECTANGLE};
		s.Replace(utility::Vec2<float>{this->m_Position.x, this->m_Position.y});
		s.Resize(utility::Vec2<float>{this->m_Size.x, this->m_Size.y});
		s.SetColor(utility::Vec3<float>{this->m_pColor[0], this->m_pColor[1], this->m_pColor[2]});
		s_pRenderer->PushShape(std::move(s));
		
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
			s_pRenderer->PushText(std::move(txt));
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

		if (this->m_IsClicked && this->m_Trigger == ButtonTrigger::BTN_CLICKED)
			this->m_Callback(this);
	}

	void Button::HandleMotion(int x, int y) noexcept
	{
		this->m_IsHovered = (x >= this->m_Position.x && x <= this->m_Position.x + this->m_Size.x && y >= this->m_Position.y && y <= this->m_Position.y + this->m_Size.y);
		if (this->m_IsHovered && this->m_Trigger == ButtonTrigger::BTN_HOVERED)
			this->m_Callback(this);
	}
} /* disxx::ui */
