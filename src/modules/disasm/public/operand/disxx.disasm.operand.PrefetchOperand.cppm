module;

#include <disconf.hpp>

#include <unordered_map>

export module disxx.disasm.operand.PrefetchOperand;

import disxx.disasm.operand.AbstractOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ PrefetchOperand final : public AbstractOperand
	{
	  private:
		enum class Identifier : unsigned short int
		{
			ID_PLDL1KEEP,
			ID_PLDL1STRM,
            ID_PLDL2KEEP,
			ID_PLDL2STRM,
            ID_PLDL3KEEP,
			ID_PLDL3STRM,
            ID_PLDSLCKEEP,
			ID_PLDSLCSTRM,
            ID_PLIL1KEEP,
			ID_PLIL1STRM,
            ID_PLIL2KEEP,
			ID_PLIL2STRM,
			ID_PLIL3KEEP,
			ID_PLIL3STRM,
            ID_PLISLCKEEP,
			ID_PLISLCSTRM,
            ID_PSTL1KEEP,
			ID_PSTL1STRM,
            ID_PSTL2KEEP,
			ID_PSTL2STRM,
            ID_PSTL3KEEP,
			ID_PSTL3STRM,
            ID_PSTSLCKEEP,
			ID_PSTSLCSTRM,
			ID_IR,
			// Part for INSN_RPRFM
			ID_PLDKEEP = 0b010011000,
			ID_PSTKEEP = 0b010011001,
			ID_PLDSTRM = 0b010011100,
			ID_PSTSTRM = 0b010011101
		};
	  
	  private:
		Identifier m_Identifier{};

	  public:
		explicit PrefetchOperand(void) noexcept;
		explicit PrefetchOperand(unsigned short int) noexcept;

		PrefetchOperand(const PrefetchOperand &) noexcept;
		PrefetchOperand &operator=(const PrefetchOperand &) noexcept;

		PrefetchOperand(PrefetchOperand &&) noexcept;
		PrefetchOperand &operator=(PrefetchOperand &&) noexcept;
 
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;
	
		inline Identifier GetIdentifier(void) const noexcept;
	};

	inline PrefetchOperand::Identifier PrefetchOperand::GetIdentifier(void) const noexcept
	{ return this->m_Identifier; }
} /* operand */
