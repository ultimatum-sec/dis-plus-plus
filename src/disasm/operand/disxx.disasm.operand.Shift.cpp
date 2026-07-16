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
        : IOperand{}
        , m_Identifier{}
        , m_Amount{}
    {}

    Shift::Shift(unsigned short int bits, unsigned short int amount) noexcept
        : IOperand{}
		, m_Identifier{static_cast<Identifier>(bits)}
		, m_Amount{amount}
    {}

    Shift::Shift(const Shift &other) noexcept
		: IOperand{}
		, m_Identifier{other.m_Identifier}
        , m_Amount{other.m_Amount}
	{}

    Shift &Shift::operator=(const Shift &other) noexcept
    {
        if (this != &other) [[unlikely]]
		{
			this->m_Identifier = other.m_Identifier;
			this->m_Amount = other.m_Amount;
       	} 

		return *this;
    }

	Shift::Shift::Shift(Shift &&other) noexcept
		: IOperand{}
		, m_Identifier{std::move(other.m_Identifier)}
        , m_Amount{std::move(other.m_Amount)}
	{}

    Shift &Shift::operator=(Shift &&other) noexcept
    {
		if (this != &other) [[likely]]
		{
			this->m_Identifier = std::move(other.m_Identifier);
			this->m_Amount = std::move(other.m_Amount);
		}

		return *this;
    }
	
	std::unique_ptr<IOperand> Shift::Clone(void) const noexcept
	{ return std::make_unique<Shift>(*this); }
} /* disxx::disasm::operand */
