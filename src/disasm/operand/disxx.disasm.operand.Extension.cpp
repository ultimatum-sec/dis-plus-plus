module;

#include <unordered_map>
#include <stdexcept>
#include <utility>
#include <format>

module disxx.disasm.operand.Extension;

namespace disxx::disasm::operand
{
	/* Impl */

	class Extension::Impl final : public AbstractOperand::AbstractImpl
	{
	  private:
		enum class Type : unsigned short int
		{
			TYPE_UXTB,
			TYPE_UXTH,
			TYPE_UXTW,
			TYPE_UXTX,
			TYPE_SXTB,
			TYPE_SXTH,
			TYPE_SXTW,
			TYPE_SXTX
		};

	  private:
		static const std::unordered_map<Type, const char *> s_ExtensionTable;

	  private:
		Type m_ExtensionType{};
		unsigned short int m_Val{};

	  public:
		explicit Impl(void) noexcept;
		explicit Impl(unsigned short int, unsigned short int) noexcept;

		explicit Impl(const Impl &) noexcept;
		Impl &operator=(const Impl &) noexcept;

		explicit Impl(const Impl &&) noexcept;
		Impl &operator=(const Impl &&) noexcept;

		virtual std::string GetMnemonic(void) const noexcept(false) override;
	};

	Extension::Impl::Impl(void) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_EXTENSION}
		, m_ExtensionType{}
		, m_Val{}
	{}

	Extension::Impl::Impl(unsigned short int type, unsigned short int val) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_EXTENSION}
		, m_ExtensionType{static_cast<Type>(type)}
		, m_Val{val}
	{}

	Extension::Impl::Impl(const Impl &other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_EXTENSION}
		, m_ExtensionType{other.m_ExtensionType}
		, m_Val{other.m_Val}
	{}

	Extension::Impl &Extension::Impl::operator=(const Impl &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_ExtensionType = other.m_ExtensionType;
			this->m_Val = other.m_Val;
		}

		return *this;
	}

	Extension::Impl::Impl(const Impl &&other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_EXTENSION}
		, m_ExtensionType{other.m_ExtensionType}
		, m_Val{other.m_Val}
	{}

	Extension::Impl &Extension::Impl::operator=(const Impl &&other) noexcept
	{
		this->m_ExtensionType = other.m_ExtensionType;
		this->m_Val = other.m_Val;

		return *this;
	}

	std::string Extension::Impl::GetMnemonic(void) const noexcept(false)
	{
		return std::format
		(
			"{} #0x{:x}",
			s_ExtensionTable.at(this->m_ExtensionType),
			this->m_Val
		);
	}

	const std::unordered_map<Extension::Impl::Type, const char *> Extension::Impl::s_ExtensionTable = {
		{Type::TYPE_UXTB, "uxtb"},
		{Type::TYPE_UXTH, "uxth"},
		{Type::TYPE_UXTW, "uxtw"},
		{Type::TYPE_UXTX, "uxtx"},
		{Type::TYPE_SXTB, "sxtb"},
		{Type::TYPE_SXTH, "sxth"},
		{Type::TYPE_SXTW, "sxtw"},
		{Type::TYPE_SXTX, "sxtx"}
	};

	/* Extension */

	Extension::Extension(void) noexcept
		: AbstractOperand{}
	{}

	Extension::Extension(unsigned short int type, unsigned short int val) noexcept
		: AbstractOperand{std::make_unique<Impl>(type, val)}
	{}

	Extension::Extension(const Extension &other) noexcept
		: AbstractOperand{std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)))}
	{}

	Extension &Extension::operator=(const Extension &other) noexcept(false)
	{
		if (this != &other) [[likely]]
		{
			if (this->m_pImpl)
				this->m_pImpl.Get().reset();
			this->m_pImpl.Get() = std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)));
		}

		return *this;
	}

	Extension::Extension(Extension &&other) noexcept
		: AbstractOperand{std::move(other.m_pImpl.Get())}
	{}

	Extension &Extension::operator=(Extension &&other) noexcept
	{
		if (this->m_pImpl)
			this->m_pImpl.Get().reset();
		this->m_pImpl.Get() = std::move(other.m_pImpl.Get());

		return *this;
	}

	std::unique_ptr<AbstractOperand> Extension::Clone(void) const noexcept
	{ return std::make_unique<Extension>(*this); }
} /* operand */



