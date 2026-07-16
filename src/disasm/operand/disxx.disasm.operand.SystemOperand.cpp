module;

#include <unordered_map>
#include <stdexcept>

module disxx.disasm.operand.SystemOperand;

namespace disxx::disasm::operand
{
	SystemOperand::SystemOperand(void) noexcept
		: AbstractOperand{}
		, m_Identifier{}
	{}

	SystemOperand::SystemOperand(unsigned short int bits) noexcept
		: AbstractOperand{}
		, m_Identifier{static_cast<Identifier>(bits)}
	{}

	SystemOperand::SystemOperand(const SystemOperand &other) noexcept
		: AbstractOperand{}
		, m_Identifier{other.m_Identifier}
	{}

	SystemOperand &SystemOperand::operator=(const SystemOperand &other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Identifier = other.m_Identifier;
		return *this;
	}

	SystemOperand::SystemOperand(SystemOperand &&other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_SYSTEMOPERAND}
		, m_Identifier{std::move(other.m_Identifier)}
	{}

	SystemOperand &SystemOperand::operator=(SystemOperand &&other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Identifier = std::move(other.m_Identifier);
		return *this;
	}

	std::unique_ptr<AbstractOperand> SystemOperand::Clone(void) const noexcept
	{ return std::make_unique<SystemOperand>(*this); }
} /* disxx::disasm::operand */
