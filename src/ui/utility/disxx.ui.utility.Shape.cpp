module;

#include <utility>
#include <tuple>
#include <array>

module disxx.ui.utility.Shape;

namespace disxx::ui::utility
{
	Shape::Shape(Type type) noexcept
		: m_Type{type}
		, m_Color{}
		, m_Width{}
		, m_Heigth{}
		, m_X{}
		, m_Y{}
	{}

	Shape::Shape(const Shape &other) noexcept
		: m_Type{other.m_Type}
		, m_Color{other.m_Color}
		, m_Width{other.m_Width}
		, m_Heigth{other.m_Heigth}
		, m_X{other.m_X}
		, m_Y{other.m_Y}
	{}

	Shape &Shape::operator=(const Shape &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Type = other.m_Type;
			this->m_Color = other.m_Color;
			this->m_Width = other.m_Width;
			this->m_Heigth = other.m_Heigth;
			this->m_X = other.m_X;
			this->m_Y = other.m_Y;
		}
	
		return *this;
	}

	Shape::Shape(Shape &&other) noexcept
		: m_Type{std::move(other.m_Type)}
		, m_Color{std::move(other.m_Color)}
		, m_Width{std::move(other.m_Width)}
		, m_Heigth{std::move(other.m_Heigth)}
		, m_X{std::move(other.m_X)}
		, m_Y{std::move(other.m_Y)}
	{}

	Shape &Shape::operator=(Shape &&other) noexcept
	{
		this->m_Type = std::move(other.m_Type);
		this->m_Color = std::move(other.m_Color);
		this->m_Width = std::move(other.m_Width);
		this->m_Heigth = std::move(other.m_Heigth);
		this->m_X = std::move(other.m_X);
		this->m_Y = std::move(other.m_Y);

		return *this;
	}

	Shape::Type Shape::GetType(void) const noexcept
	{ return this->m_Type; }

	void Shape::Resize(float width, float heigth) noexcept
	{
		this->m_Width = width;
		this->m_Heigth = heigth;
	}

	void Shape::Replace(float x, float y) noexcept
	{
		this->m_X = x;
		this->m_Y = y;
	}

	std::pair<float, float> Shape::GetSize(void) const noexcept
	{
		return std::make_pair
		(
			this->m_Width,
			this->m_Heigth
		);
	}

	std::pair<float, float> Shape::GetPosition(void) const noexcept
	{
		return std::make_pair
		(
			this->m_X,
			this->m_Y
		);
	}

	void Shape::SetColor(float r, float g, float b) noexcept
	{
		this->m_Color.at(0) = r;
		this->m_Color.at(1) = g;
		this->m_Color.at(2) = b;
	}

	std::tuple<float, float, float> Shape::GetColor(void) const noexcept
	{
		return std::make_tuple
		(
			this->m_Color.at(0),
			this->m_Color.at(1),
			this->m_Color.at(2)
		);
	}
} /* disxx::ui::utility */
