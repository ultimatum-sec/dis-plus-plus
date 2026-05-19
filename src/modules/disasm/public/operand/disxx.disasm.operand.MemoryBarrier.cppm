module;

#include <disconf.hpp>

export module disxx.disasm.operand.MemoryBarrier;

import disxx.disasm.operand.AbstractOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ MemoryBarrier final : public AbstractOperand
	{
	  private:
		class __DISXX_PRIVATE__ [[nodiscard]] Impl;

	  public:
		explicit MemoryBarrier(void) noexcept;
		explicit MemoryBarrier(unsigned short int, bool) noexcept;

		explicit MemoryBarrier(const MemoryBarrier &) noexcept(false);
		MemoryBarrier &operator=(const MemoryBarrier &) noexcept(false);

		explicit MemoryBarrier(MemoryBarrier &&) noexcept;
		MemoryBarrier &operator=(MemoryBarrier &&) noexcept;
  
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;
	};
} /* operand */
