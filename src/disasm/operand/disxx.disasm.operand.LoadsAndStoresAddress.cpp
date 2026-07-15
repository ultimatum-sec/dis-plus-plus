module;

#include <stdexcept>
#include <utility>
#include <variant>
#include <cassert>
#include <format>
#include <string>
#include <cmath>

module disxx.disasm.operand.LoadsAndStoresAddress;

namespace disxx::disasm::operand
{
	LoadsAndStoresAddress::LoadsAndStoresAddress(void) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_LOADSANDSTORESADDRESS}
		, m_Modifier{std::nullopt}
		, m_ExtraValue{std::nullopt}
		, m_BaseRegister{}
	{}

	LoadsAndStoresAddress::LoadsAndStoresAddress(Register &&reg) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_LOADSANDSTORESADDRESS}
		, m_Modifier{std::nullopt}
		, m_ExtraValue{std::nullopt}
		, m_BaseRegister{std::forward<Register &&>(reg)}
	{}

	LoadsAndStoresAddress::LoadsAndStoresAddress(const LoadsAndStoresAddress &other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_LOADSANDSTORESADDRESS}
        , m_Modifier{other.m_Modifier}
		, m_ExtraValue{other.m_ExtraValue}
		, m_BaseRegister{other.m_BaseRegister}
	{}

	LoadsAndStoresAddress &LoadsAndStoresAddress::operator=(const LoadsAndStoresAddress &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Modifier = other.m_Modifier;
			this->m_ExtraValue = other.m_ExtraValue;
			this->m_BaseRegister = other.m_BaseRegister;
		}

		return *this;
	}

	LoadsAndStoresAddress::LoadsAndStoresAddress(LoadsAndStoresAddress &&other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_LOADSANDSTORESADDRESS}
		, m_Modifier{std::move(other.m_Modifier)}
        , m_ExtraValue{std::move(other.m_ExtraValue)}
		, m_BaseRegister{std::move(other.m_BaseRegister)}
	{}

	LoadsAndStoresAddress &LoadsAndStoresAddress::operator=(LoadsAndStoresAddress &&other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Modifier = std::move(other.m_Modifier);
			this->m_ExtraValue = std::move(other.m_ExtraValue);
			this->m_BaseRegister = std::move(other.m_BaseRegister);
		}

		return *this;
	}

	std::unique_ptr<AbstractOperand> LoadsAndStoresAddress::Clone(void) const noexcept
	{ return std::make_unique<LoadsAndStoresAddress>(*this); }
} /* operand */
