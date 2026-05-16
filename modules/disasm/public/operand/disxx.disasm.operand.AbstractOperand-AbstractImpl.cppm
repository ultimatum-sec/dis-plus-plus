export module disxx.disasm.operand.AbstractOperand:AbstractImpl;

export namespace disxx::disasm::operand
{
	class AbstractOperand::AbstractImpl
    {
      private:
        OperandType m_Type;

       public:
        explicit AbstractImpl(OperandType) noexcept;

        explicit AbstractImpl(const AbstractImpl &) noexcept;
        AbstractImpl &operator=(const AbstractImpl &) noexcept;

        explicit AbstractImpl(const AbstractImpl &&) noexcept;
        AbstractImpl &operator=(const AbstractImpl &&) noexcept;

        virtual ~AbstractImpl(void) noexcept = default;

        OperandType GetType(void) const noexcept;
	    virtual std::string GetMnemonic(void) const noexcept(false) = 0;
	};
} /* operand */








