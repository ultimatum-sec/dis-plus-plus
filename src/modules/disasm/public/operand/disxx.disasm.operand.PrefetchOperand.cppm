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
		enum class Type : unsigned short int
		{
			TYPE_PLDL1KEEP,
			TYPE_PLDL1STRM,
            TYPE_PLDL2KEEP,
			TYPE_PLDL2STRM,
            TYPE_PLDL3KEEP,
			TYPE_PLDL3STRM,
            TYPE_PLDSLCKEEP,
			TYPE_PLDSLCSTRM,
            TYPE_PLIL1KEEP,
			TYPE_PLIL1STRM,
            TYPE_PLIL2KEEP,
			TYPE_PLIL2STRM,
			TYPE_PLIL3KEEP,
			TYPE_PLIL3STRM,
            TYPE_PLISLCKEEP,
			TYPE_PLISLCSTRM,
            TYPE_PSTL1KEEP,
			TYPE_PSTL1STRM,
            TYPE_PSTL2KEEP,
			TYPE_PSTL2STRM,
            TYPE_PSTL3KEEP,
			TYPE_PSTL3STRM,
            TYPE_PSTSLCKEEP,
			TYPE_PSTSLCSTRM,
			TYPE_IR,
			// Part for INSN_RPRFM
			TYPE_PLDKEEP = 0b010011000,
			TYPE_PSTKEEP = 0b010011001,
			TYPE_PLDSTRM = 0b010011100,
			TYPE_PSTSTRM = 0b010011101
		};
	  
	  private:
		static const std::unordered_map<Type, const char *> s_PrfopTable;

	  private:
		Type m_Prfop{};

	  public:
		explicit PrefetchOperand(void) noexcept;
		explicit PrefetchOperand(unsigned short int) noexcept;

		explicit PrefetchOperand(const PrefetchOperand &) noexcept;
		PrefetchOperand &operator=(const PrefetchOperand &) noexcept;

		explicit PrefetchOperand(PrefetchOperand &&) noexcept;
		PrefetchOperand &operator=(PrefetchOperand &&) noexcept;
 
		virtual std::string GetMnemonic(void) const noexcept(false) override; 
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;
	};
} /* operand */
