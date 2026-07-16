module;

#include <disconf.hpp>

#include <optional>
#include <variant>
#include <cassert>

export module disxx.disasm.operand.LoadsAndStoresAddress;

import disxx.disasm.operand.IOperand;

export import disxx.disasm.operand.Immediate;
export import disxx.disasm.operand.Extension;
export import disxx.disasm.operand.Register;
export import disxx.disasm.operand.Shift;

export namespace disxx::disasm::operand
{
	class __DISXX_EXPORT__ LoadsAndStoresAddress final : public IOperand
	{
	  public:
		enum class PreIndexedOffsetKind : bool
		{
			IDX_REGULAR,
			IDX_ACCUMULATIVE
		};

	  private:
		std::optional
		<
			std::variant
			<
				Extension,
				Shift
			>
		> m_Modifier{};
		std::optional
		<
			std::variant
			<
				Register,
				std::pair
				<
					Immediate<signed short int, 9>,
					PreIndexedOffsetKind
				>
			>
		> m_PreIndexedOffset{};
		Register m_BaseRegister{};

	  public:
		explicit LoadsAndStoresAddress(void) noexcept;
		explicit LoadsAndStoresAddress(Register &&) noexcept;
		
		LoadsAndStoresAddress(const LoadsAndStoresAddress &) noexcept;
		LoadsAndStoresAddress &operator=(const LoadsAndStoresAddress &) noexcept;

		LoadsAndStoresAddress(LoadsAndStoresAddress &&) noexcept;
		LoadsAndStoresAddress &operator=(LoadsAndStoresAddress &&) noexcept;

        virtual std::unique_ptr<IOperand> Clone(void) const noexcept override;

		inline void AddImmediatePreIndexedOffset(const Immediate<signed short int, 9>, const PreIndexedOffsetKind) noexcept;
		inline void AddRegisterOffset(Register &&) noexcept;

		inline void AddExtension(Extension &&) noexcept;
		inline void AddShift(Shift &&) noexcept;

		inline std::optional
		<
			std::variant
			<
				Register,
				std::pair
				<
					Immediate<signed short int, 9>,
					PreIndexedOffsetKind
				>
			>
		> GetPreIndexedOffset(void) const noexcept;
		inline std::optional
		<
			std::variant
			<
				Extension,
				Shift
			>
		> GetModifier(void) const noexcept;
	};

	inline void LoadsAndStoresAddress::AddImmediatePreIndexedOffset(const Immediate<signed short int, 9> value, const PreIndexedOffsetKind kind) noexcept
	{
		assert(!this->m_ExtraValue && "Adding offset twice");
		this->m_PreIndexedOffset.emplace
		(
			std::in_place_type
			<
				std::pair
				<
					Immediate<signed short int, 9>,
					PreIndexedOffsetKind
				>
			>,
			std::make_pair(value, kind)
		);
	}

	inline void LoadsAndStoresAddress::AddRegisterOffset(Register &&reg) noexcept
	{
		assert(!this->m_ExtraValue && "Adding offset twice");
		this->m_PreIndexedOffset.emplace
		(
			std::in_place_type<Register>,
			std::forward<Register &&>(reg)
		);
	}

	inline void LoadsAndStoresAddress::AddExtension(Extension &&extension) noexcept
	{
		assert(!this->m_Modifier && "Adding modifier twice");
		this->m_Modifier.emplace
		(
			std::in_place_type<Extension>,
			std::forward<Extension &&>(extension)
		);
	}

	inline void LoadsAndStoresAddress::AddShift(Shift &&shift) noexcept
	{
		assert(!this->m_Modifier && "Adding modifier twice");
		this->m_Modifier.emplace
		(
			std::in_place_type<Shift>,
			std::forward<Shift &&>(shift)
		);
	}

	inline std::optional
	<
		std::variant
		<
			Register,
			std::pair
			<
				Immediate<signed short int, 9>,
				LoadsAndStoresAddress::PreIndexedOffsetKind
			>
		>
	> LoadsAndStoresAddress::GetPreIndexedOffset(void) const noexcept
	{ return this->m_PreIndexedOffset; }

	inline std::optional
	<
		std::variant
		<
			Extension,
			Shift
		>
	> LoadsAndStoresAddress::GetModifier(void) const noexcept
	{ return this->m_Modifier; }
} /* disxx::disasm::operand */
