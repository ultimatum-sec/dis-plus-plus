module;

#include <string_view>
#include <utility>
#include <format>
#include <string>
#include <vector>
#include <regex>

export module disxx.ui.SourceEditor;

import disxx.ui.Widget;

export namespace disxx::ui
{
	class SourceEditor final : public Widget
	{
	  private:
		std::vector<std::string> m_Lines;
		
		float m_LastMouseX{}, m_LastMouseY{};
		float m_ScrollX{}, m_ScrollY{};	
		float m_MaxScrollX{}, m_MaxScrollY{};
		float m_VerticalSliderHeight{}, m_HorizontalSliderWidth{};
		mutable bool m_IsActiveVertical{false}, m_IsActiveHorizontal{false};

	  private:
		void _CalcMaxScroll(void) noexcept;

	  public:
		explicit SourceEditor(void) noexcept;
		explicit SourceEditor(float, float, float, float) noexcept;
		SourceEditor(const SourceEditor &) noexcept;	
		SourceEditor &operator=(const SourceEditor &) noexcept;

		virtual ~SourceEditor(void) noexcept override = default;

		virtual void Resize(utility::Vec2<float>) noexcept override;

		template <typename ...Args> inline void AddString(std::format_string<Args...>, Args &&...) noexcept(false);
		template <typename ...Args> inline void AddLine(std::format_string<Args...>, Args &&...) noexcept(false);
		inline void ClearText(void) noexcept;

		virtual void HandleMouse(int, int, int, int) noexcept override;
		virtual void HandleMotion(int, int) noexcept override;
		virtual void Render(void) const noexcept override;
	};

	template <typename ...Args>
	inline void SourceEditor::AddString(std::format_string<Args...> fmt, Args &&...args) noexcept(false)
	{
		// Change '\t' --> "    "
		*this->m_Lines.rbegin() += std::regex_replace
		(
			std::format(fmt, std::forward<Args>(args)...),
			std::regex{R"(\t)"},
			"    "
		);
	}

	template <typename ...Args>
	inline void SourceEditor::AddLine(std::format_string<Args...> fmt, Args &&...args) noexcept(false)
    {
		// Change '\t' --> "    "
		this->m_Lines.emplace_back
		(
			std::regex_replace
			(
				std::format(fmt, std::forward<Args>(args)...),
				std::regex{R"(\t)"},
				"    "
			)
		);
     }

	inline void SourceEditor::ClearText(void) noexcept
	{
		if (!this->m_Lines.empty()) [[likely]]
			this->m_Lines.clear();
	}
} /* disxx::ui */
