module;

#include <unordered_map>
#include <stdexcept>
#include <optional>
#include <utility>
#include <cassert>
#include <memory>
#include <string>

module disxx.disasm.operand.Register;

namespace disxx::disasm::operand
{
	
	Register::Register(void) noexcept
		: IOperand{}
		, m_VectorArrangementSpecifier{}
		, m_Identifier{}
		, m_PointsToStack{}
	{}

	Register::Register(Type type, unsigned short int bits, bool sp) noexcept
		: IOperand{}
		, m_VectorArrangementSpecifier{std::nullopt}
		, m_Identifier{static_cast<Identifier>(static_cast<unsigned short int>(type) + bits)}
		, m_PointsToStack{sp}
	{}

	Register::Register(const Register &other) noexcept
		: IOperand{}
		, m_VectorArrangementSpecifier{other.m_VectorArrangementSpecifier}
		, m_Identifier{other.m_Identifier}
		, m_PointsToStack{other.m_PointsToStack}
	{}

	Register &Register::operator=(const Register &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_VectorArrangementSpecifier = other.m_VectorArrangementSpecifier;
			this->m_Identifier = other.m_Identifier;
			this->m_PointsToStack = other.m_PointsToStack;
		}		

		return *this;
	}

	Register::Register(Register &&other) noexcept
		: IOperand{}
		, m_VectorArrangementSpecifier{std::move(other.m_VectorArrangementSpecifier)}
		, m_Identifier{std::move(other.m_Identifier)}
		, m_PointsToStack{std::move(other.m_PointsToStack)}
	{}


	Register &Register::operator=(Register &&other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_VectorArrangementSpecifier = std::move(other.m_VectorArrangementSpecifier);
			this->m_Identifier = std::move(other.m_Identifier);
			this->m_PointsToStack = std::move(other.m_PointsToStack);
		}

		return *this;
	}

	std::unique_ptr<IOperand> Register::Clone(void) const noexcept
	{ return std::make_unique<Register>(*this); }
} /* operand */
