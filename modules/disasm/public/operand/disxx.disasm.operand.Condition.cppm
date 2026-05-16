module;

#include <disconf.hpp>

export module disxx.disasm.operand.Condition;

import disxx.disasm.operand.AbstractOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ Condition final : public AbstractOperand
	{
	  private:
		class __DISXX_PRIVATE__ [[nodiscard]] Impl;

	  public:
		explicit Condition(void) noexcept;
		explicit Condition(unsigned short int) noexcept;
		
		explicit Condition(const Condition &) noexcept(false);
		Condition &operator=(const Condition &) noexcept(false);

		explicit Condition(Condition &&) noexcept;
		Condition &operator=(Condition &&) noexcept;
  
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;
	};
} /* operand */
