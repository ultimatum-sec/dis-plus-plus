module;

#include <disconf.hpp>

export module disxx.disasm.operand.PrefetchOperand;

import disxx.disasm.operand.AbstractOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ PrefetchOperand final : public AbstractOperand
	{
	  private:
		class __DISXX_PRIVATE__ [[nodiscard]] Impl;

	  public:
		explicit PrefetchOperand(void) noexcept;
		explicit PrefetchOperand(unsigned short int) noexcept;

		explicit PrefetchOperand(const PrefetchOperand &) noexcept(false);
		PrefetchOperand &operator=(const PrefetchOperand &) noexcept(false);

		explicit PrefetchOperand(PrefetchOperand &&) noexcept;
		PrefetchOperand &operator=(PrefetchOperand &&) noexcept;
  
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;
	};
} /* operand */
