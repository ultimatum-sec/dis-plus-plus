module;

#include <unordered_map>
#include <stdexcept>
#include <utility>
#include <format>

module disxx.disasm.operand.Extension;

namespace disxx::disasm::operand
{
	Extension::Extension(void) noexcept
		: IOperand{}
		, m_Identifier{}
		, m_Value{}
	{}

	Extension::Extension(unsigned short int id, unsigned short int value) noexcept
		: IOperand{}
		, m_Identifier{static_cast<Identifier>(id)}
		, m_Value{value}
	{}

	Extension::Extension(const Extension &other) noexcept
		: IOperand{}
		, m_Identifier{other.m_Identifier}
		, m_Value{other.m_Value}
	{}

	Extension &Extension::operator=(const Extension &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Identifier = other.m_Identifier;
			this->m_Value = other.m_Value;
		}

		return *this;
	}

	Extension::Extension(Extension &&other) noexcept
		: IOperand{}
		, m_Identifier{std::move(other.m_Identifier)}
		, m_Value{std::move(other.m_Value)}
	{}

	Extension &Extension::operator=(Extension &&other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Identifier = std::move(other.m_Identifier);
			this->m_Value = std::move(other.m_Value);
		}

		return *this;
	}

	std::unique_ptr<IOperand> Extension::Clone(void) const noexcept
	{ return std::make_unique<Extension>(*this); }
} /* operand */
