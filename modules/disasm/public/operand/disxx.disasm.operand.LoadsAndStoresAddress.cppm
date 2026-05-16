module;

#include <disconf.hpp>

export module disxx.disasm.operand.LoadsAndStoresAddress;

import disxx.disasm.operand.AbstractOperand;

export import disxx.disasm.operand.Extension;
export import disxx.disasm.operand.Register;
export import disxx.disasm.operand.Shift;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ LoadsAndStoresAddress final : public AbstractOperand
	{
	  private:
		class __DISXX_PRIVATE__ [[nodiscard]] Impl;

	  public:
		explicit LoadsAndStoresAddress(operand::Register::Type, unsigned short int, unsigned short int, bool = false) noexcept;
		
		explicit LoadsAndStoresAddress(const LoadsAndStoresAddress &) noexcept(false);
		LoadsAndStoresAddress &operator=(const LoadsAndStoresAddress &) noexcept(false);

		explicit LoadsAndStoresAddress(LoadsAndStoresAddress &&) noexcept;
		LoadsAndStoresAddress &operator=(LoadsAndStoresAddress &&) noexcept;

        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;

		void AddImmediatePreIndexedOffset(const signed short int, bool) noexcept(false);
		void AddRegisterOffset(disxx::disasm::operand::Register &&) noexcept(false);

		void AddExtension(unsigned short int, unsigned short int) noexcept(false);
		void AddShift(Shift::Type, unsigned short int) noexcept(false);
	};
} /* operand */
