module;

#include <functional>
#include <algorithm>
#include <ranges>
#include <memory>
#include <vector>

// Make y starts at left bottom corner
#define TRANSLATE(y) \
	const auto [_, height]{backend::GLUTContext::GetWindowSize()}; \
	y = height - y

module disxx.ui.MainWindow;

namespace disxx::ui
{
	decltype(MainWindow::s_pContext) MainWindow::s_pContext
	{
		new
		#if defined(BACKEND_CTX_GLUT)
			backend::GLUTContext{}
		#else
		#	error "Context required"
		#endif
	};

	MainWindow::MainWindow(void) noexcept
		: m_Widgets{}
		, m_InitialSize{}
		, m_Size{}
		, m_hWin{0}
	{
		this->m_hWin = s_pContext->CreateWindow(utility::Vec2<int>{this->m_Size}, "");
		s_pContext->SwitchWindow(this->m_hWin);
		
		s_pContext->SetDisplayCallback
		(
			[this] -> void
			{ this->DisplayCallback(); }
		);
		s_pContext->SetReshapeCallback
		(
			[this](int width, int height) -> void
			{ this->ReshapeCallback(width, height); }
		);
		s_pContext->SetKeyboardCallback
		(
			[this](unsigned char key, int x, int y) -> void
			{ this->KeyboardCallback(key, x, y); }
		);
		s_pContext->SetMouseButtonCallback
		(
			[this](int button, int state, int x, int y) -> void
			{ this->MouseButtonCallback(button, state, x, y); }
		);
		s_pContext->SetMouseMotionCallback
		(
			[this](int x, int y) -> void
			{ this->MouseMotionCallback(x, y); }
		);
	}

	MainWindow::MainWindow(utility::Vec2<int> size, std::string_view title) noexcept
		: m_Widgets{}
		, m_InitialSize{utility::Vec2<int>{size}}
		, m_Size{size}
		, m_hWin{0}
	{
		this->m_hWin = s_pContext->CreateWindow(utility::Vec2<int>{this->m_Size}, title);
		s_pContext->SwitchWindow(this->m_hWin);

		s_pContext->SetDisplayCallback
		(
			[this] -> void
			{ this->DisplayCallback(); }
		);
		s_pContext->SetReshapeCallback
		(
			[this](int width, int height) -> void
			{ this->ReshapeCallback(width, height); }
		);
		s_pContext->SetKeyboardCallback
		(
			[this](unsigned char key, int x, int y) -> void
			{ this->KeyboardCallback(key, x, y); }
		);
		s_pContext->SetMouseButtonCallback
		(
			[this](int button, int state, int x, int y) -> void
			{ this->MouseButtonCallback(button, state, x, y); }
		);
		s_pContext->SetMouseMotionCallback
		(
			[this](int x, int y) -> void
			{ this->MouseMotionCallback(x, y); }
		);
	}

	MainWindow::MainWindow(const MainWindow &other) noexcept
		: m_Widgets{}
		, m_InitialSize{utility::Vec2<int>{other.m_InitialSize}}
		, m_Size{other.m_Size}
		, m_hWin{0}
	{
		for (const auto &pWidget : other.m_Widgets)
			this->m_Widgets.emplace_back(pWidget->Clone());

		this->m_hWin = s_pContext->CreateWindow(utility::Vec2<int>{this->m_Size}, "Copy");
		s_pContext->SwitchWindow(this->m_hWin);
	
		s_pContext->SetDisplayCallback
		(
			[this] -> void
			{ this->DisplayCallback(); }
		);
		s_pContext->SetReshapeCallback
		(
			[this](int width, int height) -> void
			{ this->ReshapeCallback(width, height); }
		);
		s_pContext->SetKeyboardCallback
		(
			[this](unsigned char key, int x, int y) -> void
			{ this->KeyboardCallback(key, x, y); }
		);
		s_pContext->SetMouseButtonCallback
		(
			[this](int button, int state, int x, int y) -> void
			{ this->MouseButtonCallback(button, state, x, y); }
		);
		s_pContext->SetMouseMotionCallback
		(
			[this](int x, int y) -> void
			{ this->MouseMotionCallback(x, y); }
		);
	}

	MainWindow &MainWindow::operator=(const MainWindow &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			for (const auto &pWidget : other.m_Widgets)
				this->m_Widgets.emplace_back(pWidget->Clone());
			this->m_InitialSize = other.m_InitialSize;
			this->m_Size = other.m_Size;
		}

