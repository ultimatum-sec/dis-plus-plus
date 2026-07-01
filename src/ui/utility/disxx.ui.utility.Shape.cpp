module;

#include <utility>
#include <tuple>
#include <array>

module disxx.ui.utility.Shape;

namespace disxx::ui::utility
{
	Shape::Shape(void) noexcept
		: Renderable{Renderable::Type::TYPE_SHAPE}
		, m_ShapeType{Type::TYPE_TRIANGLE}
	{}

	Shape::Shape(Type type) noexcept
		: Renderable{Renderable::Type::TYPE_SHAPE}
		, m_ShapeType{type}
	{}

	Shape::Shape(const Shape &other) noexcept
		: Renderable{other}
		, m_ShapeType{other.m_ShapeType}
	{}

	Shape &Shape::operator=(const Shape &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			Renderable::operator=(other);
			this->m_ShapeType = other.m_ShapeType;
		}
	
		return *this;
	}

	Shape::Shape(Shape &&other) noexcept
		: Renderable{std::move(other)}
		, m_ShapeType{std::move(other.m_ShapeType)}
	{}

	Shape &Shape::operator=(Shape &&other) noexcept
	{
		Renderable::operator=(other);
		this->m_ShapeType = std::move(other.m_ShapeType);

		return *this;
	}
} /* disxx::ui::utility */
