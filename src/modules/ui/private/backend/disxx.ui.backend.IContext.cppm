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
		// Some window methods
		[[clang::acquire_handle("Window")]]
		virtual WindowHandle CreateWindow(utility::Vec2<int>, std::string_view) noexcept = 0;
		virtual void DestroyWindow([[clang::release_handle("Window")]] WindowHandle &) noexcept = 0;
		virtual void SwitchWindow([[clang::use_handle("Window")]] const WindowHandle &) noexcept = 0;
		virtual void ShowWindow(void) noexcept = 0;
		virtual void HideWindow(void) noexcept = 0;

		// Miscellaneous methods
		virtual void SwapBuffers(void) const noexcept = 0;
		virtual void Redisplay(void) const noexcept = 0;
		virtual void Exec(void) const noexcept = 0;
	};
} /* disxx::ui::backend */
