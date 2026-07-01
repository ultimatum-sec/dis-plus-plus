module;

#include <string_view>
#include <functional>
#include <algorithm>
#include <memory>
#include <ranges>
#include <format>
#include <string>
#include <vector>

module disxx.ui.SourceEditor;

import disxx.ui.backend.GLUTContext;
import disxx.ui.backend.GLRenderer;
import disxx.ui.utility.ColorTag;
import disxx.ui.utility.Shape;
import disxx.ui.utility.Text;
import disxx.ui.utility.Vec;

namespace
{
	constexpr auto CHAR_WIDTH = 15ul;
	constexpr auto CHAR_HEIGHT = 9ul;

	constexpr auto CORNER_WIDTH = 20.f;
	constexpr auto CORNER_HEIGHT = 20.f;

	constexpr auto SKIP_PER_SCROLL = 3ul;
} /* */

namespace disxx::ui
{
	SourceEditor::SourceEditor(void) noexcept
		: Widget{}
		, m_Lines{}
		, m_LastMouseX{0.f}
		, m_LastMouseY{0.f}
		, m_ScrollX{0.f}
		, m_ScrollY{0.f}
		, m_MaxScrollX{0.f}
		, m_MaxScrollY{0.f}
		, m_VerticalSliderHeight{0.f}
		, m_HorizontalSliderWidth{0.f}
		, m_IsActiveVertical{false}
		, m_IsActiveHorizontal{false}
	{
		this->m_Lines.emplace_back("");
		this->_CalcMaxScroll();
	}

	SourceEditor::SourceEditor(float x, float y, float width, float height) noexcept
		: Widget{x, y, width, height}
       	, m_Lines{} 
		, m_LastMouseX{0.f}
        , m_LastMouseY{0.f}
		, m_ScrollX{0.f}
        , m_ScrollY{0.f}
        , m_MaxScrollX{0.f}
        , m_MaxScrollY{0.f}
        , m_VerticalSliderHeight{0.f}
        , m_HorizontalSliderWidth{0.f}
		, m_IsActiveVertical{false}
		, m_IsActiveHorizontal{false}
	{
		this->m_Lines.emplace_back("");
		this->_CalcMaxScroll();
	}

	SourceEditor::SourceEditor(const SourceEditor &other) noexcept
		: Widget{other}
		, m_Lines{other.m_Lines}
        , m_LastMouseX{other.m_LastMouseX}
        , m_LastMouseY{other.m_LastMouseY}
		, m_ScrollX{other.m_ScrollX}
		, m_ScrollY{other.m_ScrollY}
        , m_MaxScrollX{other.m_MaxScrollX}
        , m_MaxScrollY{other.m_MaxScrollY}
        , m_VerticalSliderHeight{other.m_VerticalSliderHeight}
        , m_HorizontalSliderWidth{other.m_HorizontalSliderWidth}
		, m_IsActiveVertical{other.m_IsActiveVertical}
		, m_IsActiveHorizontal{other.m_IsActiveHorizontal}
	{}

	SourceEditor &SourceEditor::operator=(const SourceEditor &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			Widget::operator=(other);
			this->m_Lines = other.m_Lines;
			this->m_LastMouseX = other.m_LastMouseX;
			this->m_LastMouseY = other.m_LastMouseY;
			this->m_ScrollX = other.m_ScrollX;
			this->m_ScrollY = other.m_ScrollY;	
			this->m_MaxScrollX = other.m_MaxScrollX;
			this->m_MaxScrollY = other.m_MaxScrollY;	
			this->m_VerticalSliderHeight = other.m_VerticalSliderHeight;
			this->m_HorizontalSliderWidth = other.m_HorizontalSliderWidth;
			this->m_IsActiveVertical = other.m_IsActiveVertical;
			this->m_IsActiveHorizontal = other.m_IsActiveHorizontal;
		}

