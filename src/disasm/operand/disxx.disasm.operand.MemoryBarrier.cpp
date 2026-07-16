module;

#include <unordered_map>
#include <stdexcept>
#include <format>

module disxx.disasm.operand.MemoryBarrier;

namespace disxx::disasm::operand
{
	MemoryBarrier::MemoryBarrier(void) noexcept
		: AbstractOperand{}
		, m_Identifier{}
		, m_Nxs{}
	{}

	MemoryBarrier::MemoryBarrier(unsigned short int bits, bool nXS) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_MEMORYBARRIER}
		, m_Identifier{static_cast<Type>(bits)}
		, m_Nxs{nXS}
	{}

	MemoryBarrier::MemoryBarrier(const MemoryBarrier &other) noexcept
		: AbstractOperand{}
		, m_Identifier{other.m_Identifier}
		, m_Nxs{other.m_Nxs}
	{}

	MemoryBarrier &MemoryBarrier::MemoryBarrier::operator=(const MemoryBarrier &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Identifier = other.m_Identifier;
			this->m_Nxs = other.m_Nxs;
		}

		return *this;
	}

	MemoryBarrier::MemoryBarrier(MemoryBarrier &&other) noexcept
		: AbstractOperand{}
		, m_Identifier{std::move(other.m_Identifier)}
		, m_Nxs{std::move(other.m_Nxs)}
	{}

	MemoryBarrier &MemoryBarrier::MemoryBarrier::operator=(MemoryBarrier &&other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Identifier = std::move(other.m_Identifier);
			this->m_Nxs = std::move(other.m_Nxs);
		}

		return *this;
	}

	std::unique_ptr<AbstractOperand> MemoryBarrier::Clone(void) const noexcept
	{ return std::make_unique<MemoryBarrier>(*this); }
} /* disxx::disasm::operand */
