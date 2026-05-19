export module disxx.disasm.operand.AbstractOperand:OperandType;

export namespace disxx::disasm::operand
{
	enum class AbstractOperand::OperandType
	{
		TYPE_LOADSANDSTORESADDRESS,
		TYPE_PREFETCHOPERAND,
		TYPE_MEMORYBARRIER,
		TYPE_SYSTEMOPERAND,
		TYPE_IMMEDIATE,
		TYPE_EXTENSION,
		TYPE_CONDITION,
		TYPE_REGISTER,
		TYPE_SHIFT
	};
} /* operand */








