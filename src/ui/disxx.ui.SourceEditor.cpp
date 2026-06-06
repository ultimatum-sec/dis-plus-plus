module;

#ifdef __APPLE__
#   include <OpenGL/gl.h>
#   include <GLUT/glut.h>
#else
#   include <GL/freeglut.h>
#endif

#include <string_view>
#include <functional>
#include <algorithm>
#include <ranges>
#include <format>
#include <string>
#include <vector>

module disxx.ui.SourceEditor;

import disxx.ui.utility.ColorTag;
import disxx.ui.utility.Shape;

namespace
{
	constexpr auto CHAR_WIDTH = 15ul;
	constexpr auto CHAR_HEIGHT = 9ul;

	constexpr auto CORNER_WIDTH = 20.f;
	constexpr auto CORNER_HEIGHT = 20.f;

	//constexpr auto VSCROLLBAR_WIDTH = 5.f;
	//constexpr auto HSCROLLBAR_HEIGHT = 5.f;

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
		this->_CalcMaxScroll();
		this->m_Lines.emplace_back("");
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
		this->_CalcMaxScroll();
		this->m_Lines.emplace_back("");
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
		this->m_MaxScrollY = std::max(0.f, static_cast<float>(this->m_Lines.size() * CHAR_WIDTH - (this->m_Height - CORNER_HEIGHT) + 5));
		this->m_ScrollY = std::max(0.f, std::min(this->m_ScrollY, this->m_MaxScrollY));

		this->m_MaxScrollX = 0.f;
		for (const auto &line : this->m_Lines)
			if (auto realText{utility::ColorTag{}.RemoveTags(line)}; realText.size() * CHAR_HEIGHT + 5 > this->m_MaxScrollX)
					this->m_MaxScrollX = realText.size() * CHAR_HEIGHT + 5;

		this->m_VerticalSliderHeight = (this->m_Height - CORNER_HEIGHT) * ((this->m_Height - CORNER_HEIGHT)
			/ static_cast<float>(this->m_Lines.size() * CHAR_WIDTH));
		this->m_VerticalSliderHeight = std::max(30.f, std::min(this->m_VerticalSliderHeight, this->m_Height - CORNER_HEIGHT));

