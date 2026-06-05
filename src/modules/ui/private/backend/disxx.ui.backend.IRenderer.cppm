module;

#include <disconf.hpp>

export module disxx.ui.backend.IRenderer;

export import disxx.ui.utility.Shape;

export namespace disxx::ui::backend
{
	class __DISXX_EXPORT__ [[nodiscard]] IRenderer
	{
	  public:
		virtual ~IRenderer(void) noexcept = default;

		virtual void ResizeWindow(float, float) noexcept = 0;

		virtual void AddShape(utility::Shape &&) noexcept = 0;
		virtual void Render(void) noexcept = 0;
	};
} /* disxx::ui::backend */
