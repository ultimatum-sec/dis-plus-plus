module;

#include <string_view>
#include <functional>
#include <string>

export module disxx.ui.TextInput;

import disxx.ui.Widget;

export namespace disxx::ui
{
	class TextInput final : public Widget
	{
	  private:
		std::function<void(const Widget *const)> m_Callback{};
		std::string m_Text{};

	  public:
		explicit TextInput(void) noexcept;
		explicit TextInput(float, float, float, float) noexcept;
		TextInput(const TextInput &) noexcept;
		TextInput &operator=(const TextInput &) noexcept;

		virtual ~TextInput(void) noexcept override = default;

		void SetText(std::string_view) noexcept;
		std::string_view GetText(void) const noexcept;

		virtual void HandleKeyboard(unsigned char, int, int) override;
		virtual void HandleMouse(int, int, int, int) override;
		virtual void Render(void) const noexcept override;
	};
} // disxx::ui
