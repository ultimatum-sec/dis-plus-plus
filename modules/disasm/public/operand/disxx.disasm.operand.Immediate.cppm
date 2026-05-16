module;

#include <disconf.hpp>

#include <stdexcept>
#include <ranges>
#include <memory>
#include <string>

#if defined(__clang__)
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wshift-count-overflow"
#	pragma clang diagnostic ignored "-Winteger-overflow"
#elif defined(__GNUC__)
#	pragma GCC diagnostic push
#	pragma GCC diagnostic ignored "-Wshift-count-overflow"
#	pragma GCC diagnostic ignored "-Winteger-overflow"
#elif defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4293)
#endif

export module disxx.disasm.operand.Immediate;

import disxx.disasm.operand.AbstractOperand;

export import <type_traits>;
export import <numeric>;
export import <format>;
export import <cmath>;

template <typename T>
concept Imm = std::is_arithmetic<T>::value;

template <typename T, unsigned short int _Size>
concept ImmSize = sizeof(T) * 8 >= _Size;

template <typename T, typename U>
concept OverflowProof = sizeof(T) >= sizeof(U);

namespace disxx::disasm::operand
{
	/* Immediate */

    export template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	class __DISXX_PRIVATE__ Immediate final : public AbstractOperand
    {
	  private:
		class __DISXX_PRIVATE__ [[nodiscard]] Impl;

	  public:
		enum class Option
		{
			OPT_NONE,
			OPT_SIGNEXTEND,
			OPT_ZEROEXTEND,
			OPT_VFPEXPANDIMM
		};

      public:
		explicit Immediate(void) noexcept;
        explicit Immediate(T val, Option opt = Option::OPT_NONE) noexcept;
      
		virtual ~Immediate(void) noexcept override = default;
 
		explicit Immediate(const Immediate &) noexcept;
		Immediate& operator=(const Immediate &) noexcept(false);
        
		explicit Immediate(Immediate &&) noexcept;
		Immediate& operator=(Immediate &&) noexcept;

		inline T GetValue(void) const noexcept(false);

       	template <Imm U> requires OverflowProof<T, U>
		Immediate<T, _Size> operator+(const U &) const noexcept(false);
        template <Imm U> requires OverflowProof<T, U>
		Immediate<T, _Size> &operator+=(const U &) noexcept(false);
		template <Imm U> requires OverflowProof<T, U>
		Immediate<T, _Size> operator<<(const U &) const noexcept(false);
    	template <Imm U> requires OverflowProof<T, U>
		Immediate<T, _Size> &operator<<=(const U &) noexcept(false);
  
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;
	};

