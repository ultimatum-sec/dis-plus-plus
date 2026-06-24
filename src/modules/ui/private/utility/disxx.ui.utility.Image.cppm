module;

#ifdef __APPLE__
#   include <OpenGL/gl.h>
#	include <OpenGL/glu.h>
#else
#   include <GL/gl.h>
#endif

#include <tuple>

export module disxx.ui.utility.Image;

export namespace disxx::ui::utility
{
	class Image
	{
	  private:
		GLuint m_Id{};
		int m_Width{}, m_Height{};

	  public:
		explicit Image(void) noexcept;
		explicit Image(GLuint, signed int, signed int) noexcept;
		Image(const Image &) noexcept;
		Image &operator=(const Image &) noexcept;

		~Image(void) noexcept = default;
		GLuint GetId(void) const noexcept;
		std::tuple<int, int> GetSizes(void) const noexcept;
	};
} /* disxx::ui::utility */
