module;

#include <disconf.hpp>

#include <unordered_map>

export module disxx.disasm.operand.Condition;

import disxx.disasm.operand.AbstractOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ Condition final : public AbstractOperand
	{
	  public:
		enum class Identifier : unsigned short int
		{
			TYPE_EQ, TYPE_NE, TYPE_CS, TYPE_CC,
			TYPE_MI, TYPE_PL, TYPE_VS, TYPE_VC,
			TYPE_HI, TYPE_LS, TYPE_GE, TYPE_LT,
			TYPE_GT, TYPE_LE, TYPE_AL, TYPE_NV
		};
	
	  private:
		Identifier m_Identifier{};

	  public:
		explicit Condition(void) noexcept;
		explicit Condition(unsigned short int) noexcept;
		
		explicit Condition(const Condition &) noexcept;
		Condition &operator=(const Condition &) noexcept;

		explicit Condition(Condition &&) noexcept;
		Condition &operator=(Condition &&) noexcept;
 
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;
	
		inline Identifier GetIdentifier(void) const noexcept;
	};

	inline Condition::Identifier Condition::GetIdentifier(void) const noexcept
	{ return this->m_Identifier; }
} /* disxx::disasm::operand */
