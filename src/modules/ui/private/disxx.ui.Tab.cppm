module;

#include <disconf.hpp>

#include <utility>
#include <string>

export module disxx.ui.Tab;

export import <string_view>;
export import <memory>;

export import disxx.ui.SourceEditor;
import disxx.ui.Widget;

export namespace disxx::ui
{
	class __DISXX_EXPORT__ [[nodiscard]] Tab final : public Widget
	{
	  private:
		SourceEditor m_TextArea{};
		std::string m_Text{};

	  public:
		explicit Tab(void) noexcept;
		explicit Tab(float, float, float, float) noexcept;

		Tab(const Tab &) noexcept;
		Tab &operator=(const Tab &) noexcept;

		Tab(Tab &&) noexcept;
		Tab &operator=(Tab &&) noexcept;

		inline void SetTextArea(SourceEditor &&) noexcept;
		inline void SetText(std::string_view) noexcept;

		inline const SourceEditor &GetTextArea(void) const noexcept;
		inline SourceEditor &GetTextArea(void) noexcept;
		inline std::string_view GetText(void) const noexcept;

		inline void SetPassive(void) noexcept;

		virtual std::unique_ptr<Widget> Clone(void) const noexcept override;

		virtual void Render(void) const noexcept override;
		virtual void HandleMouse(int, int, int, int) noexcept(false) override;
		virtual void HandleMotion(int, int) noexcept(false) override;
	};

	inline void Tab::SetTextArea(SourceEditor &&textArea) noexcept
	{ this->m_TextArea = std::forward<SourceEditor &&>(textArea); }

	inline void Tab::SetText(std::string_view str) noexcept
	{ this->m_Text = str.data(); }

	inline const SourceEditor &Tab::GetTextArea(void) const noexcept
	{ return this->m_TextArea; }

	inline SourceEditor &Tab::GetTextArea(void) noexcept
	{ return this->m_TextArea; }

	inline std::string_view Tab::GetText(void) const noexcept
	{ return this->m_Text; }

	inline void Tab::SetPassive(void) noexcept
	{ this->m_IsClicked = false; }
} /* disxx::ui */
