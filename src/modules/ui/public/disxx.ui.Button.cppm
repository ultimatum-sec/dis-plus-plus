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
		enum class ButtonTrigger
		{
			BTN_NONE,
			BTN_CLICKED,
			BTN_HOVERED
		};

	  private:
		std::function<void(const Widget *const)> m_Callback;
		ButtonTrigger m_Trigger;
		utility::Image m_Image;
		std::string m_Text;

	  public:
		explicit Button(void) noexcept;
        explicit Button(float, float, float, float) noexcept;
        Button(const Button &) noexcept;
        Button &operator=(const Button &) noexcept;

		virtual ~Button(void) noexcept override = default;

		void SetCallback(const enum ButtonTrigger, std::function<void(const Widget *const)>) noexcept;
		void SetImage(const std::filesystem::path &) noexcept(false);
		void SetText(std::string) noexcept(false);	

		virtual void HandleMouse(int, int, int, int) noexcept override;
		virtual void HandleMotion(int, int) noexcept override;
		virtual void Render(void) const noexcept override;
	};
} /* disxx::ui */







