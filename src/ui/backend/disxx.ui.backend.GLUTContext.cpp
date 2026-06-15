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

	GLUTContext::WindowHandle GLUTContext::CreateWindow(utility::Vec2<int> size, std::string_view title) noexcept
	{
		glutInitWindowSize(size.x, size.y);
		return glutCreateWindow(title.data());
	}

	GLUTContext::~GLUTContext(void) noexcept
    { this->DestroyWindow(); }

	void GLUTContext::SwitchWindow(WindowHandle &hWin) noexcept
	{
		glutSetWindow(hWin);
		this->m_hWin = hWin;
	}

	void GLUTContext::DestroyWindow(void) noexcept
	{
		if (this->m_hWin) [[likely]]
			glutDestroyWindow(this->m_hWin);
	}

	void GLUTContext::SwapBuffers(void) const noexcept
	{ glutSwapBuffers(); }

	void GLUTContext::Redisplay(void) const noexcept
	{ glutPostRedisplay(); }

	void GLUTContext::ExecLoop(void) const noexcept
	{ glutMainLoop(); }
} /* disxx::ui::backend */
