module;

#ifdef __APPLE__
#	include <GLUT/glut.h>
#	include <OpenGL/gl.h>
#else
#	include <GL/freeglut.h>
#endif

#include <string_view>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <tuple>

module disxx.ui.LabelBrowser;

namespace disxx::ui
{
	LabelBrowser::LabelBrowser(void) noexcept
		: Widget{}
		, m_Scroll{0.f}
		, m_MaxScroll{0.f}
		, m_LastMouse{0.f}
		, m_SliderHeight{0.f}
		, m_Labels{}	
	{ this->_CalcMaxScroll(); }

	LabelBrowser::LabelBrowser(float x, float y, float width, float height) noexcept
		: Widget{x, y, width, height}
        , m_Scroll{0.f}
		, m_MaxScroll{0.f}
        , m_LastMouse{0.f}
		, m_SliderHeight{0.f}
        , m_Labels{}
	{ this->_CalcMaxScroll(); }

	LabelBrowser::LabelBrowser(const LabelBrowser &other) noexcept
		: Widget{other}
        , m_Scroll{other.m_Scroll}
		, m_MaxScroll{other.m_MaxScroll}
		, m_LastMouse{other.m_LastMouse}
		, m_SliderHeight{other.m_SliderHeight}
		, m_Labels{other.m_Labels}
	{}

	LabelBrowser &LabelBrowser::operator=(const LabelBrowser &other) noexcept
	{
		if (this != &other)
		{
			Widget::operator=(other);
			this->m_Scroll = other.m_Scroll;
			this->m_MaxScroll = other.m_MaxScroll;
			this->m_LastMouse = other.m_LastMouse;
			this->m_SliderHeight = other.m_SliderHeight;
			this->m_Labels = other.m_Labels;
		}

		return *this;
	}

	void LabelBrowser::_CalcMaxScroll(void) noexcept
    {
        if (this->m_Labels.empty()) [[unlikely]]
		{
            this->m_MaxScroll = 0.f;
            this->m_SliderHeight = this->m_Height;
            
			return;
        }
    
        float labelsHeight{35.f * this->m_Labels.size()};
        this->m_MaxScroll = std::max(0.f, labelsHeight - this->m_Height);
        this->m_Scroll = std::clamp(this->m_Scroll, 0.f, this->m_MaxScroll);

        float visibleRatio{this->m_Height / labelsHeight};
        this->m_SliderHeight = this->m_Height * visibleRatio;
        this->m_SliderHeight = std::clamp(this->m_SliderHeight, 30.f, this->m_Height);
    }
	
	void LabelBrowser::AddLabel(std::string str) noexcept(false)
    {
        Button btn;
        btn.Resize(this->m_Width - 15.f, 30);
        btn.SetColor(0.7f, 0.7f, 0.7f);
        btn.SetText(str);

        float yPos
		{
			!this->m_Labels.empty() 
            	? (this->m_Labels.size() * 35.f) 
            	: 0.f
		};
        
        btn.Replace(0.f, yPos);
        this->m_Labels.push_back(btn);
        this->_CalcMaxScroll();
    }

	void LabelBrowser::Resize(float width, float height) noexcept
	{
		Widget::Resize(width, height);
		
		for (auto &label : this->m_Labels)
		{
			const auto [_, h] = label.GetSizes();
			label.Resize(width - 15.f, h);
		}
		
		this->_CalcMaxScroll();
	}
	
