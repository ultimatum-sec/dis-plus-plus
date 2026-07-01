module;

#include <disconf.hpp>

#include <string_view>
#include <cstdint>
#include <string>

export module disxx.ui.utility.Text;

import disxx.ui.utility.Renderable;

export namespace disxx::ui::utility
{
	class __DISXX_PRIVATE__ [[nodiscard]] Text final : public Renderable
	{
	  private:
		std::string m_Text{};

	  public:
		explicit Text(void) noexcept;
		explicit Text(std::string_view) noexcept;

		explicit Text(const Text &) noexcept;
		Text &operator=(const Text &) noexcept;

		explicit Text(Text &&) noexcept;
		Text &operator=(Text &&) noexcept;

		inline void SetText(std::string_view) noexcept;
		inline std::string_view GetText(void) const noexcept;
	};

	inline void Text::SetText(std::string_view str) noexcept
	{ this->m_Text = str; }

	inline std::string_view Text::GetText(void) const noexcept
	{ return this->m_Text; }
} /* disxx::ui::utility */
