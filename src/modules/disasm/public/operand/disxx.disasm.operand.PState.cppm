module;

#include <disconf.hpp>

#include <unordered_map>

export module disxx.disasm.operand.PState;

import disxx.disasm.operand.IOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ PState final : public IOperand
	{
	  public:
		enum class Identifier : unsigned short int
		{
			ID_ZA			= 0b000001,
			ID_SM			= 0b000010,
			ID_UAO			= 0b000011,
			ID_PAN			= 0b000100,
			ID_SPSEL		= 0b000101,
			ID_SSBS			= 0b011001,
			ID_DIT			= 0b011010,
			ID_TCO			= 0b011100,
			ID_DAIFSET		= 0b011110,
			ID_DAIFCLR		= 0b011111,
			// PState encoding with CRm in it
			ID_ALLINT		= 0b0000001000,
			ID_PM			= 0b0010001000,
			ID_SVCRSM		= 0b0010011011,
			ID_SVCRZA		= 0b0100011011,
			ID_SVCRSMZA		= 0b0110011011
		};

	  private:
		Identifier m_Identifier{};

	  public:
		explicit PState(void) noexcept;
		explicit PState(unsigned short int) noexcept;

		PState(const PState &) noexcept;
		PState &operator=(const PState &) noexcept;

		PState(PState &&) noexcept;
		PState &operator=(PState &&) noexcept;

        virtual std::unique_ptr<IOperand> Clone(void) const noexcept override;
	
		Identifier GetIdentifier(void) const noexcept;
	};

	PState::Identifier PState::GetIdentifier(void) const noexcept
	{ return this->m_Identifier; }
} /* operand */
