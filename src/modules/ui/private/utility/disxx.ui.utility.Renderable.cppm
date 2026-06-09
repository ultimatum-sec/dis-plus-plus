module;

#include <disconf.hpp>

export module disxx.ui.utility.Renderable;

import disxx.ui.utility.Vec;

export namespace disxx::ui::utility
{
	class __DISXX_PRIVATE__ [[nodiscard]] Renderable
	{
	  protected:
		Vec3<float> m_Color;
		Vec2<float> m_Position;
		Vec2<float> m_Size;

	  public:
		explicit Renderable(void) noexcept;

		explicit Renderable(const Renderable &) noexcept;
		Renderable &operator=(const Renderable &) noexcept;

		explicit Renderable(Renderable &&) noexcept;
		Renderable &operator=(Renderable &&) noexcept;

		void Replace(Vec2<float>) noexcept;
		void Resize(Vec2<float>) noexcept;

		Vec2<float> GetPosition(void) const noexcept;
		Vec2<float> GetSize(void) const noexcept;

		void SetColor(Vec3<float>) noexcept;
		Vec3<float> GetColor(void) const noexcept;
	};
} /* disxx::ui::utility */
