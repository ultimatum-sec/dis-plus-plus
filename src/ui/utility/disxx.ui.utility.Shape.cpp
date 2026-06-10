module;

#include <utility>
#include <tuple>
#include <array>

module disxx.ui.utility.Shape;

namespace disxx::ui::utility
{
	Shape::Shape(void) noexcept
		: Renderable{}
		, m_Type{Type::TRIANGLE}
	{}

	Shape::Shape(Type type) noexcept
		: Renderable{}
		, m_Type{type}
	{}

	Shape::Shape(const Shape &other) noexcept
		: Renderable{other}
		, m_Type{other.m_Type}
	{}

	Shape &Shape::operator=(const Shape &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			Renderable::operator=(other);
			this->m_Type = other.m_Type;
		}
	
		return *this;
	}

	Shape::Shape(Shape &&other) noexcept
		: Renderable{std::move(other)}
		, m_Type{std::move(other.m_Type)}
	{}

	Shape &Shape::operator=(Shape &&other) noexcept
	{
		Renderable::operator=(other);
		this->m_Type = std::move(other.m_Type);

		return *this;
	}

	Shape::Type Shape::GetType(void) const noexcept
	{ return this->m_Type; }
} /* disxx::ui::utility */
