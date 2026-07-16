module;

#include <disconf.hpp>

#include <unordered_map>

export module disxx.disasm.operand.Condition;

import disxx.disasm.operand.IOperand;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ Condition final : public IOperand
	{
	  public:
		enum class Identifier : unsigned short int
		{
			ID_EQ, ID_NE, ID_CS, ID_CC,
			ID_MI, ID_PL, ID_VS, ID_VC,
			ID_HI, ID_LS, ID_GE, ID_LT,
			ID_GT, ID_LE, ID_AL, ID_NV
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
 
        virtual std::unique_ptr<IOperand> Clone(void) const noexcept override;
	
		inline Identifier GetIdentifier(void) const noexcept;
	};

	inline Condition::Identifier Condition::GetIdentifier(void) const noexcept
	{ return this->m_Identifier; }
} /* disxx::disasm::operand */
