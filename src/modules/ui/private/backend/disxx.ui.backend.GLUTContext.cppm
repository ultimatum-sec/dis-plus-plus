module;

#include <disconf.hpp>

#ifdef __APPLE__
#	include <GLUT/glut.h>
#else
#	include <GL/freeglut.h>
#endif

export module disxx.ui.backend.GLUTContext;

export import <unordered_map>;
export import <string_view>;
export import <functional>;
export import <utility>;

import disxx.ui.backend.IContext;
import disxx.ui.utility.Vec;

export namespace disxx::ui::backend
{
	class __DISXX_EXPORT__ [[nodiscard]] GLUTContext final : public IContext<int>
	{
	  private:
		static std::unordered_map
		<
			WindowHandle,
			std::function<void(void)>
		> s_DisplayCallbacks;
		static std::unordered_map
		<
			WindowHandle,
			std::function<void(int, int)>
		> s_ReshapeCallbacks;
		static std::unordered_map
		<
			WindowHandle,
			std::function<void(unsigned char, int, int)>
		> s_KeyboardCallbacks;
		static std::unordered_map
		<
			WindowHandle,
			std::function<void(int, int, int, int)>
		> s_MouseButtonCallbacks;
		static std::unordered_map
		<
			WindowHandle,
			std::function<void(int, int)>
		> s_MouseMotionCallbacks;

	  public:
		template <typename ...Args>
		static void Init(Args &&...) noexcept;
		static inline utility::Vec2<float> GetWindowSize(void) noexcept;

	  public:
		explicit GLUTContext(void) noexcept = default;

		explicit GLUTContext(const GLUTContext &) noexcept = default;
		GLUTContext &operator=(const GLUTContext &) noexcept = default;

		~GLUTContext(void) noexcept = default;

		// Sone window methods
		[[clang::acquire_handle("window")]]
		virtual WindowHandle CreateWindow(utility::Vec2<int>, std::string_view) noexcept override;
		virtual void DestroyWindow([[clang::release_handle("window")]] WindowHandle &) noexcept override;
		virtual void SwitchWindow([[clang::use_handle("window")]] const WindowHandle &) noexcept override;
		virtual void ShowWindow(void) noexcept override;
		virtual void HideWindow(void) noexcept override;

		// Callback methods
		inline void SetDisplayCallback(std::function<void(void)>) noexcept;
		inline void SetReshapeCallback(std::function<void(int, int)>) noexcept;
		inline void SetKeyboardCallback(std::function<void(unsigned char, int, int)>) noexcept;
		inline void SetMouseButtonCallback(std::function<void(int, int, int, int)>) noexcept;
		inline void SetMouseMotionCallback(std::function<void(int, int)>) noexcept;
	
		// Miscellaneous methods
		virtual void SwapBuffers(void) const noexcept override;
		virtual void Redisplay(void) const noexcept override;
		virtual void Exec(void) const noexcept override;
	};

	template <typename ...Args>
	void GLUTContext::Init(Args &&...args) noexcept
	{
		if constexpr (sizeof...(args) == 0)
		{
			static int argc{0};
			static char *argv[] = {"\0"};
			glutInit(&argc, argv);
		}
		else if constexpr (sizeof...(args) == 2)
			glutInit(std::forward<Args>(args)...);
		else
			static_assert(false, "Incorrect arguments for disxx::ui::backend::GLUTContext::Init");
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	}

	inline utility::Vec2<float> GLUTContext::GetWindowSize(void) noexcept
	{
		return utility::Vec2<float>
		{
			glutGet(GLUT_WINDOW_WIDTH) * 1.f,
			glutGet(GLUT_WINDOW_HEIGHT) * 1.f
		};
	}

	inline void GLUTContext::SetDisplayCallback(std::function<void(void)> func) noexcept
	{ s_DisplayCallbacks[glutGetWindow()] = func; }

	inline void GLUTContext::SetReshapeCallback(std::function<void(int, int)> func) noexcept
	{ s_ReshapeCallbacks[glutGetWindow()] = func; }
	
	inline void GLUTContext::SetKeyboardCallback(std::function<void(unsigned char, int, int)> func) noexcept
	{ s_KeyboardCallbacks[glutGetWindow()] = func; }
	
	inline void GLUTContext::SetMouseButtonCallback(std::function<void(int, int, int, int)> func) noexcept
	{ s_MouseButtonCallbacks[glutGetWindow()] = func; }
	
	inline void GLUTContext::SetMouseMotionCallback(std::function<void(int, int)> func) noexcept
	{ s_MouseMotionCallbacks[glutGetWindow()] = func; }
} /* disxx::ui::backend */
