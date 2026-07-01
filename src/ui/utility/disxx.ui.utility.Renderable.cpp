module;

#include <utility>

module disxx.ui.utility.Renderable;

namespace disxx::ui::utility
{
	Renderable::Renderable(void) noexcept
		: m_Color{}
		, m_Position{}
		, m_Size{}
		, m_Type{}
	{}

	Renderable::Renderable(Renderable::Type type) noexcept
		: m_Color{}
		, m_Position{}
		, m_Size{}
		, m_Type{type}
	{}

	Renderable::Renderable(const Renderable &other) noexcept
		: m_Color{other.m_Color}
		, m_Position{other.m_Position}
		, m_Size{other.m_Size}
		, m_Type{other.m_Type}
	{}

	Renderable &Renderable::operator=(const Renderable &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Color = other.m_Color;
			this->m_Position = other.m_Position;
			this->m_Size = other.m_Size;
			this->m_Type = other.m_Type;
		}

		return *this;
	}

	Renderable::Renderable(Renderable &&other) noexcept
		: m_Color{std::move(other.m_Color)}
		, m_Position{std::move(other.m_Position)}
		, m_Size{std::move(other.m_Size)}
		, m_Type{std::move(other.m_Type)}
	{}

	Renderable &Renderable::operator=(Renderable &&other) noexcept
	{
		this->m_Color = std::move(other.m_Color);
		this->m_Position = std::move(other.m_Position);
		this->m_Size = std::move(other.m_Size);
		this->m_Type = std::move(other.m_Type);
		
		return *this;
	}

	void Renderable::Replace(Vec2<float> vec) noexcept
	{ this->m_Position = vec; }

	void Renderable::Resize(Vec2<float> vec) noexcept
	{ this->m_Size = vec; }

	Vec2<float> Renderable::GetPosition(void) const noexcept
	{ return Vec2<float>{this->m_Position}; }

	Vec2<float> Renderable::GetSize(void) const noexcept
	{ return Vec2<float>{this->m_Size}; }

	void Renderable::SetColor(Vec3<float> vec) noexcept
	{ this->m_Color = vec; }

	Vec3<float> Renderable::GetColor(void) const noexcept
	{ return Vec3<float>{this->m_Color}; }
} /* disxx::ui::utility */