		return *this;
	}

	MainWindow::MainWindow(MainWindow &&other) noexcept
		: m_Widgets{std::move(other.m_Widgets)}
		, m_InitialSize{std::move(utility::Vec2<int>{other.m_InitialSize})}
		, m_Size{std::move(other.m_Size)}
		, m_hWin{std::exchange(other.m_hWin, {})}
	{
		s_pContext->SwitchWindow(this->m_hWin);

		s_pContext->SetDisplayCallback
		(
			[this] -> void
			{ this->DisplayCallback(); }
		);
		s_pContext->SetReshapeCallback
		(
			[this](int width, int height) -> void
			{ this->ReshapeCallback(width, height); }
		);
		s_pContext->SetKeyboardCallback
		(
			[this](unsigned char key, int x, int y) -> void
			{ this->KeyboardCallback(key, x, y); }
		);
		s_pContext->SetMouseButtonCallback
		(
			[this](int button, int state, int x, int y) -> void
			{ this->MouseButtonCallback(button, state, x, y); }
		);
		s_pContext->SetMouseMotionCallback
		(
			[this](int x, int y) -> void
			{ this->MouseMotionCallback(x, y); }
		);
	}

	MainWindow &MainWindow::operator=(MainWindow &&other) noexcept
	{
		this->m_Widgets = std::move(other.m_Widgets);
		this->m_InitialSize = std::move(other.m_InitialSize);
		this->m_Size = std::move(other.m_Size);
		this->m_hWin = std::exchange(other.m_hWin, {});

		s_pContext->SwitchWindow(this->m_hWin);

		s_pContext->SetDisplayCallback
		(
			[this] -> void
			{ this->DisplayCallback(); }
		);
		s_pContext->SetReshapeCallback
		(
			[this](int width, int height) -> void
			{ this->ReshapeCallback(width, height); }
		);
		s_pContext->SetKeyboardCallback
		(
			[this](unsigned char key, int x, int y) -> void
			{ this->KeyboardCallback(key, x, y); }
		);
		s_pContext->SetMouseButtonCallback
		(
			[this](int button, int state, int x, int y) -> void
			{ this->MouseButtonCallback(button, state, x, y); }
		);
		s_pContext->SetMouseMotionCallback
		(
			[this](int x, int y) -> void
			{ this->MouseMotionCallback(x, y); }
		);

		return *this;
	}

	MainWindow::~MainWindow(void) noexcept
	{ s_pContext->DestroyWindow(this->m_hWin); }

	void MainWindow::DisplayCallback(void) const noexcept
	{
		s_pContext->SwitchWindow(this->m_hWin);

		for (const auto &pWidget : this->m_Widgets)
			pWidget->Render();
		s_pContext->SwapBuffers();
		Widget::ClearBuffer();
	}

	void MainWindow::ReshapeCallback(int width, int height) noexcept(false)
	{
		s_pContext->SwitchWindow(this->m_hWin);

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

		s_pContext->Redisplay();
	}

	void MainWindow::KeyboardCallback(unsigned char key, int x, int y) noexcept(false)
	{
		s_pContext->SwitchWindow(this->m_hWin);

		#ifdef BACKEND_CTX_GLUT
			TRANSLATE(y);
		#endif

		for (const auto i : std::views::iota(0ul, this->m_Widgets.size()))
			this->m_Widgets[i]->HandleKeyboard(key, x, y);
		s_pContext->Redisplay();
	}

	void MainWindow::MouseButtonCallback(int button, int state, int x, int y) noexcept(false)
	{
		s_pContext->SwitchWindow(this->m_hWin);

		#ifdef BACKEND_CTX_GLUT
			TRANSLATE(y);
		#endif

		for (const auto i : std::views::iota(0ul, this->m_Widgets.size()))
			this->m_Widgets[i]->HandleMouse(button, state, x, y);
		s_pContext->Redisplay();
	}

	void MainWindow::MouseMotionCallback(int x, int y) noexcept(false)
	{
		s_pContext->SwitchWindow(this->m_hWin);

		#ifdef BACKEND_CTX_GLUT
			TRANSLATE(y);
		#endif

		for (const auto i : std::views::iota(0ul, this->m_Widgets.size()))
			this->m_Widgets[i]->HandleMotion(x, y);
		s_pContext->Redisplay();
	}
} /* disxx::ui */
