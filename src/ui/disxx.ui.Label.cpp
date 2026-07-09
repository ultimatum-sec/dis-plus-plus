module;

#include <utility>
#include <memory>

module disxx.ui.Label;

import disxx.ui.utility.Text;

namespace disxx::ui
{
	Label::Label(void) noexcept
		: Widget{}
		, m_Text{}
	{}

	Label::Label(float x, float y, float width, float height) noexcept
		: Widget{x, y, width, height}
		, m_Text{}
	{}

	Label::Label(const Label &other) noexcept
		: Widget{other}
		, m_Text{other.m_Text}
	{}

	Label &Label::operator=(const Label &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			Widget::operator=(other);
			this->m_Text = other.m_Text;
		}

		return *this;
	}

	Label::Label(Label &&other) noexcept
		: Widget{std::forward<Label &&>(other)}
		, m_Text{std::move(other.m_Text)}
	{}

	Label &Label::operator=(Label &&other) noexcept
	{
		Widget::operator=(std::forward<Label &&>(other));
		this->m_Text = std::move(other.m_Text);

		return *this;
	}

	std::unique_ptr<Widget> Label::Clone(void) const noexcept
	{ return std::make_unique<std::decay<decltype(*this)>::type>(*this); }

	void Label::Render(void) const noexcept
	{
		// Render the text
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
} /* disxx::ui */
