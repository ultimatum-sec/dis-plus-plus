module;

#include <disconf.hpp>

#include <type_traits>
#include <functional>
#include <memory>
#include <tuple>

export module disxx.ui.Widget;

import disxx.utility.wrapper.Pointer;
import disxx.ui.backend.IRenderer;
import disxx.ui.backend.GLRenderer;

export namespace disxx::ui
{
	class __DISXX_EXPORT__ [[nodiscard]] Widget
	{
	  protected:
		static disxx::utility::wrapper::Pointer<std::shared_ptr<backend::IRenderer>> s_pRenderer;

	  protected:
		float m_X, m_Y;
		float m_Width, m_Height;
		float m_pColor[3];
		[[maybe_unused]] mutable bool m_IsClicked, m_IsHovered;

	  public:
		static void ClearBuffer(void) noexcept;
		static void SwapBuffers(void) noexcept;
		static void Redisplay(void) noexcept;
	
	  public:
		explicit Widget(void) noexcept;
		explicit Widget(float, float, float, float) noexcept;
		Widget(const Widget &) noexcept;
		Widget &operator=(const Widget &) noexcept;

		virtual ~Widget(void) noexcept = default;

		std::tuple<float, float> GetCords(void) const noexcept;
		std::tuple<float, float> GetSizes(void) const noexcept;
	
		virtual void Replace(float, float) noexcept;
		virtual void Resize(float, float) noexcept;
		
		void SetColor(float, float, float) noexcept;

		virtual void HandleKeyboard(unsigned char, int, int);
		virtual void HandleMouse(int, int, int, int);
		virtual void HandleMotion(int, int);
		
		virtual void Render(void) const noexcept = 0;
	};
} /* disxx::ui */
