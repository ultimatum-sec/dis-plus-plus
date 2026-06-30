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

		utility::Vec2<float> GetPosition(void) const noexcept;
		utility::Vec2<float> GetSize(void) const noexcept;
	
		virtual void Replace(utility::Vec2<float>) noexcept;
		virtual void Resize(utility::Vec2<float>) noexcept;
		
		void SetColor(float, float, float) noexcept;

		virtual void HandleKeyboard(unsigned char, int, int);
		virtual void HandleMouse(int, int, int, int);
		virtual void HandleMotion(int, int);
		virtual void Render(void) const noexcept = 0;
	};
} /* disxx::ui */
