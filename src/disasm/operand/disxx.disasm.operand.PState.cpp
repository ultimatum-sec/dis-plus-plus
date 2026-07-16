module;

#include <unordered_map>
#include <stdexcept>
#include <format>

module disxx.disasm.operand.PState;

namespace disxx::disasm::operand
{
	PState::PState(void) noexcept
		: AbstractOperand{}
		, m_Identifier{}
	{}

	PState::PState(unsigned short int bits) noexcept
		: AbstractOperand{}
		, m_Identifier{static_cast<Identifier>(bits)}
	{}

	PState::PState(const PState &other) noexcept
		: AbstractOperand{}
		, m_Identifier{other.m_Identifier}
	{}

	PState &PState::operator=(const PState &other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Identifier = other.m_Identifier;

		return *this;
	}

	PState::PState(PState &&other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_PSTATE}
		, m_Identifier{std::move(other.m_Identifier)}
	{}

	PState &PState::operator=(PState &&other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Identifier = std::move(other.m_Identifier);
		return *this;
	}

	std::unique_ptr<AbstractOperand> PState::Clone(void) const noexcept
	{ return std::make_unique<PState>(*this); }
} /* operand */
