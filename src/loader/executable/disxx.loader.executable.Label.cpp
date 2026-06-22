module;

#include <utility>

module disxx.loader.executable.Label;

namespace disxx::loader::executable
{
	Label::Label(void) noexcept
		: m_Name{}
		, m_Offset{}
		, m_Data{}
	{}

	Label::Label(const Label &other) noexcept
		: m_Name{other.m_Name}
		, m_Offset{other.m_Offset}
		, m_Data{other.m_Data}
	{}

	Label &Label::operator=(const Label &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Name = other.m_Name;
			this->m_Offset = other.m_Offset;
			this->m_Data = other.m_Data;
		}
	
		return *this;
	}
	
	Label::Label(Label &&other) noexcept
		: m_Name{std::move(other.m_Name)}
		, m_Offset{std::move(other.m_Offset)}
		, m_Data{std::move(other.m_Data)}
	{}
	
	Label &Label::operator=(Label &&other) noexcept
	{
		this->m_Name = std::move(other.m_Name);
		this->m_Offset = std::move(other.m_Offset);
		this->m_Data = std::move(other.m_Data);

		return *this;
	}
} /* disxx::loader::executable */
