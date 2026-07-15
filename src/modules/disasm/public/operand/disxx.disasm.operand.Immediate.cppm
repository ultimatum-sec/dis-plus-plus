module;

#include <disconf.hpp>

#include <stdexcept>
#include <ranges>
#include <memory>
#include <string>

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

export namespace disxx::disasm::operand
{
    template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	class __DISXX_PRIVATE__ Immediate final : public AbstractOperand
    {
	  public:
		enum class Option
		{
			OPT_NONE, OPT_SIGNEXTEND,
			OPT_ZEROEXTEND, OPT_VFPEXPANDIMM
		};

	  private:
		Option m_Opt{};
		T m_Value{};
	
      public:
		explicit Immediate(void) noexcept;
        explicit Immediate(T, Option = Option::OPT_NONE) noexcept;
      
		virtual ~Immediate(void) noexcept override = default;
 
		Immediate(const Immediate &) noexcept;
		Immediate& operator=(const Immediate &) noexcept;
        
		Immediate(Immediate &&) noexcept;
		Immediate& operator=(Immediate &&) noexcept;

       	template <Imm U> requires OverflowProof<T, U>
		Immediate<T, _Size> operator+(const U &) const noexcept;
        template <Imm U> requires OverflowProof<T, U>
		Immediate<T, _Size> &operator+=(const U &) noexcept;
		template <Imm U> requires OverflowProof<T, U>
		Immediate<T, _Size> operator<<(const U &) const noexcept;
    	template <Imm U> requires OverflowProof<T, U>
		Immediate<T, _Size> &operator<<=(const U &) noexcept;
 
        virtual std::unique_ptr<AbstractOperand> Clone(void) const noexcept override;

		inline T GetValue(void) const noexcept;
		inline Option GetOption(void) const noexcept;
	};


	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
    constexpr Immediate<T, _Size>::Immediate(void) noexcept
        : AbstractOperand{AbstractOperand::Type::TYPE_IMMEDIATE}
		, m_Opt{}
        , m_Value{static_cast<T>(0)}
	{}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	constexpr Immediate<T, _Size>::Immediate(T value, Option opt) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_IMMEDIATE}
		, m_Opt{opt}
		, m_Value{value}
	{
		switch (opt)
		{
		  case Option::OPT_SIGNEXTEND:
			if constexpr (std::is_integral<T>::value)
				this->m_Value |= this->m_Value & (1ull << (_Size - 1))
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
	Immediate<T, _Size>::Immediate(const Immediate &other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_IMMEDIATE}
		, m_Opt{other.m_Opt}
		, m_Value{other.m_Value}
	{}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	Immediate<T, _Size> &Immediate<T, _Size>::operator=(const Immediate &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Opt = other.m_Opt;
			this->m_Value = other.m_Value;
		}

		return *this;
	}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	Immediate<T, _Size>::Immediate(Immediate &&other) noexcept
		: AbstractOperand{AbstractOperand::Type::TYPE_IMMEDIATE}
		, m_Opt{std::move(other.m_Opt)}
		, m_Value{std::move(other.m_Value)}
	{}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	Immediate<T, _Size> &Immediate<T, _Size>::operator=(Immediate &&other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_Opt = std::move(other.m_Opt);
			this->m_Value = std::move(other.m_Value);
		}

		return *this;
	}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
    template <Imm U> requires OverflowProof<T, U>
    Immediate<T, _Size> Immediate<T, _Size>::operator+(const U &val) const noexcept
    {
		return Immediate<T, _Size>
		{
			std::add_sat<T>(this->m_Value, val),
			Option::OPT_NONE
		};
    }

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
    template <Imm U> requires OverflowProof<T, U>
    Immediate<T, _Size> &Immediate<T, _Size>::operator+=(const U &val) noexcept
    {
		this->m_Value = std::add_sat<T>(this->m_Value, val);
		return *this;
	}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
    template <Imm U> requires OverflowProof<T, U>
    Immediate<T, _Size> Immediate<T, _Size>::operator<<(const U &val) const noexcept
    {
		return Immediate<T, _Size>
		{
 			std::mul_sat<T>
			(
				this->m_Value,
				[](U num) -> U
				{
					auto result{static_cast<U>(2)};
					for (const auto _ : std::views::iota(static_cast<U>(1), num))
						result = std::mul_sat<U>(result, 2);
					return result;
				}(val)
  			),
			Option::OPT_NONE
		};
	}

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
    template <Imm U> requires OverflowProof<T, U>
    Immediate<T, _Size> &Immediate<T, _Size>::operator<<=(const U &val) noexcept
    {
		this->m_Value = std::mul_sat<T>
		(
			this->m_Value,
			[](U num) -> U
			{
				auto result{static_cast<U>(2)};
				for (const auto _ : std::views::iota(static_cast<U>(1), num))
					result = std::mul_sat<U>(result, 2);
				return result;
			}(val)
		);

		return *this;
    }

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	std::unique_ptr<AbstractOperand> Immediate<T, _Size>::Clone(void) const noexcept
	{ return std::make_unique<Immediate<T, _Size>>(*this); }

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	inline T Immediate<T, _Size>::GetValue(void) const noexcept
	{ return this->m_Value; }

	template <Imm T, unsigned short int _Size> requires ImmSize<T, _Size>
	inline Immediate<T, _Size>::Option Immediate<T, _Size>::GetOption(void) const noexcept
	{ return this->m_Option; }
} /* disxx::disasm::operand */
