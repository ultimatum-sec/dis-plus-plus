module;

#include <disconf.hpp>

#include <optional>
#include <variant>

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
		std::optional
		<
			std::variant
			<
				disxx::disasm::operand::Extension,
				disxx::disasm::operand::Shift
			>
		> m_Modifier{};
		std::optional
		<
			std::variant
			<
				disxx::disasm::operand::Register,
				std::pair
				<
					signed short int,
					bool
				>
			>
		> m_ExtraValue{};
		Register m_BaseRegister{};

	  public:
		explicit LoadsAndStoresAddress(void) noexcept;
		explicit LoadsAndStoresAddress(Register &&) noexcept;
		
		explicit LoadsAndStoresAddress(const LoadsAndStoresAddress &) noexcept;
		LoadsAndStoresAddress &operator=(const LoadsAndStoresAddress &) noexcept;

		explicit LoadsAndStoresAddress(LoadsAndStoresAddress &&) noexcept;
		LoadsAndStoresAddress &operator=(LoadsAndStoresAddress &&) noexcept;

		virtual std::string GetMnemonic(void) const noexcept(false) override;
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;

		void AddImmediatePreIndexedOffset(const signed short int, bool) noexcept;
		void AddRegisterOffset(disxx::disasm::operand::Register &&) noexcept;

		void AddExtension(Extension &&) noexcept;
		void AddShift(Shift &&) noexcept;
	};
} /* operand */
