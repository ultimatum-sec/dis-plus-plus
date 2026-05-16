module;

#include <string_view>
#include <vector>

export module disxx.ui.LabelBrowser;

import disxx.ui.Button;
import disxx.ui.Widget;

export namespace disxx::ui
{
	class LabelBrowser : public Widget
	{
	  private:
		float m_Scroll, m_MaxScroll;	
		float m_LastMouse, m_SliderHeight;
		std::vector<Button> m_Labels;

		void _CalcMaxScroll(void) noexcept;

	  public:
		explicit LabelBrowser(void) noexcept;
		explicit LabelBrowser(float, float, float, float) noexcept;
		LabelBrowser(const LabelBrowser &) noexcept;
		LabelBrowser &operator=(const LabelBrowser &) noexcept;

		virtual ~LabelBrowser(void) noexcept override = default;
	
		void AddLabel(std::string) noexcept(false);

		virtual void Resize(float, float) noexcept override;
		
		virtual void HandleMouse(int, int, int, int) noexcept override;
		virtual void HandleMotion(int, int) noexcept override;	
		virtual void Render(void) const noexcept override;
	};
} /* disxx::ui */







