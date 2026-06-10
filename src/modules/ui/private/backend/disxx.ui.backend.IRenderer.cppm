module;

#include <disconf.hpp>

export module disxx.ui.backend.IRenderer;

export import disxx.ui.utility.Shape;
export import disxx.ui.utility.Text;

export namespace disxx::ui::backend
{
	class __DISXX_EXPORT__ [[nodiscard]] IRenderer
	{
	  public:
		virtual ~IRenderer(void) noexcept = default;

		virtual void PushShape(utility::Shape &&) noexcept = 0;
		virtual void PopShape(void) noexcept = 0;		
		
		virtual void PushText(utility::Text &&) noexcept = 0;
		virtual void PopText(void) noexcept = 0;

		virtual void ClearBuffers(void) noexcept = 0;
		virtual void Render(void) noexcept = 0;
	};
} /* disxx::ui::backend */
