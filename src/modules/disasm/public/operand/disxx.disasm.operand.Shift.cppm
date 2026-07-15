module;

#include <disconf.hpp>

#include <unordered_map>

export module disxx.disasm.operand.Shift;

import disxx.disasm.operand.AbstractOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ Shift final : public AbstractOperand
	{
	  public:
		enum class Identifier : unsigned short int
		{
			ID_LSL = 0b000, ID_LSR = 0b001,
            ID_ASR = 0b010, ID_ROR = 0b011,
            ID_MSL = 0b100
		};

	  private:
		Identifier m_Identifier{};
		unsigned short int m_Amount{};

	  public:
		explicit Shift(void) noexcept;
		explicit Shift(unsigned short int, unsigned short int) noexcept;
		
		Shift(const Shift &) noexcept;
		Shift &operator=(const Shift &) noexcept;

		Shift(Shift &&) noexcept;
        Shift &operator=(Shift &&) noexcept;
       
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;
	
		inline Identifier GetIdentifier(void) const noexcept;
		inline unsigned short int GetAmount(void) const noexcept;
	};

	inline Shift::Identifier Shift::GetIdentifier(void) const noexcept
	{ return this->m_Identifier; }

	inline unsigned short int Shift::GetAmount(void) const noexcept
	{ return this->m_Amount; }
} /* operand */
