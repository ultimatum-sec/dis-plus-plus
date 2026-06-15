module;

#include <disconf.hpp>

export module disxx.ui.Context;

export import disxx.ui.backend.GLUTContext;

export namespace disxx::ui
{
	#if defined(BACKEND_CTX_GLUT)
		class __DISXX_EXPORT__ [[nodiscard]] Context final : public backend::GLUTContext {};
	#endif
} /* disxx::ui */
