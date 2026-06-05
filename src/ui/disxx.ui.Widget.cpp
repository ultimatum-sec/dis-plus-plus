module;

#include <functional>
#include <thread>
#include <array>
#include <tuple>

module disxx.ui.Widget;

import disxx.ui.backend.IRenderer;

namespace disxx::ui
{
	Widget::Widget(void) noexcept
		: m_Renderer{}
		, m_Callback
		{
			[]([[maybe_unused]] const Widget *const pWidget) -> void
			{ return; }
		}
		, m_X{0}
		, m_Y{0}
		, m_Width{0}
		, m_Height{0}
		, m_pColor{0.0f, 0.0f, 0.0f}
		, m_IsClicked{false}
        , m_IsHovered{false}
	{}

	Widget::Widget(float x, float y, float width, float height) noexcept
		: m_Renderer{}
		, m_Callback
		{
			[]([[maybe_unused]] const Widget *const pWidget) -> void
			{ return; }
		}
		, m_X{x}
		, m_Y{y}
		, m_Width{width}
		, m_Height{height}
		, m_pColor{0.0f, 0.0f, 0.0f}
		, m_IsClicked{false}
        , m_IsHovered{false}
	{}

	Widget::Widget(const Widget &other) noexcept
		: m_Renderer{}
		, m_Callback{other.m_Callback}
		, m_X{other.m_X}
		, m_Y{other.m_Y}
		, m_Width{other.m_Width}
        , m_Height{other.m_Height}
		, m_pColor{other.m_pColor[0], other.m_pColor[1], other.m_pColor[2]}
		, m_IsClicked{false}
        , m_IsHovered{other.m_IsHovered}	
	{}

	Widget &Widget::operator=(const Widget &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Callback = other.m_Callback;
			this->m_X = other.m_X;
			this->m_Y = other.m_Y;
			this->m_Width = other.m_Width;
			this->m_Height = other.m_Height;
			for (unsigned short int i{0}; i < 3; ++i)
				this->m_pColor[i] = other.m_pColor[i];
			this->m_IsClicked = other.m_IsClicked;
            this->m_IsHovered = other.m_IsHovered;	
		}

		return *this;
	}

	backend::IRenderer &Widget::GetRenderer(void) noexcept
	{ return this->m_Renderer; }

	void Widget::SetColor(float r, float g, float b) noexcept
	{
		this->m_pColor[0] = r;
		this->m_pColor[1] = g;
		this->m_pColor[2] = b;
	}

	//bool Widget::Clicked(void) const noexcept
	//{ return this->m_IsClicked; }

	//bool Widget::Hovered(void) const noexcept
	//{ return this->m_IsHovered; }

	std::tuple<float, float> Widget::GetCords(void) const noexcept
	{ return std::make_tuple(this->m_X, this->m_Y); }

	std::tuple<float, float> Widget::GetSizes(void) const noexcept
	{ return std::make_tuple(this->m_Width, this->m_Height); }

	void Widget::operator()(void) const
    { this->m_Callback(this); }

	void Widget::Replace(float x, float y) noexcept
	{
		this->m_X = x;
		this->m_Y = y;
	}

	void Widget::Resize(float width, float height) noexcept
	{
		this->m_Width = width;
		this->m_Height = height;
	}

	/*
	bool Widget::IsInField(float x, float y) const noexcept
	{
		return x >= this->m_X
			&& x <= this->m_X + this->m_Width
			&& y >= this->m_Y
			&& y <= this->m_Y + this->m_Height;
	}
	*/

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


