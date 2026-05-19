module;

#include <disconf.hpp>

export module disxx.disasm.operand.SystemOperand;

import disxx.disasm.operand.AbstractOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ SystemOperand final : public AbstractOperand
	{
	  private:
		class __DISXX_PRIVATE__ [[nodiscard]] Impl;

	  public:
		explicit SystemOperand(void) noexcept;
		explicit SystemOperand(unsigned short int) noexcept;

		explicit SystemOperand(const SystemOperand &) noexcept(false);
		SystemOperand &operator=(const SystemOperand &) noexcept(false);

		explicit SystemOperand(SystemOperand &&) noexcept;
		SystemOperand &operator=(SystemOperand &&) noexcept;
  
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;
	};
} /* operand */
