module;

#include <disconf.hpp>

#include <utility>
#include <tuple>
#include <array>

export module disxx.ui.utility.Shape;

export namespace disxx::ui::utility
{
	class __DISXX_PRIVATE__ [[nodiscard]] Shape
	{
	  public:
		enum class Type
		{
			TRIANGLE,
			RECTANGLE
		};
 
	  protected:
		Type m_Type;
		std::array<float, 3> m_Color;
		float m_Width, m_Heigth;
		float m_X, m_Y;

	  public:
		explicit Shape(Type) noexcept;
		
		explicit Shape(const Shape &) noexcept;
		Shape &operator=(const Shape &) noexcept;

		explicit Shape(Shape &&) noexcept;
		Shape &operator=(Shape &&) noexcept;

		Type GetType(void) const noexcept;
	
		void Resize(float, float) noexcept;
		void Replace(float, float) noexcept;

		std::pair<float, float> GetSize(void) const noexcept;
		std::pair<float, float> GetPosition(void) const noexcept;

		void SetColor(float, float, float) noexcept;
		std::tuple<float, float, float> GetColor(void) const noexcept;
	};
} /* disxx::ui::utility */
