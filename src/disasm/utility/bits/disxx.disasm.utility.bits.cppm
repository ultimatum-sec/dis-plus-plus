module;

#include <disconf.hpp>

#include <stdexcept>
#include <format>

#if defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wshift-count-overflow"
    #pragma GCC diagnostic ignored "-Winteger-overflow"
#endif

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable : 4293)
#endif

export module disxx.disasm.utility.bits;

export import <type_traits>;
export import <concepts>;
export import <expected>;
export import <variant>;
export import <cstdint>;
export import <tuple>;
export import <cmath>;
export import <bit>;

// Output (T) value can be less than input (U)
template <typename T, typename U, unsigned short int _Start, unsigned int _Width>
concept Extractable = (_Width - _Start + 1 <= sizeof(T) * 8)
	&& (_Start + 1 <= sizeof(U) * 8)
	&& (_Width + 1 <= sizeof(U) * 8)
	&& (_Start <= _Width);

export namespace bits
{
	template<std::integral T, std::integral U, unsigned int _Start, unsigned int _Width>
	requires Extractable<T, U, _Start, _Width>
	inline T extract(U input) noexcept
	{
		T result{0};
		#ifdef __aarch64__
			if constexpr (std::is_signed<T>::value)
			{	
				asm volatile
				(
					"sbfm	%w0, %w1, #%2, #%3"
					: "=r"	(result)
					: "r"	(input)
					, "i"	(_Start)
					, "i"	(_Width)
					:
				);
			}
			else
			{
				asm volatile
                (
                    "ubfm	%w0, %w1, #%2, #%3"
                    : "=r"  (result)
                    : "r"   (input)
                    , "i"   (_Start)
                    , "i"   (_Width)
                    :
                );
			}
		#else
			// Don't know other assembly dialects, can't fully optimize that :(
			constexpr U mask{static_cast<U>(((1 << ((_Width - _Start) + 1)) - 1) << _Start)};
			result = (input & mask) >> _Start;
		#endif
	
		return result;
	}

	template <std::integral T, unsigned short int _Size>
	requires (_Size + 1 <= sizeof(T) * 8)
	__DISXX_PRIVATE__ short int HighestSetBit(T x) noexcept
	{
		for (short int i{_Size - 1}; i >= 0; --i)
		{
			if (x >> i)
				return i;
		}

		return -1;
	}

	template <std::integral T, unsigned short int _Size>
    requires (_Size + 1 <= sizeof(T) * 8)
	__DISXX_PRIVATE__ short int HighestSetBitNZ(T x) noexcept(false)
	{
		if (!x) [[unlikely]]
			throw std::invalid_argument{std::format("ContractViolationError: {}", __func__)};

		return HighestSetBit<T, _Size>(x);
	}

	template <std::unsigned_integral T, unsigned short int _Size>
   	requires (_Size + 1 <= sizeof(T) * 8)
   	__DISXX_PRIVATE__ short int LowestSetBit(T x) noexcept
   	{
   	    for (short int i{0}; i < _Size; ++i)
   	    {
   	        if ((x >> i) & 1)
   	            return i + 1;
   	    }
	
	 	return -1;
   	}

	template <std::unsigned_integral T, unsigned short int _Size>
    requires (_Size + 1 <= sizeof(T) * 8)
    __DISXX_PRIVATE__ short int LowestSetBitNZ(T x)
    {
		if (!x) [[unlikely]]
      		throw std::invalid_argument{std::format("ContractViolationError: {}", __func__)};
 
		return LowestSetBit<T, _Size>(x);
    }

	template <std::integral T, unsigned short int _Size>
	__DISXX_PRIVATE__ unsigned short int BitCount(T x) noexcept
	{
		unsigned short int result{0};
		for (unsigned short int i{0}; i < _Size; ++i)
		{
			if ((x >> i) & 1)
				result++;
		}

		return result;
	}

	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wimplicit-int-conversion"
	template <std::unsigned_integral T, std::unsigned_integral U, unsigned short int _M>
	requires (_M <= sizeof(U) * 8)
	__DISXX_PRIVATE__ T Replicate(U x, unsigned short int N) noexcept
	{
		T result{0};
		for (unsigned short int i{0}; i < N; ++i)
			result = (result << _M) | x;
		
		return result;
	}
	#pragma clang diagnostic pop

