module;

#include <disconf.hpp>

export module disxx.disasm.operand.AbstractOperand;

import disxx.utility.wrapper.Pointer;

export import <memory>;
export import <string>;

export namespace disxx::disasm::operand
{
    class __DISXX_EXPORT__ AbstractOperand
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

	  public:
		class __DISXX_PRIVATE__ [[nodiscard]] AbstractImpl
    	{
     	  private:
        	Type m_Type;
	  
	  	  public:
			explicit AbstractImpl(Type) noexcept;

        	//explicit AbstractImpl(const AbstractImpl &) noexcept;
        	//AbstractImpl &operator=(const AbstractImpl &) noexcept;

        	explicit AbstractImpl(AbstractImpl &&) noexcept;
        	AbstractImpl &operator=(AbstractImpl &&) noexcept;

        	virtual ~AbstractImpl(void) noexcept = default;

        	Type GetType(void) const noexcept;
			virtual std::string GetMnemonic(void) const noexcept(false) = 0;
		};

      protected:
		// Should be able to guarantee that pImpl isn't nullptr
		// when it dereferences
		utility::wrapper::Pointer<std::unique_ptr<AbstractImpl>> m_pImpl;

      public:
		explicit AbstractOperand(void) noexcept;
		explicit AbstractOperand(std::unique_ptr<AbstractImpl> &&) noexcept;

		//explicit AbstractOperand(const AbstractOperand &) noexcept(false);
		//AbstractOperand &operator=(const AbstractOperand &) noexcept(false);

		explicit AbstractOperand(AbstractOperand &&) noexcept;
		AbstractOperand &operator=(AbstractOperand &&) noexcept;

		virtual ~AbstractOperand(void) noexcept = default;
	
        Type GetType(void) const noexcept(false);
		std::string GetMnemonic(void) const noexcept(false);
    
		virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept = 0;
	};
} /* operand */
