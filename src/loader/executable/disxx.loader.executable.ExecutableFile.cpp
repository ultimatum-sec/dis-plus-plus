module;

#include <utility>

module disxx.loader.executable.ExecutableFile;

namespace disxx::loader::executable
{
	ExecutableFile::ExecutableFile(void) noexcept
		: m_Sections{}
		, m_Magic{}
	{}

	ExecutableFile::ExecutableFile(const ExecutableFile &other) noexcept
		: m_Sections{other.m_Sections}
		, m_Magic{other.m_Magic}
	{}

	ExecutableFile &ExecutableFile::operator=(const ExecutableFile &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Sections = other.m_Sections;
			this->m_Magic = other.m_Magic;
		}
	
		return *this;
	}
	
	ExecutableFile::ExecutableFile(ExecutableFile &&other) noexcept
		: m_Sections{std::move(other.m_Sections)}
		, m_Magic{std::move(other.m_Magic)}
	{}
	
	ExecutableFile &ExecutableFile::operator=(ExecutableFile &&other) noexcept
	{
		this->m_Sections = std::move(other.m_Sections);
		this->m_Magic = std::move(other.m_Magic);
	
		return *this;
	}
} /* disxx::loader::executable */
