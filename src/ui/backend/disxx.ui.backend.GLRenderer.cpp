module;

#include <OpenGL/gl3.h>
#include <GLUT/glut.h>

#include <utility>
#include <memory>
#include <vector>
#include <array>
#include <tuple>
#include <cmath>

#include <iostream>

/*
#define CHECKSH(sh) \
{ \
	GLint success; \
	GLchar infoLog[512]; \
	glGetShaderiv(sh, GL_COMPILE_STATUS, &success); \
	if (!success) \
	{ \
	    glGetShaderInfoLog(sh, sizeof(infoLog), nullptr, infoLog); \
	    std::cerr << infoLog << std::endl; \
	} \
}
*/

module disxx.ui.backend.GLRenderer;

import disxx.ui.utility.Vertex;
import disxx.ui.utility.Vec;

namespace disxx::ui::backend
{
	void GLRenderer::SwapBuffers(void) noexcept
	{ glutSwapBuffers(); }

	void GLRenderer::Redisplay(void) noexcept
	{ glutPostRedisplay(); }

	GLRenderer::GLRenderer(void) noexcept
		: m_Shapes{}
		, m_Vao{}
		, m_Vbo{}
		, m_VertexShader{}
		, m_FragmentShader{}
		, m_Program{}
	{
		glGenVertexArrays(1, &this->m_Vao);
		glBindVertexArray(this->m_Vao);

		glGenBuffers(1, &this->m_Vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, std::pow(1048, 2), nullptr, GL_DYNAMIC_DRAW);
		// For positions
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(utility::Vertex<GLfloat>), (GLvoid *)(offsetof(utility::Vertex<GLfloat>, position)));
		glEnableVertexAttribArray(0);
		// For colors
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(utility::Vertex<GLfloat>), (GLvoid *)(offsetof(utility::Vertex<GLfloat>, color)));
		glEnableVertexAttribArray(1);

		this->m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(this->m_VertexShader, 1, &s_pVertexSource, nullptr);
		glCompileShader(this->m_VertexShader);
		
		this->m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(this->m_FragmentShader, 1, &s_pFragmentSource, nullptr);
		glCompileShader(this->m_FragmentShader);

		this->m_Program = glCreateProgram();
		glAttachShader(this->m_Program, this->m_VertexShader);
		glAttachShader(this->m_Program, this->m_FragmentShader);
		glBindAttribLocation(this->m_Program, 0, "position");
		glBindAttribLocation(this->m_Program, 1, "incolor");
		glLinkProgram(this->m_Program);
		glUseProgram(this->m_Program);
	}

	GLRenderer::~GLRenderer(void) noexcept
	{
		glDeleteVertexArrays(1, &this->m_Vao);
		glDeleteBuffers(1, &this->m_Vbo);
		glDeleteShader(this->m_VertexShader);
		glDeleteShader(this->m_FragmentShader);
		glDeleteProgram(this->m_Program);
	}

	void GLRenderer::PushShape(utility::Shape &&shape) noexcept
	{
		if (this->m_Shapes.size() < 1024 * 1024) [[likely]]
			this->m_Shapes.emplace_back(std::move(shape));
	}

	void GLRenderer::PopShape(void) noexcept
	{
		if (this->m_Shapes.size() > 0) [[likely]]
			this->m_Shapes.pop_back();
	}

	void GLRenderer::ClearBuffer(void) noexcept
	{
		if (this->m_Shapes.size() > 0) [[likely]]
			this->m_Shapes.clear();
	}

	void GLRenderer::Render(void) noexcept
	{
		// Get actual window size and set up a projection
		GLfloat projection[] = {
			2.f / glutGet(GLUT_WINDOW_WIDTH), 0.f, 0.f, 0.f,
			0.f, 2.f / glutGet(GLUT_WINDOW_HEIGHT), 0.f, 0.f,
			0.f, 0.f, -1.f, 0.f,
			-1.f, -1.f, 0.f, 1.f
		};
		
		glUseProgram(this->m_Program);
		GLint loc{glGetUniformLocation(this->m_Program, "projection")};
		glUniformMatrix4fv(loc, 1, GL_FALSE, projection);

		std::vector<utility::Vertex<GLfloat>> vertices{};
		for (const auto &shape : this->m_Shapes)
		{
			switch (shape.GetType())
			{
			  case utility::Shape::Type::RECTANGLE:
			  {
				const auto &[x, y]{shape.GetPosition()};
				const auto &[width, height]{shape.GetSize()};
				const auto &[r, g, b]{shape.GetColor()};

				// First triangle
				vertices.emplace_back(utility::Vertex<GLfloat>{utility::Vec2<GLfloat>{x, y}, utility::Vec3<GLfloat>{r, g, b}});
				vertices.emplace_back(utility::Vertex<GLfloat>{utility::Vec2<GLfloat>{x, y + height}, utility::Vec3<GLfloat>{r, g, b}});
				vertices.emplace_back(utility::Vertex<GLfloat>{utility::Vec2<GLfloat>{x + width, y + height}, utility::Vec3<GLfloat>{r, g, b}});

				// Second triangle
				vertices.emplace_back(utility::Vertex<GLfloat>{utility::Vec2<GLfloat>{x + width, y + height}, utility::Vec3<GLfloat>{r, g, b}});
				vertices.emplace_back(utility::Vertex<GLfloat>{utility::Vec2<GLfloat>{x + width, y}, utility::Vec3<GLfloat>{r, g, b}});
				vertices.emplace_back(utility::Vertex<GLfloat>{utility::Vec2<GLfloat>{x, y}, utility::Vec3<GLfloat>{r, g, b}});
			 	
				break;
			  }
			  
			  default:
				break;
			}
		}

		glBindVertexArray(this->m_Vao);
		glBindBuffer(GL_ARRAY_BUFFER, this->m_Vbo);

		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(utility::Vertex<GLfloat>), vertices.data());

		glUseProgram(this->m_Program);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}
} /* disxx::ui::backend */