		this->m_HorizontalSliderWidth = (this->m_Width - CORNER_WIDTH) * ((this->m_Width - CORNER_WIDTH) / (this->m_MaxScrollX + (this->m_Width - CORNER_WIDTH)));
		this->m_HorizontalSliderWidth = std::max(30.f, std::min(this->m_HorizontalSliderWidth, this->m_Width - CORNER_WIDTH));
	}

	void SourceEditor::Resize(float width, float height) noexcept
	{
		Widget::Resize(width, height);
		this->_CalcMaxScroll();
	}

	void SourceEditor::HandleMouse(int button, int state, int x, int y) noexcept
	{
		x -= this->m_X;
        y = (glutGet(GLUT_WINDOW_HEIGHT) - y) - this->m_Y;
	
		// Mouse clicked	
		if (button == GLUT_LEFT && state == GLUT_DOWN)
		{
			// The verticall scrollbar has been dragged
			if (x >= this->m_Width - CORNER_WIDTH && x < this->m_Width && y > CORNER_HEIGHT && y < this->m_Height)
			{
				this->m_IsActiveVertical = true;
				this->m_LastMouseY = y;
			}
			// The horizontal scrollbar has been dragged
			else if (x >= 0 && x < this->m_Width - CORNER_WIDTH && y >= 0 && y < CORNER_HEIGHT)
			{
				this->m_IsActiveHorizontal = true;
				this->m_LastMouseX = x;
			}
		}
		// Mouse released
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			this->m_IsActiveVertical = false;
			this->m_IsActiveHorizontal = false;
		}
		// Mouse wheel
		else if (button == 3 || button == 4)
		{
			this->m_ScrollY += CHAR_HEIGHT * SKIP_PER_SCROLL * (button == 3 ? 1 : -1);
			this->m_ScrollY = std::max(0.f, std::min(this->m_ScrollY,this->m_MaxScrollY));
			
			glutPostRedisplay();
		}
	}

	void SourceEditor::HandleMotion(int x, int y) noexcept
	{
		x -= this->m_X;
		y = (glutGet(GLUT_WINDOW_HEIGHT) - y) - this->m_Y;

		if (this->m_IsActiveVertical)
		{
			float delta{y - this->m_LastMouseY};
			this->m_LastMouseY = y;

			this->m_ScrollY -= delta * (this->m_MaxScrollY / (this->m_Height - CORNER_HEIGHT));
			this->m_ScrollY = std::max(0.f, std::min(this->m_ScrollY, this->m_MaxScrollY));
		
			glutPostRedisplay();
		}
		else if (this->m_IsActiveHorizontal)
		{
			float delta{x - this->m_LastMouseX};
            this->m_LastMouseX = x;

  	        this->m_ScrollX += delta * (this->m_MaxScrollX / (this->m_Width - CORNER_WIDTH));
			this->m_ScrollX = std::max(0.f, std::min(this->m_ScrollX, this->m_MaxScrollX));
		
			glutPostRedisplay();
		}
	}
	
	void SourceEditor::Render(void) const noexcept
	{
		this->m_Renderer.ClearShapes();

		// Render the text area
		glViewport(this->m_X, this->m_Y + CORNER_HEIGHT, this->m_Width - CORNER_WIDTH, this->m_Height - CORNER_HEIGHT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, this->m_Width - CORNER_WIDTH, this->m_Height - CORNER_HEIGHT, 0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

        utility::ColorTag tag{};
        for (const auto &i : std::views::iota(0UL, this->m_Lines.size()))
        {
            if ((i * CHAR_WIDTH + CHAR_WIDTH - this->m_ScrollY < 0.f) || (i * CHAR_WIDTH - this->m_ScrollY > this->m_Height - CORNER_HEIGHT))
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
							renderStart + static_cast<unsigned long>((this->m_Width - CORNER_WIDTH) / CHAR_HEIGHT) + 2ul
						)
					)
				};

				glColor3f(color.at(0), color.at(1), color.at(2));
       			glRasterPos2f
				(
					std::max(1.f, 5.f - this->m_ScrollX + static_cast<float>(renderStart) * CHAR_HEIGHT),
					std::clamp(5.f + i * CHAR_WIDTH - this->m_ScrollY, 1.f, this->m_Height - CORNER_HEIGHT - 1.f)
				);
       			
				if (renderStart < renderEnd)
					for (const auto &j : std::views::iota(renderStart, renderEnd))
           				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, realText.at(j));
                   
				pos += text.size();
             }
        }
	
		glViewport(
	        0,
	        0,
	        glutGet(GLUT_WINDOW_WIDTH),
	        glutGet(GLUT_WINDOW_HEIGHT)
	    );

    	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();

    	gluOrtho2D(
    	    0,
    	    glutGet(GLUT_WINDOW_WIDTH),
    	    0,
    	    glutGet(GLUT_WINDOW_HEIGHT)
    	);

    	glMatrixMode(GL_MODELVIEW);
    	glLoadIdentity();
	
		// Render the vertical scrollbar
		if (this->m_MaxScrollY > 0)
		{
			float pos{(this->m_ScrollY / this->m_MaxScrollY) * (this->m_Height - CORNER_HEIGHT - this->m_VerticalSliderHeight)};
			pos = std::max(0.f, std::min(pos, this->m_Height - CORNER_HEIGHT - this->m_VerticalSliderHeight));

			utility::Shape vScrollbar{utility::Shape::Type::RECTANGLE};
			vScrollbar.Replace(this->m_Width - CORNER_WIDTH, pos);
			vScrollbar.Resize(CORNER_WIDTH, this->m_VerticalSliderHeight);
			vScrollbar.SetColor(0.5f, 0.5f, 0.5f);
		
			this->m_Renderer.AddShape(std::move(vScrollbar));
		}

		// Render the horizontal scrollbar
		if (this->m_MaxScrollX > 0)
		{
			float pos{(this->m_ScrollX / this->m_MaxScrollX) * (this->m_Width - CORNER_WIDTH - this->m_HorizontalSliderWidth)};
			pos = std::max(0.f, std::min(pos, this->m_Width - CORNER_WIDTH - this->m_HorizontalSliderWidth));

			utility::Shape hScrollbar{utility::Shape::Type::RECTANGLE};
			hScrollbar.Replace(pos, 0.f);
			hScrollbar.Resize(this->m_HorizontalSliderWidth, CORNER_HEIGHT);
			hScrollbar.SetColor(0.5f, 0.5f, 0.5f);

			this->m_Renderer.AddShape(std::move(hScrollbar));
		}

		this->m_Renderer.Render();
	} 
} /* disxx::ui */
