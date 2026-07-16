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
        : AbstractOperand{}
        , m_Identifier{}
        , m_Amount{}
    {}

    Shift::Shift(Identifier id, unsigned short int amount) noexcept
        : AbstractOperand{}
		, m_Identifier{id}
		, m_Amount{amount}
    {}

    Shift::Shift(const Shift &other) noexcept
		: AbstractOperand{}
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
		: AbstractOperand{}
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
	
	std::unique_ptr<AbstractOperand> Shift::Clone(void) const noexcept
	{ return std::make_unique<Shift>(*this); }
} /* disxx::disasm::operand */
