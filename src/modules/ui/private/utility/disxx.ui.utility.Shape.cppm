module;

#include <disconf.hpp>

#include <utility>
#include <tuple>
#include <array>

export module disxx.ui.utility.Shape;

import disxx.ui.utility.Renderable;

export namespace disxx::ui::utility
{
	class __DISXX_PRIVATE__ [[nodiscard]] Shape final : public Renderable
	{
	  public:
		enum class Type
		{
			TRIANGLE,
			RECTANGLE
		};
 
	  private:
		Type m_Type;

	  public:
		explicit Shape(void) noexcept;
		explicit Shape(Type) noexcept;
		
		explicit Shape(const Shape &) noexcept;
		Shape &operator=(const Shape &) noexcept;

		explicit Shape(Shape &&) noexcept;
		Shape &operator=(Shape &&) noexcept;

		Type GetType(void) const noexcept;
	};
} /* disxx::ui::utility */
