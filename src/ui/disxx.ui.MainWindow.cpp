module;

#include <vector>
#include <memory>

#define MKPTR(ptr) \
	if (!ptr) [[unlikely]] \
		ptr = std::make_shared<disxx::ui::backend::GLUTContext>()

module disxx.ui.MainWindow;

namespace disxx::ui
{
	disxx::utility::wrapper::Pointer
	<
		std::shared_ptr
		<
			#if defined(BACKEND_CTX_GLUT)
				backend::GLUTContext
			#endif
		>
	> MainWindow::s_pContext{nullptr};

	MainWindow::MainWindow(void) noexcept
		: m_Widgets{}
		, m_Width{0}
		, m_Height{0}
		, m_Win{0}
	{ MKPTR(s_pContext); }

	MainWindow::MainWindow(int width, int height) noexcept
		: m_Widgets{}
		, m_Width{width}
		, m_Height{height}
		, m_Win{0}
	{ MKPTR(s_pContext); }
} /* disxx::ui */
