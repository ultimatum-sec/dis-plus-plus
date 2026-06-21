module;

#include <utility>

module disxx.loader.executable.Label;

namespace disxx::loader::executable
{
	Label::Label(void) noexcept
		: m_FileOffset{}
		, m_Name{}
		, m_Data{}
	{}

	Label(const Label &other) noexcept
		: m_FileOffset{other.m_FileOffset}
		, m_Name{other.m_Name}
		, m_Data{other.m_Data}
	{}

	Label &Label::operator=(const Label &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_FileOffset = other.m_FileOffset;
			this->m_Name = other.m_Name;
			this->m_Data = other.m_Data;
		}
	
		return *this;
	}
	
	Label::Label(Label &&other) noexcept
		: m_FileOffset{std::move(other.m_FileOffset)}
		, m_Name{std::move(other.m_Name)}
		, m_Data{std::move(other.m_Data)}
	{}
	
	Label &Label::operator=(Label &&other) noexcept
	{
		this->m_FileOffset = std::move(other.m_FileOffset);
		this->m_Name = std::move(other.m_Name);	
		this->m_Data = std::move(other.m_Data);

		return *this;
	}
} /* disxx::loader::executable */
