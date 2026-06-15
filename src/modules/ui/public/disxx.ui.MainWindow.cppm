module;

#include <type_traits>
#include <vector>
#include <memory>

export module disxx.ui.MainWindow;

export import disxx.ui.utility.Vec;

import disxx.utility.wrapper.Pointer;
import disxx.ui.backend.GLUTContext;
import disxx.ui.backend.IContext;
import disxx.ui.Widget;

export namespace disxx::ui
{
	class MainWindow
	{
	  private:
		static disxx::utility::wrapper::Pointer
		<
			std::shared_ptr
			<
				#if defined(BACKEND_CTX_GLUT)
					disxx::ui::backend::GLUTContext
				#endif
			>
		> s_pContext;

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
		#if defined(BACKEND_CTX_GLUT)
			using WindowHandle = int;
		#endif

	  public:
		template <typename ...Args>
		static void Init(Args &&...) noexcept;

		static inline WindowHandle CreateWindow(utility::Vec2<int>, std::string_view) noexcept;
		static inline void SwitchWindow(WindowHandle &) noexcept;
		static inline void DestroyWindow(void) noexcept;

		template <typename F>
		static inline void SetDisplayCallback(F) noexcept;
		template <typename F>
		static inline void SetReshapeCallback(F) noexcept;
		template <typename F>
		static inline void SetKeyboardCallback(F) noexcept;
		template <typename F>
		static inline void SetMouseButtonCallback(F) noexcept;
		template <typename F>
		static inline void SetMouseMotionCallback(F) noexcept;
		
		static inline void SwapBuffers(void) noexcept;
		static inline void Redisplay(void) noexcept;
	
		static inline void ExecLoop(void) noexcept;

	  public:
		virtual ~MainWindow(void) noexcept = default;
	};

	template <typename ...Args>
	void MainWindow::Init(Args &&...args) noexcept
	{
		#if defined(BACKEND_CTX_GLUT)
			backend::GLUTContext::Init(std::forward<Args>(args)...);
		#endif
	}

	inline MainWindow::WindowHandle MainWindow::CreateWindow(utility::Vec2<int> size, std::string_view title) noexcept
	{ return s_pContext->CreateWindow(utility::Vec2<int>{size}, title); }

	inline void MainWindow::SwitchWindow(WindowHandle &hWin) noexcept
	{ s_pContext->SwitchWindow(hWin); }

	inline void MainWindow::DestroyWindow(void) noexcept
	{ s_pContext->DestroyWindow(); }

	template <typename F>
	inline void MainWindow::SetDisplayCallback(F pCallback) noexcept
	{ s_pContext->SetDisplayCallback(pCallback); }

	template <typename F>
	inline void MainWindow::SetReshapeCallback(F pCallback) noexcept
	{ s_pContext->SetReshapeCallback(pCallback); }

	template <typename F>
	inline void MainWindow::SetKeyboardCallback(F pCallback) noexcept
	{ s_pContext->SetKeyboardCallback(pCallback); }

	template <typename F>
	inline void MainWindow::SetMouseButtonCallback(F pCallback) noexcept
	{ s_pContext->SetMouseButtonCallback(pCallback); }
	
	template <typename F>
	inline void MainWindow::SetMouseMotionCallback(F pCallback) noexcept
	{ s_pContext->SetMouseMotionCallback(pCallback); }

	inline void MainWindow::SwapBuffers(void) noexcept
	{ s_pContext->SwapBuffers(); }

	inline void MainWindow::Redisplay(void) noexcept
	{ s_pContext->Redisplay(); }

	inline void MainWindow::ExecLoop(void) noexcept
	{ s_pContext->ExecLoop(); }
} /* disxx::ui */
