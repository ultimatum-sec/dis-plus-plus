module;

#include <disconf.hpp>

#include <type_traits>
#include <functional>
#include <memory>
#include <tuple>

export module disxx.ui.Widget;

import disxx.utility.pointer.NonNull;
import disxx.ui.backend.IRenderer;
import disxx.ui.backend.GLRenderer;

export import disxx.ui.utility.Vec;

export namespace disxx::ui
{
	class __DISXX_EXPORT__ [[nodiscard]] Widget
	{
	  protected:
		static disxx::utility::pointer::NonNull<backend::IRenderer> s_pRenderer;

	  protected:
		utility::Vec2<float> m_Position{};
		utility::Vec2<float> m_Size{};
		float m_pColor[3]{};
		[[maybe_unused]] mutable bool
			m_IsClicked{},
			m_IsHovered{};

	  public:
		static void ClearBuffers(void) noexcept;
		static void SwapBuffers(void) noexcept;
		static void Redisplay(void) noexcept;
	
	  public:
		explicit Widget(void) noexcept;
		explicit Widget(float, float, float, float) noexcept;
		Widget(const Widget &) noexcept;
		Widget &operator=(const Widget &) noexcept;

		virtual ~Widget(void) noexcept = default;

		inline utility::Vec2<float> GetPosition(void) const noexcept;
		inline utility::Vec2<float> GetSize(void) const noexcept;
	
		virtual void Replace(utility::Vec2<float>) noexcept;
		virtual void Resize(utility::Vec2<float>) noexcept;
		
		inline void SetColor(float, float, float) noexcept;

		inline bool Clicked(void) const noexcept;

		virtual void Render(void) const noexcept = 0;
		virtual void HandleKeyboard(unsigned char, int, int);
		virtual void HandleMouse(int, int, int, int);
		virtual void HandleMotion(int, int);
	};

	inline void Widget::SetColor(float r, float g, float b) noexcept
	{
		this->m_pColor[0] = r;
		this->m_pColor[1] = g;
		this->m_pColor[2] = b;
	}

	inline utility::Vec2<float> Widget::GetPosition(void) const noexcept
	{ return utility::Vec2<float>{this->m_Position}; }

	inline utility::Vec2<float> Widget::GetSize(void) const noexcept
	{ return utility::Vec2<float>{this->m_Size}; }

	inline void Widget::Replace(utility::Vec2<float> position) noexcept
	{ this->m_Position = position; }

	inline void Widget::Resize(utility::Vec2<float> size) noexcept
	{ this->m_Size = size; }

	inline bool Widget::Clicked(void) const noexcept
	{ return this->m_IsClicked; }
} /* disxx::ui */
