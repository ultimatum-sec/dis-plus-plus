module;

#include <disconf.hpp>

#include <algorithm>
#include <ranges>
#include <vector>
#include <string>

export module disxx.ui.Menu;

export import <string_view>;
export import <memory>;

import disxx.ui.Widget;

export import disxx.ui.MenuEntry;

export namespace disxx::ui
{
	class __DISXX_EXPORT__ [[nodiscard]] Menu final : public Widget
	{
	  private:
		std::vector<MenuEntry> m_Entries{};
		std::string m_Text{};

	  public:
		explicit Menu(void) noexcept;
		explicit Menu(float, float, float, float) noexcept;

		Menu(const Menu &) noexcept;
		Menu &operator=(const Menu &) noexcept;

		Menu(Menu &&) noexcept;
		Menu &operator=(Menu &&) noexcept;

		inline void SetText(std::string_view) noexcept;

		inline void PushEntry(MenuEntry &&) noexcept;
		inline void PopEntry(void) noexcept;

		virtual std::unique_ptr<Widget> Clone(void) const noexcept override;

		virtual void HandleMouse(int, int, int, int) noexcept override;
		virtual void Render(void) const noexcept override;
	};

	inline void Menu::SetText(std::string_view text) noexcept
	{ this->m_Text = text.data(); }

	inline void Menu::PushEntry(MenuEntry &&entry) noexcept
	{
		entry.Replace
		(
			utility::Vec2<float>
			{
				this->m_Position.x,
				this->m_Position.y - this->m_Size.y * (this->m_Entries.size() + 1)
			}
		);

		this->m_Entries.emplace_back(std::forward<MenuEntry &&>(entry));

		const auto width
		{
			std::max
			(
				std::ranges::max
				(
					this->m_Entries
						| std::views::all
						| std::views::transform([](const auto &var) -> float { return var.GetText().size() * 8.f; })
						| std::ranges::to<std::vector<float>>()
				) + 10.f,
				this->m_Size.x
			)
		};

		// Resize all the entries to max entry
		for (auto &e : this->m_Entries)
			e.Resize(utility::Vec2<float>{width, this->m_Size.y});
	}

	inline void Menu::PopEntry(void) noexcept
	{
		if (!this->m_Entries.empty())
			this->m_Entries.pop_back();
	}
} /* disxx::ui */
