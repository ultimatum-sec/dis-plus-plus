module;

#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <string>

module disxx.disasm.operand.Condition;

namespace disxx::disasm::operand
{
	/* Impl */

	class Condition::Impl final : public AbstractOperand::AbstractImpl
    {
      private:
		enum class Type : unsigned short int
		{
			TYPE_EQ, TYPE_NE, TYPE_CS, TYPE_CC,
			TYPE_MI, TYPE_PL, TYPE_VS, TYPE_VC,
			TYPE_HI, TYPE_LS, TYPE_GE, TYPE_LT,
			TYPE_GT, TYPE_LE, TYPE_AL, TYPE_NV
		};
	
	  private:
        static const std::unordered_map<Type, const char *> m_sCondTable;

	  private:
		Type m_Value;

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

	Condition::Impl::Impl(void) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_CONDITION}
		, m_Value{}
	{}

	Condition::Impl::Impl(unsigned short int val) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_CONDITION}
		, m_Value{static_cast<Type>(val)}
	{}

	Condition::Impl::Impl(const Impl &other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_CONDITION}
		, m_Value{other.m_Value}
	{}

	Condition::Impl &Condition::Impl::operator=(const Impl &other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Value = other.m_Value;
		return *this;
	}

	Condition::Impl::Impl(const Impl &&other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_CONDITION}
		, m_Value{other.m_Value}
	{}

	Condition::Impl &Condition::Impl::operator=(const Impl &&other) noexcept
	{
		this->m_Value = other.m_Value;
		return *this;
	}

	std::string Condition::Impl::GetMnemonic(void) const noexcept(false)
	{
		// Can throw exceptions
		return m_sCondTable.at(this->m_Value);
	}

	const std::unordered_map<Condition::Impl::Type, const char *> Condition::Impl::m_sCondTable = {
		{Type::TYPE_EQ, "eq"}, 
		{Type::TYPE_NE, "ne"},
        {Type::TYPE_CS, "cs"},
        {Type::TYPE_CC, "cc"},
        {Type::TYPE_MI, "mi"},
        {Type::TYPE_PL, "pl"},
        {Type::TYPE_VS, "vs"},
        {Type::TYPE_VC, "vc"},
        {Type::TYPE_HI, "hi"},
        {Type::TYPE_LS, "ls"},
        {Type::TYPE_GE, "ge"},
        {Type::TYPE_LT, "lt"},
        {Type::TYPE_GT, "gt"},
        {Type::TYPE_LE, "le"},
        {Type::TYPE_AL, "al"},
        {Type::TYPE_NV, "nv"}
	};

	/* Condition */

	Condition::Condition(void) noexcept
	    : AbstractOperand{}
    {}

	Condition::Condition(unsigned short int val) noexcept
        : AbstractOperand{std::make_unique<Impl>(val)}
    {}

    Condition::Condition(const Condition &other) noexcept(false)
        : AbstractOperand{std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)))}
    {}

    Condition &Condition::operator=(const Condition &other) noexcept(false)
    {
        if (this != &other) [[likely]]
		{
			if (this->m_pImpl)
				this->m_pImpl.Get().reset();
			this->m_pImpl.Get() = std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)));
        }

		return *this;
    }

	Condition::Condition(Condition &&other) noexcept
        : AbstractOperand{std::move(other.m_pImpl.Get())}
    {}

    Condition &Condition::operator=(Condition &&other) noexcept
    {
		if (this->m_pImpl)
			this->m_pImpl.Get().reset();
		this->m_pImpl.Get() = std::move(other.m_pImpl.Get());

		return *this;
    }

	std::unique_ptr<AbstractOperand> Condition::Clone(void) const noexcept
	{ return std::make_unique<Condition>(*this); }
} /* operand */



