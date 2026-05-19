module;

#include <vector>
#include <memory>

module disxx.ui.MainWindow;

namespace disxx::ui
{
	MainWindow::MainWindow(void) noexcept
		: m_Widgets{}
		, m_Width{0}
		, m_Height{0}
		, m_Win{0}
	{}

	MainWindow::MainWindow(int width, int height) noexcept
		: m_Widgets{}
		, m_Width{width}
		, m_Height{height}
		, m_Win{0}
	{}
} /* disxx::ui */


