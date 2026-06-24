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

		void SetText(std::string_view) noexcept;
		std::string_view GetText(void) const noexcept;
	};
} /* disxx::ui::utility */
