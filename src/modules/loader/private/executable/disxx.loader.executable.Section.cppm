module;

#include <disconf.hpp>

#include <algorithm>
#include <utility>
#include <ranges>

export module disxx.loader.executable.Section;

export import <string_view>;
export import <cstdint>;
export import <vector>;
export import <string>;

export import disxx.loader.executable.Label;

export namespace disxx::loader::executable
{
	class __DISXX_PRIVATE__ [[nodiscard]] Section
	{
	  private:
		std::string m_Name{};
		mutable std::vector<Label> m_Labels{};
		std::uint64_t m_Address{};
		std::uint64_t m_Size{};
		std::uint64_t m_Offset{};
		unsigned short int m_Index{};

	  public:
		explicit Section(void) noexcept;
		
		Section(const Section &) noexcept;
		Section &operator=(const Section &) noexcept;

		explicit Section(Section &&) noexcept;
		Section &operator=(Section &&) noexcept;

		inline void SetName(std::string) noexcept;
		inline void AddLabel(Label &&) const noexcept;
		inline void SetAddress(std::uint64_t) noexcept;
		inline void SetSize(std::uint64_t) noexcept;
		inline void SetOffset(std::uint64_t) noexcept;
		inline void SetIndex(unsigned short int) noexcept;

		inline std::string_view GetName(void) const noexcept;
		inline std::vector<Label> &GetLabels(void) const noexcept;
		inline std::uint64_t GetAddress(void) const noexcept;
		inline std::uint64_t GetSize(void) const noexcept;
		inline std::uint64_t GetOffset(void) const noexcept;
		inline unsigned short int GetIndex(void) const noexcept;
	};

	inline void Section::SetName(std::string name) noexcept
	{ this->m_Name = name; }
	
	inline void Section::AddLabel(Label &&label) const noexcept
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
	{ this->m_Offset = offset; }

	inline void Section::SetIndex(unsigned short int index) noexcept
	{ this->m_Index = index; }

	inline std::string_view Section::GetName(void) const noexcept
	{ return this->m_Name; }
	
	inline std::vector<Label> &Section::GetLabels(void) const noexcept
	{ return this->m_Labels; }

	inline std::uint64_t Section::GetAddress(void) const noexcept
	{ return this->m_Address; }

	inline std::uint64_t Section::GetSize(void) const noexcept
	{ return this->m_Size; }

	inline std::uint64_t Section::GetOffset(void) const noexcept
	{ return this->m_Offset; }

	inline unsigned short int Section::GetIndex(void) const noexcept
	{ return this->m_Index; }
} /* disxx::loader::executable */
