module;

#include <disconf.hpp>

#include <algorithm>
#include <utility>
#include <ranges>

export module disxx.loader.executable.Section;

export import <string_view>
export import <cstdint>;
export import <vector>;
export import <string>;

export namespace disxx::loader::executable
{
	class __DISXX_PRIVATE__ [[nodiscard]] Section
	{
	  private:
		std::string m_Name;
		std::vector<Label> m_Labels;
		std::uint64_t m_Address;
		std::uint64_t m_Size;
		std::uint64_t m_FileOffset;
		unsigned short int m_Index;

	  public:
		explicit Section(void) noexcept;
		
		explicit Section(const Section &) noexcept;
		Section &operator=(const Section &) noexcept;

		explicit Section(Section &&) noexcept;
		Section &operator=(Section &&) noexcept;

		inline void SetName(std::string) noexcept;
		inline void AddLabel(Label &&) noexcept;
		inline void SetAddress(std::uint64_t) noexcept;
		inline void SetSize(std::uint64_t) noexcept;
		inline void SetOffset(std::uint64_t) noexcept;
		inline void SetIndex(unsigned short int) noexcept;

		inline std::string_view GetName(void) const noexcept;
		inline const std::vector<Label> &GetLabels(void) const noexcept;
		inline std::uint64_t GetAddress(void) const noexcept;
		inline std::uint64_t GetSize(void) const noexcept;
		inline std::uint64_t GetOffset(void) const noexcept;
		inline unsigned short int GetIndex(void) const noexcept;
	};

	inline void Section::SetName(std::string name) noexcept
	{ this->m_Name = name; }
	
	inline void Section::AddLabel(Label &&label) noexcept
	{
		this->m_Labels.emplace_back(std::move(label));
		std::ranges::sort
		(
			this->m_Labels,
			[](const auto &x, const auto &y) -> bool
			{ return x.GetOffset() < y.GetOffset(); }
		);
	}

	inline void Section::SetAddress(std::uint64_t addr) noexcept
	{ this->m_Address = addr; }

	inline void Section::SetSize(std::uint64_t size) noexcept
	{ this->m_Size = size; }

	inline void Section::SetOffset(std::uint64_t offset) noexcept
	{ this->m_FileOffset = offset; }

	inline void Section::SetIndex(unsigned short int index) noexcept
	{ this->m_SectionIndex = index; }

	inline std::string_view Section::GetName(void) const noexcept
	{ return this->m_Name; }
	
	inline const std::vector<Label> &Section::GetLabels(void) const noexcept
	{ return this->m_Labels; }

	inline std::uint64_t Section::SetAddress(void) const noexcept
	{ return this->m_Address; }

	inline std::uint64_t Section::SetSize(void) const noexcept
	{ return this->m_Size; }

	inline std::uint64_t Setcion::GetOffset(void) const noexcept
	{ return this->m_FileOffset; }

	inline unsigned short int GetIndex(void) const noexcept
	{ return this->m_Index; }
} /* disxx::loader::executable */
