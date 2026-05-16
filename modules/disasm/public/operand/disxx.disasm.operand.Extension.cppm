module;

#include <disconf.hpp>

export module disxx.disasm.operand.Extension;

import disxx.disasm.operand.AbstractOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ Extension final : public AbstractOperand
	{
	  private:
		class __DISXX_PRIVATE__ [[nodiscard]] Impl;

	  public:
		explicit Extension(void) noexcept;
		explicit Extension(unsigned short int, unsigned short int) noexcept;

		explicit Extension(const Extension &) noexcept;
		Extension &operator=(const Extension &) noexcept(false);

		explicit Extension(Extension &&) noexcept;
		Extension &operator=(Extension &&) noexcept;
  
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;
	};
} /* operand */
