module;

#ifdef __APPLE__
#	include <OpenGL/gl.h>
#	include <GLUT/glut.h>
#else
#	include <GL/gl.h>
#	include <GL/glut.h>
#endif

#include <disconf.hpp>

export module disxx.ui.backend.AbstractBackend;

export namespace disxx::ui::backend
{
	class __DISXX_EXPORT__ [[nodiscard]] AbstractBackend
	{
	  public:
		static void Init(std::span<const char *>) noexcept;

		// Callbacks
		template <typename F> static void IdleFunc(F);
		template <typename F> static void KeyboardFunc(F);
		template <typename F> static void MouseFunc(F);
		template <typename F> static void MotionFunc(F);
		template <typename F> static void ReshapeFunc(F);
		template <typename F> static void DisplayFunc(F);

		static int Exec(void) noexcept;
	};
} /* disxx::ui::backend */
