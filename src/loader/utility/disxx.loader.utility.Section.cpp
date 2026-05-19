module;

#include <utility>
#include <cstdint>
#include <string>
#include <vector>

module disxx.loader.utility.Section;

namespace disxx::loader::utility
{
	Section::Section
	(
		__uint128_t vstart, __uint128_t start,
		__uint128_t size, std::string name,
		std::uint8_t num
	) noexcept
		: m_VirtualStart{vstart}
		, m_Start{start}
		, m_Size{size}
		, m_LabelsAndData{}
		, m_SectionName{name}
		, m_SectionNumber{num}
	{}

	Section::Section(const Section &other) noexcept
		: m_VirtualStart{other.m_VirtualStart}
		, m_Start{other.m_Start}
		, m_Size{other.m_Size}
		, m_SectionName{other.m_SectionName}
		, m_SectionNumber{other.m_SectionNumber}
	{
		this->m_LabelsAndData.resize(other.m_LabelsAndData.size());
		for (auto idx{0}; const auto &[label, data] : other.m_LabelsAndData)
			this->m_LabelsAndData.at(idx) = std::pair<func_t, std::vector<std::uint8_t>>{label, data};
	}

	Section &Section::operator=(const Section &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_VirtualStart = other.m_VirtualStart;
			this->m_Start = other.m_Start;
			this->m_Size = other.m_Size;
			this->m_SectionName = other.m_SectionName;
			this->m_SectionNumber = other.m_SectionNumber;	

			this->m_LabelsAndData.resize(other.m_LabelsAndData.size());
			for (auto idx{0}; const auto &[label, data] : other.m_LabelsAndData)
				this->m_LabelsAndData.at(idx) = std::pair<func_t, std::vector<std::uint8_t>>{label, data};
		}

		return *this;
	}
} /* disxx::loader::utility */
