module;

#include <type_traits>
#include <vector>
#include <memory>

export module disxx.ui.MainWindow;

export import disxx.ui.utility.Vec;

import disxx.ui.backend.GLUTContext;
import disxx.ui.backend.IContext;
import disxx.ui.Widget;

export namespace disxx::ui
{
	class MainWindow
	{
	  private:
		#ifdef BACKEND_CTX_GLUT
			using WindowHandle = int;
		#else
		#	error "Context required"
		#endif

	  private:
		static std::unique_ptr<MainWindow> s_pInstance;

	  private:
		#if defined(BACKEND_CTX_GLUT)
			disxx::ui::backend::GLUTContext
		#else
		#	error "Context required"
		#endif
		m_Context{};
		std::vector<std::unique_ptr<Widget>> m_Widgets{};
		utility::Vec2<int> m_InitialSize{};
		utility::Vec2<int> m_Size{};
		WindowHandle m_hWin{};

	  private:
		explicit MainWindow(utility::Vec2<int>, std::string_view) noexcept;

		void __DisplayCallback(void) const noexcept;
		void __ReshapeCallback(int, int) noexcept(false);
		void __KeyboardCallback(unsigned char, int, int) noexcept(false);
		void __MouseButtonCallback(int, int, int, int) noexcept(false);
		void __MouseMotionCallback(int, int) noexcept(false);
	  
	  public:
		template <typename ...Args>
		static void InitContext(Args &&...) noexcept;
		static std::unique_ptr<MainWindow> &Init(utility::Vec2<int>, std::string_view) noexcept;

	  public:
		explicit MainWindow(const MainWindow &) = delete;
		MainWindow &operator=(const MainWindow &) = delete;

		~MainWindow(void) noexcept;

		inline utility::Vec2<int> GetSize(void) const noexcept;
		inline void SetVisible(bool) noexcept;
		inline void AddWidget(std::unique_ptr<Widget> &&) noexcept;
		inline std::vector<std::unique_ptr<Widget>> &GetWidgets(void) noexcept;
		inline void Redisplay(void) const noexcept;
		inline void Exec(void) noexcept;
	};

	template <typename ...Args>
	void MainWindow::InitContext(Args &&...args) noexcept
	{
		#if defined(BACKEND_CTX_GLUT)
			backend::GLUTContext::Init(std::forward<Args>(args)...);
		#else
		#	error "Context required"
		#endif
	}

	inline utility::Vec2<int> MainWindow::GetSize(void) const noexcept
	{ return utility::Vec2<int>{this->m_Size}; }

	inline void MainWindow::SetVisible(bool visible) noexcept
	{
		this->m_Context.SwitchWindow(this->m_hWin);
		if (visible)
			m_Context.ShowWindow();
		else
			m_Context.HideWindow();
	}

	inline void MainWindow::AddWidget(std::unique_ptr<Widget> &&pWidget) noexcept
	{ this->m_Widgets.emplace_back(std::move(pWidget)); }

	inline std::vector<std::unique_ptr<Widget>> &MainWindow::GetWidgets(void) noexcept
	{ return this->m_Widgets; }

	inline void MainWindow::Redisplay(void) const noexcept
	{ this->m_Context.Redisplay(); }

	inline void MainWindow::Exec(void) noexcept
	{
		//this->m_Context.SwitchWindow(this->m_hWin);
		m_Context.Exec();
	}
} /* disxx::ui */
