module;

#include <memory>
#include <vector>
#include <bit>

// Make y starts at left bottom corner
#define TRANSLATE(y) \
	const auto [_, height]{backend::GLUTContext::GetWindowSize()}; \
	y = height - y

module disxx.ui.MainWindow;

namespace disxx::ui
{
	std::unique_ptr<MainWindow> MainWindow::s_pInstance{nullptr};

	MainWindow::MainWindow(utility::Vec2<int> size, std::string_view title) noexcept
		: m_Context{}
		, m_Widgets{}
		, m_InitialSize{utility::Vec2<int>{size}}
		, m_Size{size}
		, m_hWin{0}
	{
		this->m_hWin = this->m_Context.CreateWindow(utility::Vec2<int>{this->m_Size}, title);
		this->m_Context.SwitchWindow(this->m_hWin);
		this->m_Context.SetDisplayCallback(std::bit_cast<void *>(+[] -> void { s_pInstance->DisplayCallback(); }));
		this->m_Context.SetReshapeCallback(std::bit_cast<void *>(+[](int x, int y) -> void { s_pInstance->ReshapeCallback(x, y); }));
		this->m_Context.SetKeyboardCallback(std::bit_cast<void *>(+[](unsigned char key, int x, int y) -> void { s_pInstance->KeyboardCallback(key, x, y); }));
		this->m_Context.SetMouseButtonCallback(std::bit_cast<void *>(+[](int button, int state, int x, int y) -> void { s_pInstance->MouseButtonCallback(button, state, x, y); }));
		this->m_Context.SetMouseMotionCallback(std::bit_cast<void *>(+[](int x, int y) -> void { s_pInstance->MouseMotionCallback(x, y); }));
	}

	MainWindow::~MainWindow(void) noexcept
	{
		if (s_pInstance) [[likely]]
			s_pInstance.release();
		s_pInstance = nullptr;
	}

	void MainWindow::DisplayCallback(void) const noexcept
	{
		for (const auto &pWidget : this->m_Widgets)
			pWidget->Render();
		this->m_Context.SwapBuffers();
		Widget::ClearBuffer();
	}

	void MainWindow::ReshapeCallback(int width, int height) noexcept(false)
	{
		auto sX{static_cast<float>(this->m_Size.x / this->m_InitialSize.x)};
		auto sY{static_cast<float>(this->m_Size.y / this->m_InitialSize.y)};
		
		this->m_Size = utility::Vec2<int>{width, height};

		for (const auto &pWidget : this->m_Widgets)
		{
			const auto [x, y]{pWidget->GetPosition()};
			const auto [w, h]{pWidget->GetSize()};

			pWidget->Replace(utility::Vec2<float>{x * sX, y * sY});
			pWidget->Resize(utility::Vec2<float>{w * sX, h * sY});
		}

		this->m_Context.Redisplay();
	}

	void MainWindow::KeyboardCallback(unsigned char key, int x, int y) noexcept(false)
	{
		#ifdef BACKEND_CTX_GLUT
			TRANSLATE(y);
		#endif

		for (const auto &pWidget : this->m_Widgets)
			pWidget->HandleKeyboard(key, x, y);
		this->m_Context.Redisplay();
	}

	void MainWindow::MouseButtonCallback(int button, int state, int x, int y) noexcept(false)
	{
		#ifdef BACKEND_CTX_GLUT
			TRANSLATE(y);
		#endif

		for (const auto &pWidget : this->m_Widgets)
			pWidget->HandleMouse(button, state, x, y);
		this->m_Context.Redisplay();
	}

	void MainWindow::MouseMotionCallback(int x, int y) noexcept(false)
	{
		#ifdef BACKEND_CTX_GLUT
			TRANSLATE(y);
		#endif

		for (const auto &pWidget : this->m_Widgets)
			pWidget->HandleMotion(x, y);
		this->m_Context.Redisplay();
	}

	std::unique_ptr<MainWindow> &MainWindow::Init(utility::Vec2<int> size, std::string_view title) noexcept
	{
		if (!s_pInstance) [[likely]]
			s_pInstance = std::unique_ptr<MainWindow>{new MainWindow{utility::Vec2<int>{size}, title}};
		return s_pInstance;
	}
} /* disxx::ui */
