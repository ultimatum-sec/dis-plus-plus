module;

#include <unordered_map>
#include <stdexcept>
#include <optional>
#include <utility>
#include <cassert>
#include <memory>
#include <string>

module disxx.disasm.operand.Register;

import :RegsTable;

namespace disxx::disasm::operand
{
	
	Register::Register(void) noexcept
		: AbstractOperand{}
		, m_VectorArrangementSpecifier{}
		, m_Identifier{}
		, m_StackPointer{}
	{}

	Register::Register(Type type, unsigned short int bits, unsigned short int size, bool sp) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_REGISTER}
		, m_VectorArrangementSpecifier{std::nullopt}
		, m_Identifier{}
		, m_Bits{bits}
		, m_Size{size}
		, m_StackPointer{sp}
	{
		if (bits == 0b11111 && !sp && type == Type::GPR)
			this->m_Identifier = static_cast<Identifier>();
	}

	Register::Register(const Register &other) noexcept
		: AbstractOperand{}
		, m_VectorArrangementSpecifier{other.m_VectorArrangementSpecifier}
		, m_Identifier{other.m_Identifier}
		, m_Bits{other.m_Bits}
		, m_Size{other.m_Size}
		, m_StackPointer{other.m_StackPointer}
	{}

	Register &Register::operator=(const Register &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_VectorArrangementSpecifier = other.m_VectorArrangementSpecifier;
			this->m_Identifier = other.m_Identifier;
			this->m_Bits = other.m_Bits;
			this->m_Size = other.m_Size;
			this->m_StackPointer = other.m_StackPointer;
		}		

		return *this;
	}

	Register::Register(Register &&other) noexcept
		: AbstractOperand{}
		, m_VectorArrangementSpecifier{std::move(other.m_VectorArrangementSpecifier)}
		, m_Identifier{std::move(other.m_Identifier)}
		, m_Bits{std::move(other.m_Bits)}
		, m_Size{std::move(other.m_Size)}
		, m_StackPointer{std::move(other.m_StackPointer)}
	{}


	Register &Register::operator=(Register &&other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_VectorArrangementSpecifier = std::move(other.m_VectorArrangementSpecifier);
			this->m_Identifier = std::move(other.m_Identifier);
			this->m_StackPointer = std::move(other.m_StackPointer);
		}

		return *this;
	}

	std::unique_ptr<AbstractOperand> Register::Clone(void) const noexcept
	{ return std::make_unique<Register>(*this); }
} /* operand */
