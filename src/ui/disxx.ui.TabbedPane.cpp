module;

#include <utility>
#include <memory>
#include <vector>

module disxx.ui.TabbedPane;

import disxx.ui.utility.Shape;
import disxx.ui.utility.Text;

namespace disxx::ui
{
	TabbedPane::TabbedPane(void) noexcept
		: Widget{}
		, m_Tabs{}
	{}

	TabbedPane::TabbedPane(float x, float y, float width, float height) noexcept
		: Widget{x, y, width, height}
		, m_Tabs{}
	{}

	TabbedPane::TabbedPane(const TabbedPane &other) noexcept
		: Widget{other}
		, m_Tabs{other.m_Tabs}
	{}

	TabbedPane &TabbedPane::operator=(const TabbedPane &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			Widget::operator=(other);
			this->m_Tabs = other.m_Tabs;
		}

		return *this;
	}

	TabbedPane::TabbedPane(TabbedPane &&other) noexcept
		: Widget{std::forward<TabbedPane &&>(other)}
		, m_Tabs{std::move(other.m_Tabs)}
	{}

	TabbedPane &TabbedPane::operator=(TabbedPane &&other) noexcept
	{
		Widget::operator=(std::forward<TabbedPane &&>(other));
		this->m_Tabs = std::move(other.m_Tabs);
		
		return *this;
	}

	void TabbedPane::Render(void) const noexcept
	{
		// Add a subframe
		utility::Shape subframe{utility::Shape::Type::TYPE_RECTANGLE};
		subframe.Replace(utility::Vec2<float>{this->m_Position.x - 1.f, this->m_Position.y - 1.f});
		subframe.Resize(utility::Vec2<float>{this->m_Size.x + 2.f, this->m_Size.y + 2.f});
		subframe.SetColor(utility::Vec3<float>{0.f, 0.f, 0.f});
		s_pRenderer->Push(std::make_unique<utility::Shape>(subframe));

		// Add the frame itself
		utility::Shape frame{utility::Shape::Type::TYPE_RECTANGLE};
		frame.Replace(utility::Vec2<float>{this->m_Position.x, this->m_Position.y});
		frame.Resize(utility::Vec2<float>{this->m_Size.x, this->m_Size.y});
		frame.SetColor(utility::Vec3<float>{this->m_pColor[0], this->m_pColor[1], this->m_pColor[2]});
		s_pRenderer->Push(std::make_unique<utility::Shape>(frame));

		for (const auto &tab : this->m_Tabs)
		{
			tab.Render();
			if (tab.Clicked())
				tab.GetTextArea().Render();
		}
	}

	void TabbedPane::HandleMouse(int button, int state, int x, int y) noexcept(false)
	{
		if (!(x >= this->m_Position.x && x <= this->m_Position.x + this->m_Size.x && y >= this->m_Position.y && y <= this->m_Position.y + this->m_Size.y))
			return;

		for (auto &tab : this->m_Tabs)
		{
			const auto [tabX, tabY]{tab.GetPosition()};
			const auto [tabWidth, tabHeight]{tab.GetSize()};
			const auto cond{x >= tabX && x <= tabX + tabWidth && y >= tabY && y <= tabY + tabHeight};
			if (button == 0 && state == 0 && cond && tab.Clicked())
				tab.SetPassive();
			else if (cond || tab.Clicked())
				tab.HandleMouse(button, state, x, y);
		}
	}

	void TabbedPane::HandleMotion(int x, int y) noexcept(false)
	{
		for (auto &tab : this->m_Tabs)
			if (tab.Clicked())
				tab.GetTextArea().HandleMotion(x, y);
	}
} /* disxx::ui */
