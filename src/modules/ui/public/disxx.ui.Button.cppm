module;

#include <filesystem>
#include <functional>
#include <string>

export module disxx.ui.Button;

import disxx.ui.utility.ImageLoader;
import disxx.ui.Widget;

export namespace disxx::ui
{
	class Button final : public Widget
	{
	  public:
		enum class Trigger
		{
			BTN_NONE,
			BTN_CLICKED,
			BTN_HOVERED
		};

	  private:
		std::function<void(const Widget *const)> m_Callback{};
		std::string m_Text{};
		Trigger m_Trigger{};

	  public:
		explicit Button(void) noexcept;
        explicit Button(float, float, float, float) noexcept;
        
		Button(const Button &) noexcept;
        Button &operator=(const Button &) noexcept;

		Button(Button &&) noexcept;
		Button &operator=(Button &&) noexcept;

		virtual ~Button(void) noexcept override = default;

		inline void SetCallback(const Trigger, std::function<void(const Widget *const)>) noexcept;
		inline void SetText(std::string_view) noexcept;	

		virtual void HandleMouse(int, int, int, int) noexcept override;
		virtual void HandleMotion(int, int) noexcept override;
		virtual void Render(void) const noexcept override;
	};

	inline void Button::SetCallback(const Trigger trigger, std::function<void(const Widget *const)> callback) noexcept
	{
		if (trigger != Trigger::BTN_NONE)
		{
			this->m_Callback = callback;
			this->m_Trigger = trigger;
		}
		else
		{
			// Don't touch the callback here!
			// It won't be used anyway...
			this->m_Trigger = Trigger::BTN_NONE;
		}
	}

	inline void Button::SetText(std::string_view str) noexcept
	{ this->m_Text = str.data(); }
} /* disxx::ui */
