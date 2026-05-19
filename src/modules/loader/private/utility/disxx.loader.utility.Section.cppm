module;

#include <type_traits>
#include <concepts>
#include <utility>
#include <cstdint>
#include <string>
#include <vector>

// For ntohl(1)
#ifndef _WIN32
#   include <arpa/inet.h>
#else
#   include <winsock2.h>
#endif

export module disxx.loader.utility.Section;

export import disxx.loader.utility.Label;

export namespace disxx::loader::utility
{
	class Section
	{
	  private:
		__uint128_t m_VirtualStart, m_Start, m_Size;
		std::vector<std::pair<func_t, std::vector<std::uint8_t>>> m_LabelsAndData;
		std::string m_SectionName;	// Segment + section names (if it's mach-o section)	
		std::uint8_t m_SectionNumber;

	  public:
		explicit Section(__uint128_t, __uint128_t, __uint128_t, std::string, std::uint8_t) noexcept;
		Section(const Section &) noexcept;
		Section &operator=(const Section &) noexcept;
		~Section(void) noexcept = default;

		// Getters
		inline const __uint128_t &GetVirtualStart(void) const noexcept;
		inline const __uint128_t &GetStart(void) const noexcept;
		inline const __uint128_t &GetSize(void) const noexcept;

		// This getter allows to get a vector, that CAN be changed!
		inline std::vector<std::pair<func_t, std::vector<std::uint8_t>>> &GetLabelsAndData(void) noexcept;

		// Still getters
		inline const std::string &GetSectionName(void) const noexcept;
		inline const std::uint8_t &GetSectionNumber(void) const noexcept;
	
		// Setter
		inline void SetSize(const __uint128_t &size) noexcept;

		// Not setter
		template <std::unsigned_integral T>
		std::vector<std::pair<func_t, std::vector<T>>> GetData(void) const;
	};

	inline const __uint128_t &Section::GetVirtualStart(void) const noexcept
	{ return this->m_VirtualStart; }
	inline const __uint128_t &Section::GetStart(void) const noexcept
	{ return this->m_Start; }
	inline const __uint128_t &Section::GetSize(void) const noexcept
	{ return this->m_Size; }

	inline std::vector<std::pair<func_t, std::vector<std::uint8_t>>> &Section::GetLabelsAndData(void) noexcept
	{ return this->m_LabelsAndData; }
	
	inline const std::string &Section::GetSectionName(void) const noexcept
	{ return this->m_SectionName; }
	inline const std::uint8_t &Section::GetSectionNumber(void) const noexcept
	{ return this->m_SectionNumber; }

	inline void Section::SetSize(const __uint128_t &size) noexcept
	{ this->m_Size = size; }

	template <std::unsigned_integral T>
	std::vector<std::pair<func_t, std::vector<T>>> Section::GetData(void) const
	{
		if constexpr (sizeof(T) == 1)
			return this->m_LabelsAndData;
	
		// Vector with labels and casted data
		std::vector<std::pair<func_t, std::vector<T>>> vec;
	
		for (const auto &[label, data] : this->m_LabelsAndData)
		{
			if (label.size % sizeof(T)) [[unlikely]]
				continue;
	
			std::vector<T> bytes;
			bytes.resize(label.size / sizeof(T));
			for (auto i{0UL}; i < data.size(); i += sizeof(T))
			{
				T var{0};
				for (unsigned short int j{0}; j < sizeof(T); ++j)
					var |= (data.at(i + j) << ((sizeof(T) - (1 + j)) * 8));
				bytes.at(i / sizeof(T)) = ntohl(var);
			}
	
			vec.emplace_back
			(
				std::pair<func_t, std::vector<T>>
				{
					label,
					std::move(bytes)
				}
			);
		}
	
		return vec;
	}
} /* disxx::loader::utility */