	void LabelBrowser::HandleMouse(int button, int state, int x, int y) noexcept
	{
	    y = glutGet(GLUT_WINDOW_HEIGHT) - y;
		
		if (!(x >= this->m_X && x <= this->m_X + this->m_Width && y >= this->m_Y && y <= this->m_Y + this->m_Height))
		{
	        this->m_IsClicked = false;
	        return;
	    }
	
	    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
	        if (x >= this->m_X + this->m_Width - 15)
			{
	            this->m_IsClicked = true;
	            this->m_LastMouse = y;
	            
	            if (this->m_MaxScroll > 0)
				{
	                this->m_Scroll = std::clamp(((y - this->m_Y) / this->m_Height) * this->m_MaxScroll, 0.f, this->m_MaxScroll);
	                glutPostRedisplay();
	            }
	        }
				
			/*
			 *	TODO: Make pushed button point on the label in SourceEditor
	         *	else
			 *	{
	         *	    for (auto& label : this->m_Labels)
	         *	        label.HandleMouse(button, state, x - this->m_X, this->m_Y + this->m_Height - y + this->m_Scroll);
	         *	}
			 */
	    }
	    else if (button == 3 || button == 4)
		{
	        this->m_Scroll += (button == 3 ? -30.f : 30.f);
	        this->m_Scroll = std::clamp(this->m_Scroll, 0.f, this->m_MaxScroll);
	        glutPostRedisplay();
	    }
   		else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        	this->m_IsClicked = false;
	}

	void LabelBrowser::HandleMotion([[maybe_unused]] int x, int y) noexcept
	{
		if (!this->m_IsClicked)
			return;
	
		const auto winHeight{glutGet(GLUT_WINDOW_HEIGHT)};	
		y = (winHeight - y) + (winHeight - (winHeight - y) * -1);
	
		//const float widgetTop{m_Y + m_Height};
		//const float widgetBottom{m_Y};
		
		float delta{this->m_LastMouse - y};
		this->m_LastMouse = y;
		
		if (this->m_MaxScroll > 0.f)
		{
			this->m_Scroll += delta * (this->m_MaxScroll / this->m_Height);
			this->m_Scroll = std::clamp(this->m_Scroll, 0.f, this->m_MaxScroll);
			
			glutPostRedisplay();
		}
	}

	void LabelBrowser::Render(void) const noexcept
	{
		auto y{glutGet(GLUT_WINDOW_HEIGHT) - (this->m_Y + this->m_Height)};

		glViewport(this->m_X, y, this->m_Width - 15, this->m_Height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, this->m_Width - 15, this->m_Height, 0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		glEnable(GL_SCISSOR_TEST);
			glScissor(this->m_X, y, this->m_Width - 15, this->m_Height);
			glColor3fv(this->m_pColor);
			glRectf(0, 0, this->m_Width - 15, this->m_Height);
			glTranslatef(0, -this->m_Scroll, 0);
			
			if (!this->m_Labels.empty()) [[likely]]
			{
                auto start{static_cast<unsigned long int>(this->m_Scroll / 35.f)};
                auto end{static_cast<unsigned long int>((this->m_Scroll + this->m_Height) / 35.f) + 1};
                
                if (end > this->m_Labels.size())
                    end = this->m_Labels.size();
                
                for (auto i{start}; i < end; ++i)
                    this->m_Labels.at(i).Render();
            }
		glDisable(GL_SCISSOR_TEST);
		
		glViewport(this->m_X + this->m_Width - 15, y, 15, this->m_Height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, 15.f, this->m_Height, 0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		glColor3f(0.5f, 0.5f, 0.5f);
		glRectf(0, 0, 15.f, this->m_Height);
		if (this->m_MaxScroll > 0.f)
		{
			float scrollRatio{this->m_Scroll / this->m_MaxScroll};
			float sliderY{scrollRatio * (this->m_Height - this->m_SliderHeight)};
			
			glColor3f(0.7f, 0.7f, 0.7f);
			glRectf(3, sliderY, 12, sliderY + m_SliderHeight);
			
			glColor3f(0.3f, 0.3f, 0.3f);
			glLineWidth(1.f);
			glBegin(GL_LINE_LOOP);
				glVertex2f(3, sliderY);
				glVertex2f(12, sliderY);
				glVertex2f(12, sliderY + this->m_SliderHeight);
				glVertex2f(3, sliderY + this->m_SliderHeight);
			glEnd();
		}
	}
} /* disxx::ui */


