module;

#include <disconf.hpp>

#include <unordered_map>

export module disxx.disasm.operand.PState;

import disxx.disasm.operand.AbstractOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ PState final : public AbstractOperand
	{
	  private:
		enum class Type : unsigned short int
		{
			TYPE_ZA			= 0b000001,
			TYPE_SM			= 0b000010,
			TYPE_UAO		= 0b000011,
			TYPE_PAN		= 0b000100,
			TYPE_SPSEL		= 0b000101,
			TYPE_SSBS		= 0b011001,
			TYPE_DIT		= 0b011010,
			TYPE_TCO		= 0b011100,
			TYPE_DAIFSET	= 0b011110,
			TYPE_DAIFCLR	= 0b011111,
			// PState encoding with CRm in it
			TYPE_ALLINT		= 0b0000001000,
			TYPE_PM			= 0b0010001000,
			TYPE_SVCRSM		= 0b0010011011,
			TYPE_SVCRZA		= 0b0100011011,
			TYPE_SVCRSMZA	= 0b0110011011
		};

	  private:
		static const std::unordered_map<Type, const char *> s_PStateTable;

	  private:
		Type m_PState{};

	  public:
		explicit PState(void) noexcept;
		explicit PState(unsigned short int) noexcept;

		explicit PState(const PState &) noexcept;
		PState &operator=(const PState &) noexcept;

		explicit PState(PState &&) noexcept;
		PState &operator=(PState &&) noexcept;

		std::string GetMnemonic(void) const noexcept(false) override;  
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;
	};
} /* operand */
