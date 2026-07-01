module;

#include <disconf.hpp>

export module disxx.ui.MenuEntry;

export import <string_view>;
export import <functional>;
export import <string>;

import disxx.ui.Widget;

export namespace disxx::ui
{
	class __DISXX_EXPORT__ [[nodiscard]] MenuEntry final : public Widget
	{
	  private:
		std::function<void(void)> m_Callback{};
		std::string m_Text{};
	
	  public:
		explicit MenuEntry(void) noexcept;
		explicit MenuEntry(std::string_view, std::function<void(void)> m_Callback) noexcept;

		MenuEntry(const MenuEntry &) noexcept;
		MenuEntry &operator=(const MenuEntry &) noexcept;
	
		MenuEntry(MenuEntry &&) noexcept;
		MenuEntry &operator=(MenuEntry &&) noexcept;

		inline void SetText(std::string_view) noexcept;
		inline void SetCallback(std::function<void(void)>) noexcept;

		inline std::string_view GetText(void) const noexcept;

		virtual void Render(void) const noexcept override;
		virtual void HandleMouse(int, int, int, int) noexcept override;
	
		inline void operator()(void) const noexcept(false);
	};

	inline void MenuEntry::SetText(std::string_view str) noexcept
	{ this->m_Text = str.data(); }

	inline void MenuEntry::SetCallback(std::function<void(void)> f) noexcept
	{ this->m_Callback = f; }

	inline std::string_view MenuEntry::GetText(void) const noexcept
	{ return this->m_Text; }

	inline void MenuEntry::operator()(void) const noexcept(false)
	{ this->m_Callback(); }
} /* disxx::ui::utility */
