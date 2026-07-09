module;

#include <disconf.hpp>

export module disxx.disasm.operand.AbstractOperand;

export import <memory>;
export import <string>;

export import disxx.utility.trait.Cloneable;

export namespace disxx::disasm::operand
{
    class __DISXX_EXPORT__ [[nodiscard]] AbstractOperand : public disxx::utility::trait::Cloneable<AbstractOperand>
    {
	  public:
		enum class Type
    	{
        	TYPE_LOADSANDSTORESADDRESS,
        	TYPE_PREFETCHOPERAND,
        	TYPE_MEMORYBARRIER,
        	TYPE_SYSTEMOPERAND,
        	TYPE_IMMEDIATE,
        	TYPE_EXTENSION,
        	TYPE_CONDITION,
        	TYPE_REGISTER,
        	TYPE_PSTATE,
			TYPE_SHIFT
    	};

	  private:
		Type m_Type{};
	  	  
      public:
		explicit AbstractOperand(void) noexcept;
		explicit AbstractOperand(Type) noexcept;

		AbstractOperand(AbstractOperand &&) noexcept;
		AbstractOperand &operator=(AbstractOperand &&) noexcept;

		virtual ~AbstractOperand(void) noexcept override = default;
	
        inline Type GetType(void) const noexcept;
		virtual std::string GetMnemonic(void) const noexcept(false) = 0;
	};

	inline AbstractOperand::Type AbstractOperand::GetType(void) const noexcept
	{ return this->m_Type; }
} /* operand */
