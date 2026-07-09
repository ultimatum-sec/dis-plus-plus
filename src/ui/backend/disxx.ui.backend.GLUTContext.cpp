module;

#ifdef __APPLE__
#	include <GLUT/glut.h>
#else
#	include <GL/freeglut.h>
#endif

#include <unordered_map>
#include <utility>

module disxx.ui.backend.GLUTContext;

namespace disxx::ui::backend
{
	std::unordered_map
	<
		GLUTContext::WindowHandle,
		std::function<void(void)>
	> GLUTContext::s_DisplayCallbacks{};
	std::unordered_map
	<
		GLUTContext::WindowHandle,
		std::function<void(int, int)>
	> GLUTContext::s_ReshapeCallbacks{};
	std::unordered_map
	<
		GLUTContext::WindowHandle,
		std::function<void(unsigned char, int, int)>
	> GLUTContext::s_KeyboardCallbacks{};
	std::unordered_map
	<
		GLUTContext::WindowHandle,
		std::function<void(int, int, int, int)>
	> GLUTContext::s_MouseButtonCallbacks{};
	std::unordered_map
	<
		GLUTContext::WindowHandle,
		std::function<void(int, int)>
	> GLUTContext::s_MouseMotionCallbacks{};

	[[clang::acquire_handle("Window")]]
	GLUTContext::WindowHandle GLUTContext::CreateWindow(utility::Vec2<int> size, std::string_view title) noexcept
	{
		glutInitWindowSize(size.x, size.y);
		const auto hWin{glutCreateWindow(title.data())};
		
		glutDisplayFunc
		(
			[] -> void
			{ s_DisplayCallbacks.at(glutGetWindow())(); }
		);

		glutReshapeFunc
		(
			[](int width, int height) -> void
			{
				s_ReshapeCallbacks.at(glutGetWindow())
				(
					width,
					height
				);
			}
		);

		glutKeyboardFunc
		(
			[](unsigned char key, int x, int y) -> void
			{
				s_KeyboardCallbacks.at(glutGetWindow())
				(
					key,
					x,
					y
				);
			}
		);

		glutMouseFunc
		(
			[](int button, int state, int x, int y) -> void
			{
				s_MouseButtonCallbacks.at(glutGetWindow())
				(
					button,
					state,
					x,
					y
				);
			}
		);

		glutMotionFunc
		(
			[](int x, int y) -> void
			{
				s_MouseMotionCallbacks.at(glutGetWindow())
				(
					x,
					y
				);
			}
		);

		glutHideWindow();

		return hWin;
	}

	void GLUTContext::DestroyWindow([[clang::release_handle("Window")]] WindowHandle &hWin) noexcept
	{
		if (hWin) [[likely]]
			glutDestroyWindow(hWin);
	}

	void GLUTContext::SwitchWindow([[clang::use_handle("Window")]] const WindowHandle &hWin) noexcept
	{ glutSetWindow(hWin); }

	void GLUTContext::ShowWindow(void) noexcept
	{ glutShowWindow(); }
	
	void GLUTContext::HideWindow(void) noexcept
	{ glutHideWindow(); }

	void GLUTContext::SwapBuffers(void) const noexcept
	{ glutSwapBuffers(); }

	void GLUTContext::Redisplay(void) const noexcept
	{ glutPostRedisplay(); }

	void GLUTContext::Exec(void) const noexcept
	{ glutMainLoop(); }
} /* disxx::ui::backend */
