module;

#ifdef __APPLE__
#   include <OpenGL/gl.h>
#	include <OpenGL/glu.h>
#else
#   include <GL/gl.h>
#endif

#include <tuple>

module disxx.ui.utility.Image;

namespace disxx::ui::utility
{
	Image::Image(void) noexcept
		: m_Id{0}
		, m_Width{0}
		, m_Height{0}
	{}

	Image::Image(GLuint id, int width, int height) noexcept
		: m_Id{id}
		, m_Width{width}
		, m_Height{height}
	{}

	Image::Image(const Image &other) noexcept
		: m_Id{other.m_Id}
		, m_Width{other.m_Width}
		, m_Height{other.m_Height}
	{}

	Image &Image::operator=(const Image &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Id = other.m_Id;
			this->m_Width = other.m_Width;
			this->m_Height = other.m_Height;
		}

		return *this;
	}

	GLuint Image::GetId(void) const noexcept
	{ return this->m_Id; }

	std::tuple<int, int> Image::GetSizes(void) const noexcept
	{
		return std::make_tuple
		(
			this->m_Width ? this->m_Width : 1,
			this->m_Height ? this->m_Height : 1
		);
	}
} /* disxx::ui::utils */


