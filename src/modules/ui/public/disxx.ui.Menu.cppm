module;

#include <disconf.hpp>

#include <vector>
#include <string>

export module disxx.ui.Menu;

export import <string_view>;

import disxx.ui.Widget;

export import disxx.ui.Button;

export namespace disxx::ui
{
	class __DISXX_EXPORT__ [[nodiscard]] Menu final : public Widget
	{
	  private:
		std::vector<Button> m_Entries{};
		std::string m_Text{};

	  public:
		explicit Menu(void) noexcept;
		explicit Menu(float, float, float, float) noexcept;

		Menu(const Menu &) noexcept;
		Menu &operator=(const Menu &) noexcept;

		Menu(Menu &&) noexcept;
		Menu &operator=(Menu &&) noexcept;

		inline void SetText(std::string_view) noexcept;

		inline void PushEntry(Button &&) noexcept;
		inline void PopEntry(void) noexcept;

		virtual void HandleMouse(int, int, int, int) noexcept override;
		virtual void Render(void) const noexcept override;
	};

	inline void Menu::SetText(std::string_view text) noexcept
	{ this->m_Text = text.data(); }

	inline void Menu::PushEntry(Button &&btn) noexcept
	{
		btn.Resize(utility::Vec2<float>{this->m_Size});
		btn.Replace
		(
			utility::Vec2<float>
			{
				this->m_Position.x,
				this->m_Position.y - this->m_Size.y * (this->m_Entries.size() + 1)
			}
		);
		this->m_Entries.emplace_back(std::move(btn));
	}

	inline void Menu::PopEntry(void) noexcept
	{
		if (!this->m_Entries.empty())
			this->m_Entries.pop_back();
	}
} /* disxx::ui */
