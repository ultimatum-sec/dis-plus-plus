module;

#include <functional>
#include <thread>
#include <array>
#include <tuple>

#define INIT_GL_RENDERER(ptr) \
	static bool first{true}; \
	if (first) \
		ptr = new backend::GLRenderer{}; \
	first = false

module disxx.ui.Widget;

import disxx.ui.backend.IRenderer;

namespace disxx::ui
{
	disxx::utility::pointer::NonNull<backend::IRenderer> Widget::s_pRenderer{};

	void Widget::ClearBuffers(void) noexcept
	{ s_pRenderer->ClearBuffers(); }

	Widget::Widget(void) noexcept
		: m_Position{0, 0}
		, m_Size{0, 0}
		, m_pColor{0.0f, 0.0f, 0.0f}
		, m_IsClicked{false}
        , m_IsHovered{false}
	{ INIT_GL_RENDERER(s_pRenderer); }

	Widget::Widget(float x, float y, float width, float height) noexcept
		: m_Position{x, y}
		, m_Size{width, height}
		, m_pColor{0.0f, 0.0f, 0.0f}
		, m_IsClicked{false}
        , m_IsHovered{false}
	{ INIT_GL_RENDERER(s_pRenderer); }

	Widget::Widget(const Widget &other) noexcept
		: m_Position{other.m_Position}
		, m_Size{other.m_Size}
		, m_pColor{other.m_pColor[0], other.m_pColor[1], other.m_pColor[2]}
		, m_IsClicked{false}
        , m_IsHovered{other.m_IsHovered}	
	{}

	Widget &Widget::operator=(const Widget &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Position = other.m_Position;
			this->m_Size = other.m_Size;
			for (unsigned short int i{0}; i < 3; ++i)
				this->m_pColor[i] = other.m_pColor[i];
			this->m_IsClicked = other.m_IsClicked;
            this->m_IsHovered = other.m_IsHovered;	
		}

		return *this;
	}

	void Widget::SetColor(float r, float g, float b) noexcept
	{
		this->m_pColor[0] = r;
		this->m_pColor[1] = g;
		this->m_pColor[2] = b;
	}

	utility::Vec2<float> Widget::GetPosition(void) const noexcept
	{ return utility::Vec2<float>{this->m_Position}; }

	utility::Vec2<float> Widget::GetSize(void) const noexcept
	{ return utility::Vec2<float>{this->m_Size}; }

	void Widget::Replace(utility::Vec2<float> position) noexcept
	{ this->m_Position = position; }

	void Widget::Resize(utility::Vec2<float> size) noexcept
	{ this->m_Size = size; }

	/*
	 * Handle Nothing.
	 */

	void Widget::HandleKeyboard(unsigned char, int, int)
	{ return; }

	void Widget::HandleMouse(int, int, int, int)
    { return; }
	
	void Widget::HandleMotion(int, int)
	{ return; }
} /* disxx::ui */
