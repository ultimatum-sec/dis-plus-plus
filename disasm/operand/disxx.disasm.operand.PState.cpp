module;

#include <unordered_map>
#include <stdexcept>
#include <format>

module disxx.disasm.operand.PState;

namespace disxx::disasm::operand
{
	/* PState::Impl */

	class PState::Impl final : public AbstractImpl
	{
	  private:
		enum class Type : unsigned short int
		{
			TYPE_ZA			= 0b000001,
			TYPE_SM			= 0b000010,
			TYPE_UAO		= 0b000011,
			TYPE_PAN		= 0b000100,
			TYPE_SPSEL		= 0b000101,
			TYPE_SSBS		= 0b011001,
			TYPE_DIT		= 0b011010,
			TYPE_TCO		= 0b011100,
			TYPE_DAIFSET	= 0b011110,
			TYPE_DAIFCLR	= 0b011111,
			// PState encoding with CRm in it
			TYPE_ALLINT		= 0b0000001000,
			TYPE_PM			= 0b0010001000,
			TYPE_SVCRSM		= 0b0010011011,
			TYPE_SVCRZA		= 0b0100011011,
			TYPE_SVCRSMZA	= 0b0110011011
		};

	  private:
		static const std::unordered_map<Type, const char *> s_PStateTable;

	  private:
		Type m_PState;

	  public:
		explicit Impl(void) noexcept;
		explicit Impl(unsigned short int) noexcept;
		
		explicit Impl(const Impl &) noexcept;
		Impl &operator=(const Impl &) noexcept;

		explicit Impl(const Impl &&) noexcept;
		Impl &operator=(const Impl &&) noexcept;

		virtual std::string GetMnemonic(void) const noexcept(false) override;
	};

	PState::Impl::Impl(void) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_PSTATE}
		, m_PState{}
	{}

	PState::Impl::Impl(unsigned short int bits) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_PSTATE}
		, m_PState{static_cast<Type>(bits)}
	{}

	PState::Impl::Impl(const Impl &other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_PSTATE}
		, m_PState{other.m_PState}
	{}

	PState::Impl &PState::Impl::operator=(const Impl &other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_PState = other.m_PState;

		return *this;
	}

	PState::Impl::Impl(const Impl &&other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_PSTATE}
		, m_PState{other.m_PState}
	{}

	PState::Impl &PState::Impl::operator=(const Impl &&other) noexcept
	{
		this->m_PState = other.m_PState;

		return *this;
	}

	std::string PState::Impl::GetMnemonic(void) const noexcept(false)
	{
		return s_PStateTable
			.at(this->m_PState);
	}

	const std::unordered_map<PState::Impl::Type, const char *> PState::Impl::s_PStateTable = {
		{Type::TYPE_ZA,			"za"},
		{Type::TYPE_SM,			"sm"},
		{Type::TYPE_UAO,		"uao"},
		{Type::TYPE_PAN,		"pan"},
		{Type::TYPE_SPSEL,		"spsel"},
		{Type::TYPE_SSBS,		"ssbs"},
		{Type::TYPE_DIT,		"dit"},
		{Type::TYPE_TCO,		"tco"},
		{Type::TYPE_DAIFSET,	"daifset"},
		{Type::TYPE_DAIFCLR,	"daifclr"},
		{Type::TYPE_ALLINT,		"allint"},
		{Type::TYPE_PM,			"pm"},
		{Type::TYPE_SVCRSM,		"svcrsm"},
		{Type::TYPE_SVCRZA,		"scvrza"},
		{Type::TYPE_SVCRSMZA,	"svcrsmza"}
	};

	/* Pstate */

	PState::PState(void) noexcept
		: AbstractOperand{}
	{}

	PState::PState(unsigned short int bits) noexcept
		: AbstractOperand{std::make_unique<Impl>(bits)}
	{}

	PState::PState(const PState &other) noexcept(false)
		: AbstractOperand{std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)))}
	{}

	PState &PState::operator=(const PState &other) noexcept(false)
	{
		if (this != &other) [[likely]]
		{
			if (this->m_pImpl)
				this->m_pImpl.Get().reset();
			this->m_pImpl.Get() = std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)));
		}

		return *this;
	}

	PState::PState(PState &&other) noexcept
		: AbstractOperand{std::move(other.m_pImpl.Get())}
	{}

	PState &PState::operator=(PState &&other) noexcept
	{
		if (this->m_pImpl)
			this->m_pImpl.Get().reset();
		this->m_pImpl.Get() = std::move(other.m_pImpl.Get());

		return *this;
	}

	std::unique_ptr<AbstractOperand> PState::Clone(void) const noexcept
	{ return std::make_unique<PState>(*this); }
} /* operand */



