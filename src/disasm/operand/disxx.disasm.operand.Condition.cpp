module;

#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <string>

module disxx.disasm.operand.Condition;

namespace disxx::disasm::operand
{
	Condition::Condition(void) noexcept
		: IOperand{}
		, m_Identifier{}
	{}

	Condition::Condition(unsigned short int val) noexcept
		: IOperand{}
		, m_Identifier{static_cast<Identifier>(val)}
	{}

	Condition::Condition(const Condition &other) noexcept
		: IOperand{}
		, m_Identifier{other.m_Identifier}
	{}

	Condition &Condition::operator=(const Condition &other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Identifier = other.m_Identifier;
		return *this;
	}

	Condition::Condition(Condition &&other) noexcept
		: IOperand{}
		, m_Identifier{std::move(other.m_Identifier)}
	{}

	Condition &Condition::operator=(Condition &&other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Identifier = std::move(other.m_Identifier);
		return *this;
	}

	std::unique_ptr<IOperand> Condition::Clone(void) const noexcept
	{ return std::make_unique<Condition>(*this); }
} /* disxx::disasm::operand */