	template <std::integral T> __DISXX_PRIVATE__ T Ones(unsigned short int N) noexcept(false)
    {
		if (N > sizeof(T) * 64) [[unlikely]]
			throw std::invalid_argument{std::format("ContractViolationError: {}", __func__)};

		T result{0};
        for (unsigned short int i{0}; i < N; ++i)
            result |= (1 << i);
        return result;
    }

	template <std::integral T, std::integral U, unsigned short int _M>
	__DISXX_PRIVATE__ T ZeroExtend(U x, unsigned short int N) noexcept(false)
	{
		if (N < _M) [[unlikely]]
			throw std::invalid_argument{std::format("ContractViolationError: {}", __func__)};

		return static_cast<T>(x & Ones<T>(N));
	}

	template <std::integral T, unsigned short int _N> requires (sizeof(T) * 8 == _N)
	__DISXX_PRIVATE__ bool IsOnes(T x) { return x == Ones<T>(_N); }

	template<std::integral T, unsigned short int _M> requires (_M <= sizeof(T) * 8)
	__DISXX_PRIVATE__ std::expected<std::tuple<T, T>, std::monostate> DecodeBitMasks
    (
        bool immN, unsigned short int imms,
		unsigned short int immr, bool immediate
    ) noexcept(false) 
	{
		auto tmask{static_cast<T>(0)}, wmask{static_cast<T>(0)};

		if ((((immN << 6) | (~imms)) & (~1)) == 0b0000000) [[unlikely]]
			return std::unexpected{std::monostate{}};

		const auto len{HighestSetBitNZ<unsigned short int, 7>((immN << 6) | (~imms))};
		if (!(2 <= std::pow(2, len) && std::pow(2, len) <= _M)) [[unlikely]]
			 return std::unexpected{std::monostate{}};
		
		const auto levels{ZeroExtend<unsigned short int, unsigned short int, 6>(Ones<unsigned short int>(len), 6)};
		if (immediate && (imms & levels) == levels) [[unlikely]]
			return std::unexpected{std::monostate{}};
		
		auto s{imms & levels}, r{immr & levels};
		auto diff{s - r}, esize{1 << len};
		auto d{diff & Ones<unsigned short int>(len - 1)};
		
		const auto &[welem, telem] = [esize, s, d](void) -> std::tuple<T, T>
		{
			return std::make_tuple
			(
				ZeroExtend<T, unsigned short int, _M>(Ones<unsigned short int>(s + 1), esize),
				ZeroExtend<T, unsigned short int, _M>(Ones<unsigned short int>(d + 1), esize)
			);
		}();

		wmask = Replicate<T, T, _M>(std::rotr(welem, r), _M / esize);
		tmask = Replicate<T, T, _M>(telem, _M / esize);

		return std::make_tuple(wmask, tmask);
	}

	/*
	 * Non-template functions
	 */

	__DISXX_PRIVATE__ std::uint64_t AdvSIMDExpandImm(unsigned short int, unsigned short int, unsigned short int) noexcept(false);

	__DISXX_PRIVATE__ unsigned short int SysOp(unsigned short int, unsigned short int, unsigned short int, unsigned short int);

	__DISXX_PRIVATE__ bool SysOp128(unsigned short int, unsigned short int, unsigned short int, unsigned short int);

	__DISXX_PRIVATE__ bool MoveWidePreferred(bool, bool, unsigned short int, unsigned short int) noexcept;

	__DISXX_PRIVATE__ bool BFXPreffered(bool, bool, unsigned short int, unsigned short int) noexcept;
} /* bits */

#if defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
    #pragma warning(pop)
#endif


