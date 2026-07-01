module;

#include <disconf.hpp>

export module disxx.ui.backend.IRenderer;

export import <memory>;

export import disxx.ui.utility.Renderable;

export namespace disxx::ui::backend
{
	class __DISXX_EXPORT__ [[nodiscard]] IRenderer
	{
	  public:
		virtual void Push(std::unique_ptr<utility::Renderable> &&) noexcept = 0;
		virtual void Pop(void) noexcept = 0;		
		
		virtual void ClearBuffer(void) noexcept = 0;
		virtual void Render(void) noexcept = 0;
	};
} /* disxx::ui::backend */
