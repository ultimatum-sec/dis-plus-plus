module;

#include <disconf.hpp>

#ifdef __APPLE__
#	include <GLUT/glut.h>
#else
#	include <GL/freeglut.h>
#endif

export module disxx.ui.backend.GLUTContext;

export import <string_view>;
export import <utility>;

import disxx.ui.backend.IContext;

export namespace disxx::ui::backend
{
	class __DISXX_EXPORT__ [[nodiscard]] GLUTContext final : public IContext<int>
	{
	  private:
		WindowHandle m_hWin{};

	  public:
		template <typename ...Args>
		static void Init(Args &&...) noexcept;

	  public:
		explicit GLUTContext(void) noexcept;

		explicit GLUTContext(const GLUTContext &) noexcept = delete;
		GLUTContext &operator=(const GLUTContext &) noexcept = delete;

		explicit GLUTContext(GLUTContext &&) noexcept;
		GLUTContext &operator=(GLUTContext &&) noexcept;

		~GLUTContext(void) noexcept;

		// Sone window methods
		[[clang::acquire_handle("window")]]
		virtual WindowHandle CreateWindow(utility::Vec2<int>, std::string_view) noexcept override;
		virtual void SwitchWindow([[clang::use_handle("window")]] WindowHandle &) noexcept override;
		virtual void ShowWindow(void) noexcept override;
		virtual void HideWindow(void) noexcept override;
		virtual void DestroyWindow(void) noexcept override;

		// Callback methods
		virtual void SetDisplayCallback(const void *) noexcept override;
		virtual void SetReshapeCallback(const void *) noexcept override;
		virtual void SetKeyboardCallback(const void *) noexcept override;
		virtual void SetMouseButtonCallback(const void *) noexcept override;
		virtual void SetMouseMotionCallback(const void *) noexcept override;
	
		// Miscellaneous methods
		virtual void SwapBuffers(void) const noexcept override;
		virtual void Redisplay(void) const noexcept override;
		virtual void Exec(void) const noexcept override;
	};

	template <typename ...Args>
	void GLUTContext::Init(Args &&...args) noexcept
	{
		if constexpr (sizeof...(args) == 0)
		{
			static int argc{0};
			static char *argv[] = {"\0"};
			glutInit(&argc, argv);
		}
		else if constexpr (sizeof...(args) == 2)
			glutInit(std::forward<Args>(args)...);
		else
			static_assert(false, "Incorrect arguments for disxx::ui::backend::GLUTContext::Init");
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	}
} /* disxx::ui::backend */
