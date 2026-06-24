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
	/* Impl */

	class LoadsAndStoresAddress::Impl final : public AbstractOperand::AbstractImpl
	{
	  private:
		std::optional
		<
			std::variant
			<
				disxx::disasm::operand::Extension,
				disxx::disasm::operand::Shift
			>
		> m_Modifier{};
		std::optional
		<
			std::variant
			<
				disxx::disasm::operand::Register,
				std::pair
				<
					signed short int,
					bool
				>
			>
		> m_ExtraValue{};
		Register m_BaseRegister{};

	  public:
		explicit Impl(void) noexcept;
		explicit Impl(const Register &&) noexcept;

		explicit Impl(const Impl &) noexcept;
		Impl &operator=(const Impl &) noexcept;

		explicit Impl(Impl &&) noexcept;
		Impl &operator=(Impl &&) noexcept;

		virtual ~Impl(void) noexcept override = default;

		void AddImmediatePreIndexedOffset(std::pair<signed short int, bool> &&) noexcept;
		void AddRegisterOffset(disxx::disasm::operand::Register &&) noexcept;

		void AddExtension(Extension &&) noexcept;
		void AddShift(Shift &&) noexcept;

		virtual std::string GetMnemonic(void) const noexcept(false) override;
	};

	LoadsAndStoresAddress::Impl::Impl(void) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_LOADSANDSTORESADDRESS}
		, m_Modifier{std::nullopt}
		, m_ExtraValue{std::nullopt}
		, m_BaseRegister{}
	{}

	LoadsAndStoresAddress::Impl::Impl(const Register &&base) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_LOADSANDSTORESADDRESS}
		, m_Modifier{std::nullopt}
		, m_ExtraValue{std::nullopt}
		, m_BaseRegister{base}
	{}

	LoadsAndStoresAddress::Impl::Impl(const Impl &other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_LOADSANDSTORESADDRESS}
        , m_Modifier{other.m_Modifier}
		, m_ExtraValue{other.m_ExtraValue}
		, m_BaseRegister{other.m_BaseRegister}
	{}

	LoadsAndStoresAddress::Impl &LoadsAndStoresAddress::Impl::operator=(const Impl &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Modifier = other.m_Modifier;
			this->m_ExtraValue = other.m_ExtraValue;
			this->m_BaseRegister = other.m_BaseRegister;
		}

		return *this;
	}

	LoadsAndStoresAddress::Impl::Impl(Impl &&other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_LOADSANDSTORESADDRESS}
		, m_Modifier{std::move(other.m_Modifier)}
        , m_ExtraValue{std::move(other.m_ExtraValue)}
		, m_BaseRegister{std::move(other.m_BaseRegister)}
	{}

	LoadsAndStoresAddress::Impl &LoadsAndStoresAddress::Impl::operator=(Impl &&other) noexcept
	{
		this->m_Modifier = std::move(other.m_Modifier);
		this->m_ExtraValue = std::move(other.m_ExtraValue);
		this->m_BaseRegister = std::move(other.m_BaseRegister);

		return *this;
	}

	void LoadsAndStoresAddress::Impl::AddImmediatePreIndexedOffset(std::pair<signed short int, bool> &&pair) noexcept
	{
		assert(!this->m_ExtraValue && "Adding offset twice");
		// If added is true mnemonic will end with '!'
		this->m_ExtraValue.emplace
		(
			std::in_place_type<std::pair<signed short int, bool>>,
			std::move(pair)
		);
	}

	void LoadsAndStoresAddress::Impl::AddRegisterOffset(Register &&reg) noexcept
	{
		assert(!this->m_ExtraValue && "Adding offset twice");
		this->m_ExtraValue.emplace
		(
			std::in_place_type<Register>,
			std::move(reg)
		);
	}

	void LoadsAndStoresAddress::Impl::AddExtension(Extension &&extension) noexcept
	{
		assert(!this->m_Modifier && "Adding modifier twice");
		this->m_Modifier.emplace
		(
			std::in_place_type<Extension>,
			std::move(extension)
		);
	}

	void LoadsAndStoresAddress::Impl::AddShift(Shift &&shift) noexcept
	{
		assert(!this->m_Modifier && "Adding modifier twice");
		this->m_Modifier.emplace
		(
			std::in_place_type<Shift>,
			std::move(shift)
		);
	}

	std::string LoadsAndStoresAddress::Impl::GetMnemonic(void) const noexcept(false)
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

	/* LoadsAndStoresAddress */

	LoadsAndStoresAddress::LoadsAndStoresAddress(operand::Register::Type type, unsigned short int val, unsigned short int bits, bool excludeZero) noexcept
		: AbstractOperand{std::make_unique<Impl>(operand::Register{type, val, bits, excludeZero})}
	{}

	LoadsAndStoresAddress::LoadsAndStoresAddress(const LoadsAndStoresAddress &other) noexcept(false)
		: AbstractOperand{std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)))}
	{}

	LoadsAndStoresAddress &LoadsAndStoresAddress::operator=(const LoadsAndStoresAddress &other) noexcept(false)
	{
		if (this != &other) [[likely]]
		{
			if (this->m_pImpl)
				this->m_pImpl.Get().reset();
			this->m_pImpl.Get() = std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)));
		}

		return *this;
	}

	LoadsAndStoresAddress::LoadsAndStoresAddress(LoadsAndStoresAddress &&other) noexcept
		: AbstractOperand{std::move(other.m_pImpl.Get())}
	{}
	
	LoadsAndStoresAddress &LoadsAndStoresAddress::operator=(LoadsAndStoresAddress &&other) noexcept
	{
		if (this->m_pImpl)
			this->m_pImpl.Get().reset();
		this->m_pImpl.Get() = std::move(other.m_pImpl.Get());

		return *this;
	}

	std::unique_ptr<AbstractOperand> LoadsAndStoresAddress::Clone(void) const noexcept
	{ return std::make_unique<LoadsAndStoresAddress>(*this); }

	void LoadsAndStoresAddress::AddImmediatePreIndexedOffset(const signed short int val, const bool isAdded) noexcept(false)
	{
		if (val)
			static_cast<Impl *>(&(*this->m_pImpl))
				->AddImmediatePreIndexedOffset(std::make_pair(val, isAdded));
	}

	void LoadsAndStoresAddress::AddRegisterOffset(disxx::disasm::operand::Register &&reg) noexcept(false)
	{
		static_cast<Impl *>(&(*this->m_pImpl))
			->AddRegisterOffset(std::move(reg)); 
	}

	void LoadsAndStoresAddress::AddExtension(unsigned short int type, unsigned short int val) noexcept(false)
	{
		static_cast<Impl *>(&(*this->m_pImpl))
			->AddExtension(Extension{type, val});
	}

	void LoadsAndStoresAddress::AddShift(Shift::Type type, unsigned short int amount) noexcept(false)
	{ 
		static_cast<Impl *>(&(*this->m_pImpl))
			->AddShift(Shift{type, amount});
	}
} /* operand */
