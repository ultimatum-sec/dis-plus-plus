module;

#include <disconf.hpp>

#include <algorithm>
#include <utility>
#include <ranges>

export module disxx.loader.executable.ExecutableFile;

export import <cstdint>;
export import <vector>;

export import disxx.loader.executable.Section;

export namespace disxx::loader::executable
{
	class __DISXX_EXPORT__ [[nodiscard]] ExecutableFile
	{
	  private:
		std::vector<Section> m_Sections;
		std::uint32_t m_Magic;

	  public:
		explicit ExecutableFile(void) noexcept;
		
		explicit ExecutableFile(const ExecutableFile &) noexcept;
		ExecutableFile &operator=(const ExecutableFile &) noexcept;

		explicit ExecutableFile(ExecutableFile &&) noexcept;
		ExecutableFile &operator=(ExecutableFile &&) noexcept;

		inline void AddSection(Section &&) noexcept;
		inline void SetMagic(std::uint32_t) noexcept;

		inline const std::vector<Section> &GetSections(void) const noexcept;
		inline std::uint32_t GetMagic(void) const noexcept;
	};

	inline void ExecutableFile::AddSection(Section &&section) noexcept
	{
		this->m_Sections.emplace_back(std::move(section));
		std::ranges::sort
		(
			this->m_Sections,
			[](const auto &x, const auto &y) -> bool
			{ return x.GetOffset() < y.GetOffset(); }
		);
	}

	inline void ExecutableFile::SetMagic(std::uint32_t magic) noexcept
	{ this->m_Magic = magic; }

	inline const std::vector<Section> &ExecutableFile::GetSections(void) const noexcept
	{ return this->m_Sections; }

	inline std::uint32_t ExecutableFile::GetMagic(void) const noexcept
	{ return this->m_Magic; }
} /* disxx::loader::executable */
