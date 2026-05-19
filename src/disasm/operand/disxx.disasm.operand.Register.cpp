module;

#include <unordered_map>
#include <stdexcept>
#include <optional>
#include <utility>
#include <cassert>
#include <memory>
#include <string>

module disxx.disasm.operand.Register;

import :RegsTable;

namespace disxx::disasm::operand
{
	/* Impl */

	class Register::Impl final : public AbstractOperand::AbstractImpl
    {
	  private:
		std::optional<std::string> m_VectorArrangementSpecifier;
		Type m_RegType;
		unsigned short int m_Bits;
		unsigned short int m_Size;
		bool m_ExcludeZero;

      public:
		explicit Impl(void) noexcept;
        explicit Impl(Type, unsigned short int, unsigned short int, bool = false) noexcept;
        
		explicit Impl(const Impl &) noexcept;
        Impl &operator=(const Impl &) noexcept;

		explicit Impl(const Impl &&) noexcept;
        Impl &operator=(const Impl &&) noexcept;

        virtual ~Impl(void) noexcept override = default;
	
		virtual std::string GetMnemonic(void) const noexcept(false) override;
		
		void SetArrangementSpecifier(const std::string) noexcept;

	  public:
		static inline const char *GetArrangementSpecifier(unsigned short, unsigned short);
    };

