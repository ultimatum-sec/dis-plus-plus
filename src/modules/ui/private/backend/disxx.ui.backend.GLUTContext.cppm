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
		WindowHandle m_hWin;

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

		virtual WindowHandle CreateWindow(utility::Vec2<int>, std::string_view) noexcept override;
		virtual void SwitchWindow(WindowHandle &) noexcept override;
		virtual void DestroyWindow(void) noexcept override;

		template <typename F>
		void SetDisplayCallback(F) noexcept;
		template <typename F>
		void SetReshapeCallback(F) noexcept;
		template <typename F>
		void SetKeyboardCallback(F) noexcept;
		template <typename F>
		void SetMouseButtonCallback(F) noexcept;
		template <typename F>
		void SetMouseMotionCallback(F) noexcept;
		
		virtual void SwapBuffers(void) const noexcept override;
		virtual void Redisplay(void) const noexcept override;
	
		virtual void ExecLoop(void) const noexcept override;
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

	template <typename F>
	void GLUTContext::SetDisplayCallback(F pCallback) noexcept
	{ glutDisplayFunc(pCallback); }
		
	template <typename F>
	void GLUTContext::SetReshapeCallback(F pCallback) noexcept
	{ glutReshapeFunc(pCallback); }
	
	template <typename F>
	void GLUTContext::SetKeyboardCallback(F pCallback) noexcept
	{ glutKeyboardFunc(pCallback); }

	template <typename F>
	void GLUTContext::SetMouseButtonCallback(F pCallback) noexcept
	{ glutMouseFunc(pCallback); }
	
	template <typename F>
	void GLUTContext::SetMouseMotionCallback(F pCallback) noexcept
	{ glutMotionFunc(pCallback); }
} /* disxx::ui::backend */
