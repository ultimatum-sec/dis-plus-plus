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
		: AbstractOperand{AbstractOperand::Type::TYPE_REGISTER}
		, m_VectorArrangementSpecifier{std::nullopt}
		, m_RegType{Type::TYPE_GPR}
		, m_Bits{0b00000}
		, m_Size{0}
		, m_ExcludeZero{false}
	{}

	Register::Register(Type type, unsigned short int bits, unsigned short int size, bool excludeZero) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_REGISTER}
		, m_VectorArrangementSpecifier{std::nullopt}
		, m_RegType{type}
		, m_Bits{bits}
		, m_Size{size}
		, m_ExcludeZero{excludeZero}
	{}

	Register::Register(const Register &other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_REGISTER}
		, m_VectorArrangementSpecifier{other.m_VectorArrangementSpecifier}
		, m_RegType{other.m_RegType}
		, m_Bits{other.m_Bits}
		, m_Size{other.m_Size}
		, m_ExcludeZero{other.m_ExcludeZero}
	{}

	Register &Register::operator=(const Register &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_VectorArrangementSpecifier = other.m_VectorArrangementSpecifier;
			this->m_RegType = other.m_RegType;
			this->m_Bits = other.m_Bits;
			this->m_Size = other.m_Size;
			this->m_ExcludeZero = other.m_ExcludeZero;
		}		

		return *this;
	}

	Register::Register(Register &&other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_REGISTER}
		, m_VectorArrangementSpecifier{std::move(other.m_VectorArrangementSpecifier)}
		, m_RegType{std::move(other.m_RegType)}
		, m_Bits{std::move(other.m_Bits)}
		, m_Size{std::move(other.m_Size)}
		, m_ExcludeZero{std::move(other.m_ExcludeZero)}
	{}


	Register &Register::operator=(Register &&other) noexcept
	{
		this->m_VectorArrangementSpecifier = std::move(other.m_VectorArrangementSpecifier);
		this->m_RegType = std::move(other.m_RegType);
		this->m_Bits = std::move(other.m_Bits);
		this->m_Size = std::move(other.m_Size);
		this->m_ExcludeZero = std::move(other.m_ExcludeZero);

		return *this;
	}

	std::string Register::GetMnemonic(void) const noexcept(false)
	{
		switch (this->m_RegType)
		{
		  case Type::TYPE_GPR:
            switch (this->m_Size)
            {
              case 32:
                if (!this->m_ExcludeZero && this->m_Bits == 0b11111)
                    return "wzr";
                return RegsTable::m_sGPR32Table.at(static_cast<GPR32>(this->m_Bits));

              case 64:
                if (!this->m_ExcludeZero && this->m_Bits == 0b11111)
                    return "xzr";
                return RegsTable::m_sGPR64Table.at(static_cast<GPR64>(this->m_Bits));

              default:
                throw std::invalid_argument{"RegisterEncodingError"};
            }

		  case Type::TYPE_NEON:
            switch (this->m_Size)
            {
              case 8:
                return RegsTable::m_sNEON8Table.at(static_cast<NEON8>(this->m_Bits));

              case 16:
                return RegsTable::m_sNEON16Table.at(static_cast<NEON16>(this->m_Bits));

              case 32:
                return RegsTable::m_sNEON32Table.at(static_cast<NEON32>(this->m_Bits));

              case 64:
                return RegsTable::m_sNEON64Table.at(static_cast<NEON64>(this->m_Bits));

              case 128:
                return RegsTable::m_sNEON128Table.at(static_cast<NEON128>(this->m_Bits));

              case 128 + 'V':
                return std::string{RegsTable::m_sNEON128VTable.at(static_cast<NEON128V>(this->m_Bits))}
					+ this->m_VectorArrangementSpecifier.value_or("");

              default:
                throw std::invalid_argument{"RegisterEncodingError"};
            }
		 
		  default:
			return RegsTable::m_sSYSREGTable.at(static_cast<SYSREG>(this->m_Bits));
		}
	}

	std::unique_ptr<AbstractOperand> Register::Clone(void) const noexcept
	{ return std::make_unique<Register>(*this); }

	void Register::SetArrangementSpecifier(const std::string spec) noexcept
	{
		assert(!this->m_VectorArrangementSpecifier && "Setting vector arrangement specifier twice");
		assert(this->m_RegType == Type::TYPE_NEON && "Register should has vector type");

		this->m_VectorArrangementSpecifier = spec.starts_with("[")
			? std::string{""}
			: std::string{"."}
			+ spec;
	}
} /* operand */
