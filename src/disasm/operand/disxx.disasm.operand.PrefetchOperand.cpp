module;

#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <format>
#include <array>

module disxx.disasm.operand.PrefetchOperand;

namespace disxx::disasm::operand
{
	/* Impl */

	class PrefetchOperand::Impl final : public AbstractImpl
	{
	  private:
		enum class Type : unsigned short int
		{
			TYPE_PLDL1KEEP,
			TYPE_PLDL1STRM,
            TYPE_PLDL2KEEP,
			TYPE_PLDL2STRM,
            TYPE_PLDL3KEEP,
			TYPE_PLDL3STRM,
            TYPE_PLDSLCKEEP,
			TYPE_PLDSLCSTRM,
            TYPE_PLIL1KEEP,
			TYPE_PLIL1STRM,
            TYPE_PLIL2KEEP,
			TYPE_PLIL2STRM,
			TYPE_PLIL3KEEP,
			TYPE_PLIL3STRM,
            TYPE_PLISLCKEEP,
			TYPE_PLISLCSTRM,
            TYPE_PSTL1KEEP,
			TYPE_PSTL1STRM,
            TYPE_PSTL2KEEP,
			TYPE_PSTL2STRM,
            TYPE_PSTL3KEEP,
			TYPE_PSTL3STRM,
            TYPE_PSTSLCKEEP,
			TYPE_PSTSLCSTRM,
			TYPE_IR,
			// Part for INSN_RPRFM
			TYPE_PLDKEEP = 0b010011000,
			TYPE_PSTKEEP = 0b010011001,
			TYPE_PLDSTRM = 0b010011100,
			TYPE_PSTSTRM = 0b010011101
		};
	  
	  private:
		static const std::unordered_map<Type, const char *> s_PrfopTable;

	  private:
		Type m_Prfop{};

	  public:
		explicit Impl(void) noexcept;
		explicit Impl(unsigned short int) noexcept;

		explicit Impl(const Impl &) noexcept;
		Impl &operator=(const Impl &) noexcept;

		explicit Impl(const Impl &&) noexcept;
		Impl &operator=(const Impl &&) noexcept;

		virtual ~Impl(void) noexcept override = default;
		
		virtual std::string GetMnemonic(void) const noexcept(false) override;
	};

	PrefetchOperand::Impl::Impl(void) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_PREFETCHOPERAND}
		, m_Prfop{}
	{}

	PrefetchOperand::Impl::Impl(unsigned short int type) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_PREFETCHOPERAND}
		, m_Prfop{static_cast<Type>(type)}
	{}

	PrefetchOperand::Impl::Impl(const Impl &other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_PREFETCHOPERAND}
		, m_Prfop{other.m_Prfop}
	{}

	PrefetchOperand::Impl &PrefetchOperand::Impl::operator=(const Impl &other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Prfop = other.m_Prfop;
		return *this;
	}

	PrefetchOperand::Impl::Impl(const Impl &&other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_PREFETCHOPERAND}
		, m_Prfop{other.m_Prfop}
	{}

	PrefetchOperand::Impl &PrefetchOperand::Impl::operator=(const Impl &&other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Prfop = other.m_Prfop;
		return *this;
	}

	std::string PrefetchOperand::Impl::GetMnemonic(void) const noexcept(false)
	{
		if (s_PrfopTable.contains(this->m_Prfop))
			return s_PrfopTable.at(this->m_Prfop);
		return std::format("#0x{:x}", static_cast<unsigned short int>(this->m_Prfop));
	}

	const std::unordered_map<PrefetchOperand::Impl::Type, const char *> PrefetchOperand::Impl::s_PrfopTable = {
		{PrefetchOperand::Impl::Type::TYPE_PLDL1KEEP, "pldl1keep"},
		{PrefetchOperand::Impl::Type::TYPE_PLDL1STRM, "pldl1strm"},
        {PrefetchOperand::Impl::Type::TYPE_PLDL2KEEP, "pldl2keep"},
		{PrefetchOperand::Impl::Type::TYPE_PLDL2STRM, "pldl2strm"},
        {PrefetchOperand::Impl::Type::TYPE_PLDL3KEEP, "pldl3keep"},
		{PrefetchOperand::Impl::Type::TYPE_PLDL3STRM, "pldl3strm"},
        {PrefetchOperand::Impl::Type::TYPE_PLDSLCKEEP, "pldslckeep"},
		{PrefetchOperand::Impl::Type::TYPE_PLDSLCSTRM, "pldslcstrm"},
        {PrefetchOperand::Impl::Type::TYPE_PLIL1KEEP, "plil1keep"},
		{PrefetchOperand::Impl::Type::TYPE_PLIL1STRM, "plil1strm"},
        {PrefetchOperand::Impl::Type::TYPE_PLIL2KEEP, "plil2keep"},
		{PrefetchOperand::Impl::Type::TYPE_PLIL2STRM, "plil2strm"},
		{PrefetchOperand::Impl::Type::TYPE_PLIL3KEEP, "plil3keep"},
		{PrefetchOperand::Impl::Type::TYPE_PLIL3STRM, "plil3strm"},
        {PrefetchOperand::Impl::Type::TYPE_PLISLCKEEP, "plislckeep"},
		{PrefetchOperand::Impl::Type::TYPE_PLISLCSTRM, "plislcstrm"},
        {PrefetchOperand::Impl::Type::TYPE_PSTL1KEEP, "pstl1keep"},
		{PrefetchOperand::Impl::Type::TYPE_PSTL1STRM, "pstl1strm"},
        {PrefetchOperand::Impl::Type::TYPE_PSTL2KEEP, "pstl2keep"},
		{PrefetchOperand::Impl::Type::TYPE_PSTL2STRM, "pstl2strm"},
        {PrefetchOperand::Impl::Type::TYPE_PSTL3KEEP, "pstl3keep"},
		{PrefetchOperand::Impl::Type::TYPE_PSTL3STRM, "pstl3strm"},
        {PrefetchOperand::Impl::Type::TYPE_PSTSLCKEEP, "pstslckeep"},
		{PrefetchOperand::Impl::Type::TYPE_PSTSLCSTRM, "pstslcstrm"}
	};

	/* PrefetchOperand */

	PrefetchOperand::PrefetchOperand(void) noexcept
		: AbstractOperand{}
	{}

	PrefetchOperand::PrefetchOperand(unsigned short int bits) noexcept
		: AbstractOperand{std::make_unique<Impl>(bits)}
	{}

	PrefetchOperand::PrefetchOperand(const PrefetchOperand &other) noexcept(false)
		: AbstractOperand{std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)))}
	{}

	PrefetchOperand &PrefetchOperand::operator=(const PrefetchOperand &other) noexcept(false)
	{
		if (this != &other) [[likely]]
		{
			if (this->m_pImpl) [[likely]]
				this->m_pImpl.Get().reset();
			this->m_pImpl.Get() = std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)));
		}

		return *this;
	}

	PrefetchOperand::PrefetchOperand(PrefetchOperand &&other) noexcept
		: AbstractOperand{std::move(other.m_pImpl.Get())}
	{}

	PrefetchOperand &PrefetchOperand::operator=(PrefetchOperand &&other) noexcept
	{
		if (this->m_pImpl) [[likely]]
			this->m_pImpl.Get().reset();
		this->m_pImpl.Get() = std::move(other.m_pImpl.Get());

		return *this;
	}

	std::unique_ptr<AbstractOperand> PrefetchOperand::Clone(void) const noexcept
	{ return std::make_unique<PrefetchOperand>(*this); }
} /* operand */
