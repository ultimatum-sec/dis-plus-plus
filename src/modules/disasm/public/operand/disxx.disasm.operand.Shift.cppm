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
		enum class Type : unsigned short int
		{
			SHIFT_LSL = 0b000, SHIFT_LSR = 0b001,
            SHIFT_ASR = 0b010, SHIFT_ROR = 0b011,
            SHIFT_MSL = 0b100
		};

	  private:
        static const std::unordered_map<Type, const char *> m_sShiftTable;

	  private:
		Type m_ShiftType{};
		unsigned short int m_Amount{};

	  public:
		explicit Shift(void) noexcept;
		explicit Shift(Type, unsigned short int) noexcept;
		
		explicit Shift(const Shift &) noexcept;
		Shift &operator=(const Shift &) noexcept;

		explicit Shift(Shift &&) noexcept;
        Shift &operator=(Shift &&) noexcept;
       
		std::string GetMnemonic(void) const noexcept(false) override; 
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;
	};
} /* operand */
