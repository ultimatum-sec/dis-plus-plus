module;

#include <disconf.hpp>

export module disxx.disasm.operand.PState;

import disxx.disasm.operand.AbstractOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ PState final : public AbstractOperand
	{
	  private:
		class __DISXX_PRIVATE__ [[nodiscard]] Impl;

	  public:
		explicit PState(void) noexcept;
		explicit PState(unsigned short int) noexcept;

		explicit PState(const PState &) noexcept(false);
		PState &operator=(const PState &) noexcept(false);

		explicit PState(PState &&) noexcept;
		PState &operator=(PState &&) noexcept;
  
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;
	};
} /* operand */
