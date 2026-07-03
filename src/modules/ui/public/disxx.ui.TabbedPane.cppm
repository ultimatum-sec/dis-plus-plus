module;

#include <disconf.hpp>

#include <algorithm>
#include <ranges>
#include <vector>

export module disxx.ui.TabbedPane;

export import disxx.ui.Tab;
import disxx.ui.Widget;

export namespace disxx::ui
{
	class __DISXX_EXPORT__ [[nodiscard]] TabbedPane final : public Widget
	{
	  private:
		std::vector<Tab> m_Tabs{};

	  public:
		explicit TabbedPane(void) noexcept;
		explicit TabbedPane(float, float, float, float) noexcept;

		TabbedPane(const TabbedPane &) noexcept;
		TabbedPane &operator=(const TabbedPane &) noexcept;

		TabbedPane(TabbedPane &&) noexcept;
		TabbedPane &operator=(TabbedPane &&) noexcept;

		inline void Push(Tab &&) noexcept;
		inline void Pop(void) noexcept;

		virtual void Render(void) const noexcept override;
		virtual void HandleMouse(int, int, int, int) noexcept(false) override;
		virtual void HandleMotion(int, int) noexcept(false) override;
	};

	inline void TabbedPane::Push(Tab &&tab) noexcept
	{
		this->m_Tabs.push_back(std::forward<Tab &&>(tab));
		
		for (const auto i : std::views::iota(0ul, this->m_Tabs.size()))
		{
			auto &iTab{this->m_Tabs[i]};
			iTab.Replace(utility::Vec2<float>{this->m_Position.x + (this->m_Size.x / this->m_Tabs.size()) * i, this->m_Size.y * 0.95f});
			iTab.Resize(utility::Vec2<float>{(this->m_Size.x / this->m_Tabs.size()), this->m_Size.y * 0.05f});
			
			auto &area{iTab.GetTextArea()};
			area.Replace(utility::Vec2<float>{this->m_Position.x, this->m_Position.y});
			area.Resize(utility::Vec2<float>{this->m_Size.x, this->m_Size.y * 0.95f});
		}
	}

	inline void TabbedPane::Pop(void) noexcept
	{
		if (this->m_Tabs.size() > 0) [[likely]]
			this->m_Tabs.pop_back();
	}
} /* disxx::ui */
