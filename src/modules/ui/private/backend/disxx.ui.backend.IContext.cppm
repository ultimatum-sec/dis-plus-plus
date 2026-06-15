module;

#include <disconf.hpp>

#include <utility>

export module disxx.ui.backend.IContext;

export import disxx.ui.utility.Vec;

export import <string_view>;

export namespace disxx::ui::backend
{
	template <typename T>
	class __DISXX_PRIVATE__ [[nodiscard]] IContext
	{
	  public:
		using WindowHandle = T;

	  public:
		virtual WindowHandle CreateWindow(utility::Vec2<int>, std::string_view) noexcept = 0;
		virtual void SwitchWindow(WindowHandle &) noexcept = 0;
		virtual void DestroyWindow(void) noexcept = 0;

		virtual void SwapBuffers(void) const noexcept = 0;
		virtual void Redisplay(void) const noexcept = 0;
	
		virtual void ExecLoop(void) const noexcept = 0;
	};
} /* disxx::ui::backend */
