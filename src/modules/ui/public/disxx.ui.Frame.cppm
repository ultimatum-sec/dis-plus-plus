module;

#include <disconf.hpp>

export module disxx.ui.Frame;

import disxx.ui.Widget;

export namespace disxx::ui
{
	class __DISXX_EXPORT__ [[nodiscard]] Frame final : public Widget
	{
	  public:
		explicit Frame(void) noexcept;
		explicit Frame(float, float, float, float) noexcept;

		Frame(const Frame &) noexcept;
		Frame &operator=(const Frame &) noexcept;

		Frame(Frame &&) noexcept;
		Frame &operator=(Frame &&) noexcept;

		virtual void Render(void) const noexcept override;
	};
} /* disxx::ui */
