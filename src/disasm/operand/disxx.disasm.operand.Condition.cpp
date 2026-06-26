module;

#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <string>

module disxx.disasm.operand.Condition;

namespace disxx::disasm::operand
{
	Condition::Condition(void) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_CONDITION}
		, m_Value{}
	{}

	Condition::Condition(unsigned short int val) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_CONDITION}
		, m_Value{static_cast<Type>(val)}
	{}

	Condition::Condition(const Condition &other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_CONDITION}
		, m_Value{other.m_Value}
	{}

	Condition &Condition::operator=(const Condition &other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Value = other.m_Value;
		return *this;
	}

	Condition::Condition(Condition &&other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_CONDITION}
		, m_Value{std::move(other.m_Value)}
	{}

	Condition &Condition::operator=(Condition &&other) noexcept
	{
		this->m_Value = std::move(other.m_Value);
		return *this;
	}

	std::string Condition::GetMnemonic(void) const noexcept(false)
	{
		return m_sCondTable
			.at(this->m_Value);
	}

	const std::unordered_map<Condition::Condition::Type, const char *> Condition::Condition::m_sCondTable = {
		{Type::TYPE_EQ, "eq"}, 
		{Type::TYPE_NE, "ne"},
        {Type::TYPE_CS, "cs"},
        {Type::TYPE_CC, "cc"},
        {Type::TYPE_MI, "mi"},
        {Type::TYPE_PL, "pl"},
        {Type::TYPE_VS, "vs"},
        {Type::TYPE_VC, "vc"},
        {Type::TYPE_HI, "hi"},
        {Type::TYPE_LS, "ls"},
        {Type::TYPE_GE, "ge"},
        {Type::TYPE_LT, "lt"},
        {Type::TYPE_GT, "gt"},
        {Type::TYPE_LE, "le"},
        {Type::TYPE_AL, "al"},
        {Type::TYPE_NV, "nv"}
	};

	std::unique_ptr<AbstractOperand> Condition::Clone(void) const noexcept
	{ return std::make_unique<Condition>(*this); }
} /* disxx::disasm::operand */