		return *this;
	}

	void SourceEditor::_CalcMaxScroll(void) noexcept
	{
		this->m_MaxScrollY = std::max(0.f, static_cast<float>(this->m_Lines.size() * CHAR_WIDTH - (this->m_Size.y - CORNER_HEIGHT) + 5));
		this->m_ScrollY = std::max(0.f, std::min(this->m_ScrollY, this->m_MaxScrollY));

		this->m_MaxScrollX = 0.f;
		for (const auto &line : this->m_Lines)
			if (auto realText{utility::ColorTag{}.RemoveTags(line)}; realText.size() * CHAR_HEIGHT + 5 > this->m_MaxScrollX)
				this->m_MaxScrollX = std::max(0.f, realText.size() * CHAR_HEIGHT + 5 - (this->m_Size.x - CORNER_WIDTH));

		this->m_VerticalSliderHeight = (this->m_Size.y - CORNER_HEIGHT) * ((this->m_Size.y - CORNER_HEIGHT)
			/ static_cast<float>(this->m_Lines.size() * CHAR_WIDTH));
		this->m_VerticalSliderHeight = std::max(30.f, std::min(this->m_VerticalSliderHeight, this->m_Size.y - CORNER_HEIGHT));

		this->m_HorizontalSliderWidth = (this->m_Size.x - CORNER_WIDTH) * ((this->m_Size.x - CORNER_WIDTH) / (this->m_MaxScrollX + (this->m_Size.x - CORNER_WIDTH)));
		this->m_HorizontalSliderWidth = std::max(30.f, std::min(this->m_HorizontalSliderWidth, this->m_Size.x - CORNER_WIDTH));
	}

	void SourceEditor::Resize(utility::Vec2<float> size) noexcept
	{
		Widget::Resize(utility::Vec2<float>{size});
		this->_CalcMaxScroll();
	}

	void SourceEditor::HandleMouse(int button, int state, int x, int y) noexcept
	{
		#ifdef BACKEND_CTX_GLUT
			y = backend::GLUTContext::GetWindowSize().y - y;
		#endif

		// Mouse clicked	
		if (button == 0 && state == 0)
		{
			// The verticall scrollbar has been dragged
			if (x >= this->m_Size.x - CORNER_WIDTH && x < this->m_Size.x && y > CORNER_HEIGHT && y < this->m_Size.y)
			{
				this->m_IsActiveVertical = true;
				this->m_LastMouseY = y;
			}
			// The horizontal scrollbar has been dragged
			else if (x >= 0 && x < this->m_Size.x - CORNER_WIDTH && y >= 0)
			{
				this->m_IsActiveHorizontal = true;
				this->m_LastMouseX = x;
			}
		}
		// Mouse released
		else if (button == 0 && state == 1)
		{
			this->m_IsActiveVertical = false;
			this->m_IsActiveHorizontal = false;
		}
		// Mouse wheel
		else if (button == 3 || button == 4)
		{
			this->m_ScrollY += CHAR_HEIGHT * SKIP_PER_SCROLL * (button == 3 ? 1 : -1);
			this->m_ScrollY = std::max(0.f, std::min(this->m_ScrollY,this->m_MaxScrollY));
		}
	}

	void SourceEditor::HandleMotion(int x, int y) noexcept
	{
		//#ifdef BACKEND_CTX_GLUT
		//	y = backend::GLUTContext::GetWindowSize().y - y;
		//#endif

		if (this->m_IsActiveVertical)
		{
			float delta{y - this->m_LastMouseY};
			this->m_LastMouseY = y;

			this->m_ScrollY += delta * (this->m_MaxScrollY / (this->m_Size.y - CORNER_HEIGHT));
			this->m_ScrollY = std::max(0.f, std::min(this->m_ScrollY, this->m_MaxScrollY));
		}
		else if (this->m_IsActiveHorizontal)
		{
			float delta{x - this->m_LastMouseX};
            this->m_LastMouseX = x;

  	        this->m_ScrollX += delta * (this->m_MaxScrollX / (this->m_Size.x - CORNER_WIDTH));
			this->m_ScrollX = std::max(0.f, std::min(this->m_ScrollX, this->m_MaxScrollX));
		}
	}
	
	void SourceEditor::Render(void) const noexcept
	{
		// Render the text area
		utility::ColorTag tag{};
        for (const auto i : std::views::iota(0UL, this->m_Lines.size()))
        {
            if (float lineY{this->m_Size.y - CORNER_HEIGHT - i * CHAR_WIDTH + this->m_ScrollY}; (lineY < 0.f) || (lineY > this->m_Size.y - CORNER_HEIGHT))
                continue;

			// The line without color tags
            const auto &realText{tag.RemoveTags(this->m_Lines[i])};
            for (auto pos{0ul}; const auto &[text, color] : tag.Parse(this->m_Lines[i]).value())
            {
				// Indexes to walk through the line
                const auto renderStart{std::max(pos, static_cast<unsigned long>(std::max(0.f, this->m_ScrollX / CHAR_HEIGHT)))};
                const auto renderEnd
				{
					std::min
					(
						pos + text.size(),
						std::min
						(
							realText.size(),
							renderStart + static_cast<unsigned long>((this->m_Size.x - CORNER_WIDTH) / CHAR_HEIGHT)
						)
					)
				};

				utility::Text txt{};
				txt.Replace
				(
					utility::Vec2<float>
					{
						std::max(1.f, 5.f - this->m_ScrollX + static_cast<float>(renderStart) * CHAR_HEIGHT),
						this->m_Position.y + this->m_Size.y - i * CHAR_WIDTH + this->m_ScrollY
					}
				);
				txt.SetColor
				(
					utility::Vec3<float>
					{
						color.at(0),
						color.at(1),
						color.at(2)
					}
				);
       		
				std::string str{};
				if (renderStart < renderEnd)
					for (const auto j : std::views::iota(renderStart, renderEnd))
           				str += realText.at(j);
				txt.SetText(str);
                s_pRenderer->Push(std::make_unique<utility::Text>(txt));

				pos += text.size();
        	}
        }
	
		// Render the vertical scrollbar
		if (this->m_MaxScrollY > 0)
		{
			float pos{(this->m_ScrollY / this->m_MaxScrollY) * (this->m_Size.y - CORNER_HEIGHT - this->m_VerticalSliderHeight)};
			pos = std::max(0.f, std::min(pos, this->m_Size.y - CORNER_HEIGHT - this->m_VerticalSliderHeight));

			// Frame
			utility::Shape frame{utility::Shape::Type::TYPE_RECTANGLE};
			frame.Replace(utility::Vec2<float>{this->m_Position.x + this->m_Size.x - CORNER_WIDTH - 1.f, this->m_Position.y + this->m_Size.y - this->m_VerticalSliderHeight - pos - 1.f});
			frame.Resize(utility::Vec2<float>{CORNER_WIDTH + 2.f, this->m_VerticalSliderHeight + 2.f});
			frame.SetColor(utility::Vec3<float>{0.5f, 0.5f, 0.5f});
			s_pRenderer->Push(std::make_unique<utility::Shape>(frame));

			// Scrollbar itself
			utility::Shape vScrollbar{utility::Shape::Type::TYPE_RECTANGLE};
			vScrollbar.Replace(utility::Vec2<float>{this->m_Position.x + this->m_Size.x - CORNER_WIDTH, this->m_Position.y + this->m_Size.y - this->m_VerticalSliderHeight - pos});
			vScrollbar.Resize(utility::Vec2<float>{CORNER_WIDTH, this->m_VerticalSliderHeight});
			vScrollbar.SetColor(utility::Vec3<float>{0.3f, 0.3f, 0.3f});
			s_pRenderer->Push(std::make_unique<utility::Shape>(vScrollbar));
		}

		// Render the horizontal scrollbar
		if (this->m_MaxScrollX > 0)
		{
			float pos{(this->m_ScrollX / this->m_MaxScrollX) * (this->m_Size.x - CORNER_WIDTH - this->m_HorizontalSliderWidth)};
			pos = std::max(0.f, std::min(pos, this->m_Size.x - CORNER_WIDTH - this->m_HorizontalSliderWidth));

			// Frame
			utility::Shape frame{utility::Shape::Type::TYPE_RECTANGLE};
			frame.Replace(utility::Vec2<float>{this->m_Position.x + pos - 1.f, this->m_Position.y - 1.f});
			frame.Resize(utility::Vec2<float>{this->m_HorizontalSliderWidth + 2.f, CORNER_HEIGHT + 2.f});
			frame.SetColor(utility::Vec3<float>{0.5f, 0.5f, 0.5f});
			s_pRenderer->Push(std::make_unique<utility::Shape>(frame));

			// Scrollbar itself
			utility::Shape hScrollbar{utility::Shape::Type::TYPE_RECTANGLE};
			hScrollbar.Replace(utility::Vec2<float>{this->m_Position.x + pos, this->m_Position.y});
			hScrollbar.Resize(utility::Vec2<float>{this->m_HorizontalSliderWidth, CORNER_HEIGHT});
			hScrollbar.SetColor(utility::Vec3<float>{0.3f, 0.3f, 0.3f});
			s_pRenderer->Push(std::make_unique<utility::Shape>(hScrollbar));
		}

		this->s_pRenderer->Render();
	}
} /* disxx::ui */
