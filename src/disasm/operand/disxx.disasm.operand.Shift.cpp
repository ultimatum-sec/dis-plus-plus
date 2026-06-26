module;

#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <format>
#include <string>

module disxx.disasm.operand.Shift;

namespace disxx::disasm::operand
{
	Shift::Shift(void) noexcept
        : AbstractOperand{AbstractOperand::Type::TYPE_SHIFT}
        , m_ShiftType{}
        , m_Amount{}
    {}

    Shift::Shift(Type type, unsigned short int amount) noexcept
        : AbstractOperand{AbstractOperand::Type::TYPE_SHIFT}
		, m_ShiftType{type}
		, m_Amount{amount}
    {}

    Shift::Shift(const Shift &other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_SHIFT}
		, m_ShiftType{other.m_ShiftType}
        , m_Amount{other.m_Amount}
	{}

    Shift &Shift::operator=(const Shift &other) noexcept
    {
        if (this != &other) [[unlikely]]
		{
			this->m_ShiftType = other.m_ShiftType;
			this->m_Amount = other.m_Amount;
       	} 

		return *this;
    }

	Shift::Shift::Shift(Shift &&other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_SHIFT}
		, m_ShiftType{std::move(other.m_ShiftType)}
        , m_Amount{std::move(other.m_Amount)}
	{}

    Shift &Shift::operator=(Shift &&other) noexcept
    {
		this->m_ShiftType = std::move(other.m_ShiftType);
		this->m_Amount = std::move(other.m_Amount);

		return *this;
    }

	std::string Shift::Shift::GetMnemonic(void) const noexcept(false)
	{
		return std::format
		(
			"{} #0x{:x}",
			m_sShiftTable.at(this->m_ShiftType),	
			this->m_Amount
		);
	}

    const std::unordered_map<Shift::Type, const char *> Shift::m_sShiftTable = {
        {Shift::Type::SHIFT_LSL, "lsl"},
        {Shift::Type::SHIFT_LSR, "lsr"},
        {Shift::Type::SHIFT_ASR, "asr"},
        {Shift::Type::SHIFT_ROR, "ror"},
        {Shift::Type::SHIFT_MSL, "msl"}
    };

	std::unique_ptr<AbstractOperand> Shift::Clone(void) const noexcept
	{ return std::make_unique<Shift>(*this); }
} /* disxx::disasm::operand */
