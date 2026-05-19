module;

#include <type_traits>
#include <vector>
#include <memory>

export module disxx.ui.MainWindow;

import disxx.ui.Widget;

export namespace disxx::ui
{
	class MainWindow
	{
	  protected:
		std::vector<std::unique_ptr<disxx::ui::Widget>> m_Widgets;
		int m_Width, m_Height;
		int m_Win;

	  protected:
		virtual void __KeyboardFunc(unsigned char, int, int) noexcept(false) = 0;
		virtual void __MouseFunc(int, int, int, int) noexcept(false) = 0;
		virtual void __ReshapeFunc(int, int) noexcept(false) = 0;
		virtual void __MotionFunc(int, int) noexcept(false) = 0;
		virtual void __DisplayFunc(void) noexcept = 0;

		explicit MainWindow(void) noexcept;
		explicit MainWindow(int, int) noexcept;
		explicit MainWindow(const MainWindow &) = delete;
		MainWindow &operator=(const MainWindow &) = delete;

	  public:
		virtual ~MainWindow(void) noexcept = default;
	};
} /* disxx::ui */







