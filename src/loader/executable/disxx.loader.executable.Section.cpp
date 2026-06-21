module;

#include <utility>

module disxx.loader.executable.Section;

namespace disxx::loader::executable
{
	Section::Section(void) noexcept
		: m_Name{}
		, m_Labels{}
		, m_Address{}
		, m_Size{}
		, m_Offset{}
		, m_Index{}
	{}

	Section::Section(const Section &other) noexcept
		: m_Name{other.m_Name}
		, m_Labels{other.m_Labels}
		, m_Address{other.m_Address}
		, m_Size{other.m_Size}
		, m_Offset{other.m_Offset}
		, m_Index{other.m_Index}
	{}

	Section &Section::operator=(const Section &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Name = other.m_Name;
			this->m_Labels = other.m_Labels;
			this->m_Address = other.m_Address;
			this->m_Size = other.m_Size;
			this->m_Offset = other.m_Offset;
			this->m_Index = other.m_Index;
		}
	
		return *this;
	}
	
	Section::Section(Section &&other) noexcept
		: m_Name{std::move(other.m_Name)}
		, m_Labels{std::move(other.m_Labels)}
		, m_Address{std::move(other.m_Address)}
		, m_Size{std::move(other.m_Size)}
		, m_Offset{std::move(other.m_Offset)}
		, m_Index{std::move(other.m_Index)}
	{}

	Section &Section::operator=(Section &&other) noexcept
	{
		this->m_Name = std::move(other.m_Name);
		this->m_Labels = std::move(other.m_Labels);
		this->m_Address = std::move(other.m_Address);
		this->m_Size = std::move(other.m_Size);
		this->m_Offset = std::move(other.m_Offset);
		this->m_Index = std::move(other.m_Index);
	
		return *this;
	}
} /* disxx::loader::executable */
