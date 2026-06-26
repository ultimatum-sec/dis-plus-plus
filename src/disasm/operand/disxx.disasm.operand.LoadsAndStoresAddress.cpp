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
		this->m_Modifier = std::move(other.m_Modifier);
		this->m_ExtraValue = std::move(other.m_ExtraValue);
		this->m_BaseRegister = std::move(other.m_BaseRegister);

		return *this;
	}

	void LoadsAndStoresAddress::AddImmediatePreIndexedOffset(const signed short int value, bool added) noexcept
	{
		assert(!this->m_ExtraValue && "Adding offset twice");
		// If added is true mnemonic will end with '!'
		this->m_ExtraValue.emplace
		(
			std::in_place_type<std::pair<signed short int, bool>>,
			std::make_pair(value, added)
		);
	}

	void LoadsAndStoresAddress::AddRegisterOffset(Register &&reg) noexcept
	{
		assert(!this->m_ExtraValue && "Adding offset twice");
		this->m_ExtraValue.emplace
		(
			std::in_place_type<Register>,
			std::move(reg)
		);
	}

	void LoadsAndStoresAddress::AddExtension(Extension &&extension) noexcept
	{
		assert(!this->m_Modifier && "Adding modifier twice");
		this->m_Modifier.emplace
		(
			std::in_place_type<Extension>,
			std::move(extension)
		);
	}

	void LoadsAndStoresAddress::AddShift(Shift &&shift) noexcept
	{
		assert(!this->m_Modifier && "Adding modifier twice");
		this->m_Modifier.emplace
		(
			std::in_place_type<Shift>,
			std::move(shift)
		);
	}

	std::string LoadsAndStoresAddress::GetMnemonic(void) const noexcept(false)
	{
		if (this->m_ExtraValue)
		{
			if (std::holds_alternative<std::pair<signed short int, bool>>(*this->m_ExtraValue))
			{
				const auto &[imm, isAdded]{std::get<std::pair<signed short int, bool>>(*this->m_ExtraValue)};
				return std::format
				(
					"[{}, #{:#x}]",
					this->m_BaseRegister.GetMnemonic(),
					imm
				) + (isAdded ? "!" : "");
			}
				
			auto result
			{
				std::format
				(
					"[{}, {}",
					this->m_BaseRegister.GetMnemonic(),
					std::get<disxx::disasm::operand::Register>(*this->m_ExtraValue).GetMnemonic()
				)
			};

			// Shift or extension?
			if (this->m_Modifier)
			{
				std::visit
				(
					[&result](auto &&modifier) -> void
					{
						result += std::format
						(
							", {}]",
							modifier.GetMnemonic()
						);
					},
					*this->m_Modifier
				);

				return result;
			}
			
			return result + "]";
		}

		return std::format("[{}]", this->m_BaseRegister.GetMnemonic());
	}

	std::unique_ptr<AbstractOperand> LoadsAndStoresAddress::Clone(void) const noexcept
	{ return std::make_unique<LoadsAndStoresAddress>(*this); }
} /* operand */
