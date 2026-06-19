module;

#ifdef __APPLE__
#	include <GLUT/glut.h>
#else
#	include <GL/freeglut.h>
#endif

#include <utility>

module disxx.ui.backend.GLUTContext;

namespace disxx::ui::backend
{
	GLUTContext::GLUTContext(void) noexcept
		: m_hWin{}
	{}

	GLUTContext::GLUTContext(GLUTContext &&other) noexcept
		: m_hWin{std::move(other.m_hWin)}
	{}

	GLUTContext &GLUTContext::operator=(GLUTContext &&other) noexcept
	{
		this->m_hWin = std::move(other.m_hWin);
		return *this;
	}

	GLUTContext::~GLUTContext(void) noexcept
    { this->DestroyWindow(); }

	GLUTContext::WindowHandle GLUTContext::CreateWindow(utility::Vec2<int> size, std::string_view title) noexcept
	{
		glutInitWindowSize(size.x, size.y);
		const auto hWin{glutCreateWindow(title.data())};
		glutHideWindow();
		return hWin;
	}

	void GLUTContext::SwitchWindow(WindowHandle &hWin) noexcept
	{
		glutSetWindow(hWin);
		this->m_hWin = hWin;
	}

	void GLUTContext::ShowWindow(void) noexcept
	{ glutShowWindow(); }
	
	void GLUTContext::HideWindow(void) noexcept
	{ glutHideWindow(); }

	void GLUTContext::DestroyWindow(void) noexcept
	{
		if (this->m_hWin) [[likely]]
			glutDestroyWindow(this->m_hWin);
	}

	void GLUTContext::SetDisplayCallback(const void *pCallback) noexcept
	{ glutDisplayFunc((void (*)(void))pCallback); }

	void GLUTContext::SetReshapeCallback(const void *pCallback) noexcept
	{ glutReshapeFunc((void (*)(int, int))pCallback); }

	void GLUTContext::SetKeyboardCallback(const void *pCallback) noexcept
	{ glutKeyboardFunc((void (*)(unsigned char, int, int))pCallback); }

	void GLUTContext::SetMouseButtonCallback(const void *pCallback) noexcept
	{ glutMouseFunc((void (*)(int, int, int, int))pCallback); }

	void GLUTContext::SetMouseMotionCallback(const void *pCallback) noexcept
	{ glutMotionFunc((void (*)(int, int))pCallback); }

	void GLUTContext::SwapBuffers(void) const noexcept
	{ glutSwapBuffers(); }

	void GLUTContext::Redisplay(void) const noexcept
	{ glutPostRedisplay(); }

	void GLUTContext::Exec(void) const noexcept
	{ glutMainLoop(); }
} /* disxx::ui::backend */
