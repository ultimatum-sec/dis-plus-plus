module;

#include <unordered_map>
#include <stdexcept>
#include <format>

module disxx.disasm.operand.MemoryBarrier;

namespace disxx::disasm::operand
{
	/* Impl */
	
	class MemoryBarrier::Impl final : public AbstractImpl
	{
	  private:
		enum class Type : unsigned short int
		{
			TYPE_OSHLD	= 0b0001,
			TYPE_OSHST	= 0b0010,
			TYPE_OSH	= 0b0011,
			TYPE_NSHLD	= 0b0101,
			TYPE_NSHST	= 0b0110,
			TYPE_NSH	= 0b0111,
			TYPE_ISHLD	= 0b1001,
			TYPE_ISHST	= 0b1010,
			TYPE_ISH	= 0b1011,
			TYPE_LD		= 0b1101,
			TYPE_ST		= 0b1110,
			TYPE_SY		= 0b1111
		};

	  private:
		static const std::unordered_map<Type, const char *> s_BarrierTable;

	  private:
		Type m_Barrier;
		bool m_NXS;

	  public:
		explicit Impl(void) noexcept;
		explicit Impl(unsigned short int, bool) noexcept;

		explicit Impl(const Impl &other) noexcept;
		Impl &operator=(const Impl &) noexcept;

		explicit Impl(const Impl &&) noexcept;
		Impl &operator=(const Impl &&) noexcept;

		virtual ~Impl(void) noexcept override = default;

		virtual std::string GetMnemonic(void) const noexcept(false) override;
	};

	MemoryBarrier::Impl::Impl(void) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_MEMORYBARRIER}
		, m_Barrier{}
		, m_NXS{}
	{}

	MemoryBarrier::Impl::Impl(unsigned short int bits, bool nXS) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_MEMORYBARRIER}
		, m_Barrier{static_cast<Type>(bits)}
		, m_NXS{nXS}
	{}

	MemoryBarrier::Impl::Impl(const Impl &other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_MEMORYBARRIER}
		, m_Barrier{other.m_Barrier}
		, m_NXS{other.m_NXS}
	{}

	MemoryBarrier::Impl &MemoryBarrier::Impl::operator=(const Impl &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Barrier = other.m_Barrier;
			this->m_NXS = other.m_NXS;
		}

		return *this;
	}

	MemoryBarrier::Impl::Impl(const Impl &&other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_MEMORYBARRIER}
		, m_Barrier{other.m_Barrier}
		, m_NXS{other.m_NXS}
	{}

	MemoryBarrier::Impl &MemoryBarrier::Impl::operator=(const Impl &&other) noexcept
	{
		this->m_Barrier = other.m_Barrier;
		this->m_NXS = other.m_NXS;

		return *this;
	}

	std::string MemoryBarrier::Impl::GetMnemonic(void) const noexcept(false)
	{
		return std::string
		{
			s_BarrierTable
				.at(this->m_Barrier)
		} + (this->m_NXS ? "nXS" : "");
	}

	const std::unordered_map<MemoryBarrier::Impl::Type, const char *> MemoryBarrier::Impl::s_BarrierTable = {
		{Type::TYPE_OSHLD,	"oshld"},
        {Type::TYPE_OSHST,	"oshst"},
        {Type::TYPE_OSH,	"osh"},
        {Type::TYPE_NSHLD,	"nshld"},
        {Type::TYPE_NSHST,	"nshst"},
        {Type::TYPE_NSH,	"nsh"},
        {Type::TYPE_ISHLD,	"ishld"},
        {Type::TYPE_ISHST,	"ishst"},
        {Type::TYPE_ISH,	"ish"},
        {Type::TYPE_LD,		"ld"},
        {Type::TYPE_ST,		"st"},
        {Type::TYPE_SY,		"sy"}
	};

	/* MemoryBarrier */

	MemoryBarrier::MemoryBarrier(void) noexcept
		: AbstractOperand{}
	{}

	MemoryBarrier::MemoryBarrier(unsigned short int bits, bool nXS) noexcept
		: AbstractOperand{std::make_unique<Impl>(bits, nXS)}
	{}

	MemoryBarrier::MemoryBarrier(const MemoryBarrier &other) noexcept(false)
		: AbstractOperand{std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)))}
	{}

	MemoryBarrier &MemoryBarrier::operator=(const MemoryBarrier &other) noexcept(false)
	{
		if (this != &other)
		{
			if (this->m_pImpl)
				this->m_pImpl.Get().reset();
			this->m_pImpl.Get() = std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)));
		}

		return *this;
	}

	MemoryBarrier::MemoryBarrier(MemoryBarrier &&other) noexcept
		: AbstractOperand{std::move(other.m_pImpl.Get())}
	{}

	MemoryBarrier &MemoryBarrier::operator=(MemoryBarrier &&other) noexcept
	{
		if (this->m_pImpl)
			this->m_pImpl.Get().reset();
		this->m_pImpl.Get() = std::move(other.m_pImpl.Get());

		return *this;
	}

	std::unique_ptr<AbstractOperand> MemoryBarrier::Clone(void) const noexcept
	{ return std::make_unique<MemoryBarrier>(*this); }
} /* operand */



