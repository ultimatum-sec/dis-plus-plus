module;

#include <disconf.hpp>

#include <type_traits>
#include <vector>
#include <memory>

export module disxx.ui.MainWindow;

export import disxx.ui.utility.Vec;
import disxx.utility.pointer.NonNull;
import disxx.ui.backend.GLUTContext;
import disxx.ui.backend.IContext;
import disxx.ui.Widget;

export namespace disxx::ui
{
	class __DISXX_EXPORT__ [[nodiscard]] MainWindow
	{
	  private:
		#ifdef BACKEND_CTX_GLUT
			using WindowHandle = int;
		#else
		#	error "Context required"
		#endif

	  private:
		static disxx::utility::pointer::NonNull
		<
			#if defined(BACKEND_CTX_GLUT)
				disxx::ui::backend::GLUTContext
			#else
			#	error "Context required"
			#endif
		> s_pContext;
		
	  private:
		std::vector<std::unique_ptr<Widget>> m_Widgets{};
		utility::Vec2<int> m_InitialSize{};
		utility::Vec2<int> m_Size{};
		WindowHandle m_hWin{};

	  private:
		void DisplayCallback(void) const noexcept;
		void ReshapeCallback(int, int) noexcept(false);
		void KeyboardCallback(unsigned char, int, int) noexcept(false);
		void MouseButtonCallback(int, int, int, int) noexcept(false);
		void MouseMotionCallback(int, int) noexcept(false);
	  
	  public:
		template <typename ...Args>
		static void Init(Args &&...) noexcept;

	  public:
		explicit MainWindow(void) noexcept;
		explicit MainWindow(utility::Vec2<int>, std::string_view) noexcept;

		MainWindow(const MainWindow &) noexcept;
		MainWindow &operator=(const MainWindow &) noexcept;

		MainWindow(MainWindow &&) noexcept;
		MainWindow &operator=(MainWindow &&) noexcept;

		~MainWindow(void) noexcept;

		inline utility::Vec2<int> GetSize(void) const noexcept;
		inline void SetVisible(bool) noexcept;
		inline void AddWidget(std::unique_ptr<Widget> &&) noexcept;
		inline std::vector<std::unique_ptr<Widget>> &GetWidgets(void) noexcept;
		inline void Redisplay(void) const noexcept;
		inline void Exec(void) const noexcept;
	};

	template <typename ...Args>
	void MainWindow::Init(Args &&...args) noexcept
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
		s_pContext->SwitchWindow(this->m_hWin);
		if (visible)
			s_pContext->ShowWindow();
		else
			s_pContext->HideWindow();
	}

	inline void MainWindow::AddWidget(std::unique_ptr<Widget> &&pWidget) noexcept
	{ this->m_Widgets.emplace_back(std::move(pWidget)); }

	inline std::vector<std::unique_ptr<Widget>> &MainWindow::GetWidgets(void) noexcept
	{ return this->m_Widgets; }

	inline void MainWindow::Redisplay(void) const noexcept
	{
		s_pContext->SwitchWindow(this->m_hWin);
		s_pContext->Redisplay();
	}

	inline void MainWindow::Exec(void) const noexcept
	{
		//s_pContext->SwitchWindow(this->m_hWin);
		s_pContext->Exec();
	}
} /* disxx::ui */