	/* Impl */

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	class Immediate<T, _Size>::Impl final : public AbstractImpl
	{
	  private:
		Option m_Opt;
		T m_Value;
		
	  public:
		explicit Impl(void) noexcept;
		explicit Impl(T, Option) noexcept;
	
		virtual ~Impl(void) noexcept override = default;
	
		explicit Impl(const Impl &) noexcept;
		Impl &operator=(const Impl &) noexcept;

		explicit Impl(const Impl &&) noexcept;
        Impl &operator=(const Impl &&) noexcept;

		inline void SetValue(const T &&) noexcept;
		inline T GetValue(void) const noexcept;
		
		inline std::string FormatValue(void) const noexcept;
		
		virtual std::string GetMnemonic(void) const noexcept(false) override;
	};

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
    Immediate<T, _Size>::Impl::Impl(void) noexcept
        : AbstractImpl{AbstractOperand::Type::TYPE_IMMEDIATE}
		, m_Opt{}
        , m_Value{static_cast<T>(0)}
	{}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	Immediate<T, _Size>::Impl::Impl(T value, Option opt) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_IMMEDIATE}
		, m_Opt{opt}
		, m_Value{value}
	{
		switch (opt)
		{
		  case Option::OPT_SIGNEXTEND:
			if constexpr (std::is_integral<T>::value)
				this->m_Value |= this->m_Value & (1ULL << (_Size - 1))
           			? ~((1 << _Size) - 1)
           			: this->m_Value;
			return;

		  case Option::OPT_ZEROEXTEND:
			if constexpr (std::is_integral<T>::value)
				this->m_Value = this->m_Value >> (_Size - sizeof(T));
			return;

		  default:
			return;
		}
	}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	Immediate<T, _Size>::Impl::Impl(const Impl &other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_IMMEDIATE}
		, m_Opt{other.m_Opt}
		, m_Value{other.m_Value}
	{}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	Immediate<T, _Size>::Impl &Immediate<T, _Size>::Impl::operator=(const Impl &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Opt = other.m_Opt;
			this->m_Value = other.m_Value;
		}

		return *this;
	}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	Immediate<T, _Size>::Impl::Impl(const Impl &&other) noexcept
		: AbstractOperand{other}
		, m_Opt{other.m_Opt}
		, m_Value{other.m_Value}
	{}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	Immediate<T, _Size>::Impl &Immediate<T, _Size>::Impl::operator=(const Impl &&other) noexcept
	{
		this->m_Opt = other.m_Opt;
		this->m_Value = other.m_Value;

		return *this;
	}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	inline std::string Immediate<T, _Size>::Impl::FormatValue(void) const noexcept
	{
		if constexpr (std::is_floating_point<T>::value)
			return std::vformat("{:f}", std::make_format_args(this->m_Value));

		if constexpr (!std::is_floating_point<T>::value)
			if (this->m_Opt != Option::OPT_VFPEXPANDIMM)
				return std::vformat("#{:#x}", std::make_format_args(this->m_Value));
	
		if constexpr (_Size <= 32)
		{
			union
			{
				T bytes;
				float fp;
			} u{0}; u.bytes = this->m_Value;
			
			return std::vformat("#{:f}", std::make_format_args(u.fp));
		}
		
		union
		{
        	T bytes;
            double fp;
		} u{0}; u.bytes = this->m_Value;
		
		return std::vformat("#{:f}", std::make_format_args(u.fp));
	}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	inline void Immediate<T, _Size>::Impl::SetValue(const T &&value) noexcept
	{ this->m_Value = value; }

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	inline T Immediate<T, _Size>::Impl::GetValue(void) const noexcept
	{ return this->m_Value; }

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	std::string Immediate<T, _Size>::Impl::GetMnemonic(void) const noexcept(false)
	{ return this->FormatValue(); }

	/* Immediate */

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	Immediate<T, _Size>::Immediate(void) noexcept
		: AbstractOperand{}
	{}
	
	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	Immediate<T, _Size>::Immediate(T val, Option opt) noexcept
		: AbstractOperand{std::make_unique<Impl>(val, opt)}
	{}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	Immediate<T, _Size>::Immediate(const Immediate<T, _Size> &other) noexcept
		: AbstractOperand{std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)))}
	{}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	Immediate<T, _Size> &Immediate<T, _Size>::operator=(const Immediate<T, _Size> &other) noexcept(false)
	{
		if (this != &other) [[likely]]
		{
			if (this->m_pImpl)
				this->m_pImpl.Get().reset();
			this->m_pImpl = std::make_unique<Impl>(*dynamic_cast<const Impl *>(*other.m_pImpl));
		}
	
		return *this;
	}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	Immediate<T, _Size>::Immediate(Immediate<T, _Size> &&other) noexcept
		: AbstractOperand{std::move(other.m_pImpl.Get())}
	{}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	Immediate<T, _Size> &Immediate<T, _Size>::operator=(Immediate<T, _Size> &&other) noexcept
	{
		if (this->m_pImpl)
           	this->m_pImpl.Get().reset();
        this->m_pImpl = std::move(other.m_pImpl);
    
        return *this;
	}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	inline T Immediate<T, _Size>::GetValue(void) const noexcept(false)
	{ return dynamic_cast<const Impl *>(&(*this->m_pImpl))->GetValue(); }

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
    template <Imm U> requires OverflowProof<T, U>
    Immediate<T, _Size> Immediate<T, _Size>::operator+(const U &val) const noexcept(false)
    {
		return Immediate<T, _Size>
		{
			std::add_sat<T>
			(
				dynamic_cast<const Impl *>(&(*this->m_pImpl))->GetValue(),
				val
			),
			Option::OPT_NONE
		};
    }

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
    template <Imm U> requires OverflowProof<T, U>
    Immediate<T, _Size> &Immediate<T, _Size>::operator+=(const U &val) noexcept(false)
    {
		static_cast<Impl *>(&(*this->m_pImpl))->SetValue
		(
			std::add_sat<T>
			(
				static_cast<Impl *>(&(*this->m_pImpl))->GetValue(),
				val
			)
		);
    
		return *this;
	}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
    template <Imm U> requires OverflowProof<T, U>
    Immediate<T, _Size> Immediate<T, _Size>::operator<<(const U &val) const noexcept(false)
    {
        return Immediate<T, _Size>
		{
			static_cast<T>
			(
				dynamic_cast<const Impl *>(&(*this->m_pImpl))->GetValue()
					<< val
			),
			Option::OPT_NONE
		};
    }

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
    template <Imm U> requires OverflowProof<T, U>
    Immediate<T, _Size> &Immediate<T, _Size>::operator<<=(const U &val) noexcept(false)
    {
		static_cast<Impl *>(&(*this->m_pImpl))->SetValue
		(
			std::mul_sat<T>
			(
				static_cast<Impl *>(&(*this->m_pImpl))->GetValue(),
				[](U num) -> U
				{
					auto result{static_cast<U>(2)};
					for (const auto _ : std::views::iota(static_cast<U>(1), num))
						result = std::mul_sat<U>(result, 2);
					return result;
				}(val)
			)
		);

		return *this;
    }

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	std::unique_ptr<AbstractOperand> Immediate<T, _Size>::Clone(void) const noexcept
	{ return std::make_unique<Immediate<T, _Size>>(*this); }
} /* operand */

#if defined(__clang__)
#	pragma clang diagnostic pop
#elif defined(__GNUC__)
#	pragma GCC diagnostic pop
#elif defind(_MSC_VER)
#	pragma warning(pop)
#endif
