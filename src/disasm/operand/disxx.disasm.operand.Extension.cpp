module;

#include <unordered_map>
#include <stdexcept>
#include <utility>
#include <format>

module disxx.disasm.operand.Extension;

namespace disxx::disasm::operand
{
	Extension::Extension(void) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_EXTENSION}
		, m_ExtensionType{}
		, m_Val{}
	{}

	Extension::Extension(unsigned short int type, unsigned short int val) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_EXTENSION}
		, m_ExtensionType{static_cast<Type>(type)}
		, m_Val{val}
	{}

	Extension::Extension(const Extension &other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_EXTENSION}
		, m_ExtensionType{other.m_ExtensionType}
		, m_Val{other.m_Val}
	{}

	Extension &Extension::operator=(const Extension &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_ExtensionType = other.m_ExtensionType;
			this->m_Val = other.m_Val;
		}

		return *this;
	}

	Extension::Extension(Extension &&other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_EXTENSION}
		, m_ExtensionType{std::move(other.m_ExtensionType)}
		, m_Val{std::move(other.m_Val)}
	{}

	Extension &Extension::operator=(Extension &&other) noexcept
	{
		this->m_ExtensionType = std::move(other.m_ExtensionType);
		this->m_Val = std::move(other.m_Val);

		return *this;
	}

	std::string Extension::GetMnemonic(void) const noexcept(false)
	{
		return std::format
		(
			"{} #0x{:x}",
			s_ExtensionTable.at(this->m_ExtensionType),
			this->m_Val
		);
	}

	const std::unordered_map<Extension::Extension::Type, const char *> Extension::s_ExtensionTable = {
		{Type::TYPE_UXTB, "uxtb"},
		{Type::TYPE_UXTH, "uxth"},
		{Type::TYPE_UXTW, "uxtw"},
		{Type::TYPE_UXTX, "uxtx"},
		{Type::TYPE_SXTB, "sxtb"},
		{Type::TYPE_SXTH, "sxth"},
		{Type::TYPE_SXTW, "sxtw"},
		{Type::TYPE_SXTX, "sxtx"}
	};

	std::unique_ptr<AbstractOperand> Extension::Clone(void) const noexcept
	{ return std::make_unique<Extension>(*this); }
} /* operand */
