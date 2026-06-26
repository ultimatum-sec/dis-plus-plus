module;

#include <unordered_map>
#include <stdexcept>
#include <format>

module disxx.disasm.operand.MemoryBarrier;

namespace disxx::disasm::operand
{
	MemoryBarrier::MemoryBarrier(void) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_MEMORYBARRIER}
		, m_Barrier{}
		, m_NXS{}
	{}

	MemoryBarrier::MemoryBarrier(unsigned short int bits, bool nXS) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_MEMORYBARRIER}
		, m_Barrier{static_cast<Type>(bits)}
		, m_NXS{nXS}
	{}

	MemoryBarrier::MemoryBarrier(const MemoryBarrier &other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_MEMORYBARRIER}
		, m_Barrier{other.m_Barrier}
		, m_NXS{other.m_NXS}
	{}

	MemoryBarrier &MemoryBarrier::MemoryBarrier::operator=(const MemoryBarrier &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Barrier = other.m_Barrier;
			this->m_NXS = other.m_NXS;
		}

		return *this;
	}

	MemoryBarrier::MemoryBarrier(MemoryBarrier &&other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_MEMORYBARRIER}
		, m_Barrier{std::move(other.m_Barrier)}
		, m_NXS{std::move(other.m_NXS)}
	{}

	MemoryBarrier &MemoryBarrier::MemoryBarrier::operator=(MemoryBarrier &&other) noexcept
	{
		this->m_Barrier = std::move(other.m_Barrier);
		this->m_NXS = std::move(other.m_NXS);

		return *this;
	}

	std::string MemoryBarrier::MemoryBarrier::GetMnemonic(void) const noexcept(false)
	{
		return std::string
		{
			s_BarrierTable
				.at(this->m_Barrier)
		} + (this->m_NXS ? "nXS" : "");
	}

	const std::unordered_map<MemoryBarrier::MemoryBarrier::Type, const char *> MemoryBarrier::MemoryBarrier::s_BarrierTable = {
		{Type::TYPE_OSHLD,	"oshld"},
        {Type::TYPE_OSHST,	"oshst"},
        {Type::TYPE_OSH,	"osh"},
        {Type::TYPE_NSHLD,	"nshld"},
        {Type::TYPE_NSHST,	"nshst"},
        {Type::TYPE_NSH,	"nsh"},
        {Type::TYPE_ISHLD,	"ishld"},
        {Type::TYPE_ISHST,	"ishst"},
        {Type::TYPE_ISH,	"ish"},
        {Type::TYPE_LD,		"ld"},
        {Type::TYPE_ST,		"st"},
        {Type::TYPE_SY,		"sy"}
	};

	std::unique_ptr<AbstractOperand> MemoryBarrier::Clone(void) const noexcept
	{ return std::make_unique<MemoryBarrier>(*this); }
} /* disxx::disasm::operand */
