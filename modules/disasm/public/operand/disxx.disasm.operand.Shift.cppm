module;

#include <disconf.hpp>

export module disxx.disasm.operand.Shift;

import disxx.disasm.operand.AbstractOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ Shift final : public AbstractOperand
	{
	  private:
		class __DISXX_PRIVATE__ [[nodiscard]] Impl;

	  public:
		enum class Type : unsigned short int
		{
			SHIFT_LSL = 0b000, SHIFT_LSR = 0b001,
            SHIFT_ASR = 0b010, SHIFT_ROR = 0b011,
            SHIFT_MSL = 0b100
		};

	  public:
		explicit Shift(void) noexcept;
		explicit Shift(Type, unsigned short int) noexcept;
		
		explicit Shift(const Shift &) noexcept(false);
		Shift &operator=(const Shift &) noexcept(false);

		explicit Shift(Shift &&) noexcept;
        Shift &operator=(Shift &&) noexcept;
        
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;
	};
} /* operand */
