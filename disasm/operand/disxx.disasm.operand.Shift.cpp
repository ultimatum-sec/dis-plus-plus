module;

#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <format>
#include <string>

module disxx.disasm.operand.Shift;

namespace disxx::disasm::operand
{
	/* Impl */

	class Shift::Impl final : public AbstractOperand::AbstractImpl
    {
      private:
        static const std::unordered_map<Type, const char *> m_sShiftTable;

	  private:
		Type m_ShiftType;
		unsigned short int m_Amount;

      public:
		explicit Impl(void) noexcept;
        explicit Impl(Type, unsigned short int) noexcept;
        
		explicit Impl(const Impl &) noexcept;
        Impl &operator=(const Impl &) noexcept;

		explicit Impl(const Impl &&) noexcept;
        Impl &operator=(const Impl &&) noexcept;

        virtual ~Impl(void) noexcept override = default;

		virtual std::string GetMnemonic(void) const noexcept(false) override;
    };

	Shift::Impl::Impl(void) noexcept
        : AbstractImpl{AbstractOperand::Type::TYPE_SHIFT}
        , m_ShiftType{}
        , m_Amount{}
    {}

    Shift::Impl::Impl(Type type, unsigned short int amount) noexcept
        : AbstractImpl{AbstractOperand::Type::TYPE_SHIFT}
		, m_ShiftType{type}
		, m_Amount{amount}
    {}

    Shift::Impl::Impl(const Impl &other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_SHIFT}
		, m_ShiftType{other.m_ShiftType}
        , m_Amount{other.m_Amount}
	{}

    Shift::Impl &Shift::Impl::operator=(const Impl &other) noexcept
    {
        if (this != &other) [[unlikely]]
		{
			this->m_ShiftType = other.m_ShiftType;
			this->m_Amount = other.m_Amount;
       	} 

		return *this;
    }

	Shift::Impl::Impl(const Impl &&other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_SHIFT}
		, m_ShiftType{other.m_ShiftType}
        , m_Amount{other.m_Amount}
	{}

    Shift::Impl &Shift::Impl::operator=(const Impl &&other) noexcept
    {
		this->m_ShiftType = other.m_ShiftType;
		this->m_Amount = other.m_Amount;

		return *this;
    }

	std::string Shift::Impl::GetMnemonic(void) const noexcept(false)
	{
		return std::format
		(
			"{} #0x{:x}",
			m_sShiftTable.at(this->m_ShiftType),	
			this->m_Amount
		);
	}

    const std::unordered_map<Shift::Type, const char *> Shift::Impl::m_sShiftTable = {
        {Type::SHIFT_LSL, "lsl"},
        {Type::SHIFT_LSR, "lsr"},
        {Type::SHIFT_ASR, "asr"},
        {Type::SHIFT_ROR, "ror"},
        {Type::SHIFT_MSL, "msl"}
    };

	/* Shift */

	Shift::Shift(void) noexcept
		: AbstractOperand{}
	{}
	
	Shift::Shift(Type type, unsigned short int amount) noexcept
        : AbstractOperand{std::make_unique<Impl>(type, amount)}
    {}

    Shift::Shift(const Shift &other) noexcept(false)
        : AbstractOperand{std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)))}
    {}

    Shift &Shift::operator=(const Shift &other) noexcept(false)
    {
        if (this != &other) [[likely]]
		{
			if (this->m_pImpl)
				this->m_pImpl.Get().reset();
			this->m_pImpl.Get() = std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)));
        }

		return *this;
    }

	Shift::Shift(Shift &&other) noexcept
        : AbstractOperand{std::move(other.m_pImpl.Get())}
    {}

    Shift &Shift::operator=(Shift &&other) noexcept
    {
		if (this->m_pImpl)
			this->m_pImpl.Get().reset();
		this->m_pImpl.Get() = std::move(other.m_pImpl.Get());

		return *this;
    }

	std::unique_ptr<AbstractOperand> Shift::Clone(void) const noexcept
	{ return std::make_unique<Shift>(*this); }
} /* operand */