	Register::Impl::Impl(void) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_REGISTER}
		, m_VectorArrangementSpecifier{std::nullopt}
		, m_RegType{Type::TYPE_GPR}
		, m_Bits{0b00000}
		, m_Size{0}
		, m_ExcludeZero{false}
	{}

	Register::Impl::Impl(Type type, unsigned short int bits, unsigned short int size, bool excludeZero) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_REGISTER}
		, m_VectorArrangementSpecifier{std::nullopt}
		, m_RegType{type}
		, m_Bits{bits}
		, m_Size{size}
		, m_ExcludeZero{excludeZero}
	{}

	Register::Impl::Impl(const Impl &other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_REGISTER}
		, m_VectorArrangementSpecifier{other.m_VectorArrangementSpecifier}
		, m_RegType{other.m_RegType}
		, m_Bits{other.m_Bits}
		, m_Size{other.m_Size}
		, m_ExcludeZero{other.m_ExcludeZero}
	{}

	Register::Impl &Register::Impl::operator=(const Impl &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_VectorArrangementSpecifier = other.m_VectorArrangementSpecifier;
			this->m_RegType = other.m_RegType;
			this->m_Bits = other.m_Bits;
			this->m_Size = other.m_Size;
			this->m_ExcludeZero = other.m_ExcludeZero;
		}		

		return *this;
	}

	Register::Impl::Impl(const Impl &&other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_REGISTER}
		, m_VectorArrangementSpecifier{other.m_VectorArrangementSpecifier}
		, m_RegType{other.m_RegType}
		, m_Bits{other.m_Bits}
		, m_Size{other.m_Size}
		, m_ExcludeZero{other.m_ExcludeZero}
	{}

	Register::Impl &Register::Impl::operator=(const Impl &&other) noexcept
	{
		this->m_VectorArrangementSpecifier = other.m_VectorArrangementSpecifier;
		this->m_RegType = other.m_RegType;
		this->m_Bits = other.m_Bits;
		this->m_Size = other.m_Size;
		this->m_ExcludeZero = other.m_ExcludeZero;

		return *this;
	}

	std::string Register::Impl::GetMnemonic(void) const noexcept(false)
	{
		switch (this->m_RegType)
		{
		  case Type::TYPE_GPR:
            switch (this->m_Size)
            {
              case 32:
                if (!this->m_ExcludeZero && this->m_Bits == 0b11111)
                    return "wzr";
                return RegsTable::m_sGPR32Table.at(static_cast<GPR32>(this->m_Bits));

              case 64:
                if (!this->m_ExcludeZero && this->m_Bits == 0b11111)
                    return "xzr";
                return RegsTable::m_sGPR64Table.at(static_cast<GPR64>(this->m_Bits));

              default:
                throw std::invalid_argument{"RegisterEncodingError"};
            }

		  case Type::TYPE_NEON:
            switch (this->m_Size)
            {
              case 8:
                return RegsTable::m_sNEON8Table.at(static_cast<NEON8>(this->m_Bits));

              case 16:
                return RegsTable::m_sNEON16Table.at(static_cast<NEON16>(this->m_Bits));

              case 32:
                return RegsTable::m_sNEON32Table.at(static_cast<NEON32>(this->m_Bits));

              case 64:
                return RegsTable::m_sNEON64Table.at(static_cast<NEON64>(this->m_Bits));

              case 128:
                return RegsTable::m_sNEON128Table.at(static_cast<NEON128>(this->m_Bits));

              case 128 + 'V':
                return std::string{RegsTable::m_sNEON128VTable.at(static_cast<NEON128V>(this->m_Bits))}
					+ this->m_VectorArrangementSpecifier.value_or("");

              default:
                throw std::invalid_argument{"RegisterEncodingError"};
            }
		 
		  default:
			return RegsTable::m_sSYSREGTable.at(static_cast<SYSREG>(this->m_Bits));
		}
	}

	void Register::Impl::SetArrangementSpecifier(const std::string spec) noexcept
	{
		assert(!this->m_VectorArrangementSpecifier && "Setting vector arrangement specifier twice");
		assert(this->m_RegType == Type::TYPE_NEON && "Register should has vector type");

		this->m_VectorArrangementSpecifier = spec.starts_with("[")
			? std::string{""}
			: std::string{"."}
			+ spec;
	}

	inline const char *Register::Impl::GetArrangementSpecifier(unsigned short size, unsigned short Q) noexcept(false)
    {
        if (size == 0b00 && Q == 0b0)
            return "8b";
        else if (size == 0b00 && Q == 0b1)
            return "16b";
        else if (size == 0b01 && Q == 0b0)
            return "4h";
        else if (size == 0b01 && Q == 0b1)
            return "8h";
        else if (size == 0b10 && Q == 0b0)
            return "2s";
        else if (size == 0b10 && Q == 0b1)
            return "4s";
        else if (size == 0b11 && Q == 0b0)
            throw std::invalid_argument{"ReservedArrangementSpecifierError"};
        return "2d";
	}
	
	/* Register */

	Register::Register(void) noexcept
		: AbstractOperand{std::make_unique<Impl>()}
	{}

	Register::Register(Type type, unsigned short int bits, unsigned short int size, bool excludeZero) noexcept
        : AbstractOperand{std::make_unique<Impl>(type, bits, size, excludeZero)}
    {}

    Register::Register(const Register &other) noexcept(false)
        : AbstractOperand{std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)))}
    {}

    Register &Register::operator=(const Register &other) noexcept(false)
    {
        if (this != &other) [[likely]]
		{
			if (this->m_pImpl)
				this->m_pImpl.Get().reset();
			this->m_pImpl.Get() = std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)));
        }

		return *this;
    }

	Register::Register(Register &&other) noexcept
    	: AbstractOperand{std::move(other.m_pImpl.Get())}
	{}

    Register &Register::operator=(Register &&other) noexcept
    {
		if (this->m_pImpl)
			this->m_pImpl.Get().reset();
		this->m_pImpl.Get() = std::move(other.m_pImpl.Get());

        return *this;
    }

	std::unique_ptr<AbstractOperand> Register::Clone(void) const noexcept
	{ return std::make_unique<Register>(*this); }

	void Register::SetArrangementSpecifier(const std::string spec) noexcept(false)
	{
		static_cast<Impl *>(&(*this->m_pImpl))
			->SetArrangementSpecifier(spec);
	}

	std::string_view Register::GetArrangementSpecifier(unsigned short int size, unsigned short int Q) noexcept(false)
	{ return Impl::GetArrangementSpecifier(size, Q); }
} /* operand */
