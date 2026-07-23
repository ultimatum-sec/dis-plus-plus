module;

#include <disconf.hpp>

export module disxx.disasm.utility.bits;

export import <type_traits>;
export import <stdexcept>;
export import <concepts>;
export import <expected>;
export import <variant>;
export import <utility>;
export import <cstdint>;
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
		auto result{static_cast<T>(0)};
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
			// Don't know other assembly dialects - can't fully optimize that :(
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
	__DISXX_PRIVATE__ std::expected<signed short int, std::monostate> HighestSetBitNZ(T x) noexcept
	{
		if (!x) [[unlikely]]
			return std::unexpected{std::monostate{}};

		return HighestSetBit<T, _Size>(x);
	}

	template <std::unsigned_integral T, unsigned short int _Size>
   	requires (_Size + 1 <= sizeof(T) * 8)
   	__DISXX_PRIVATE__ signed short int LowestSetBit(T x) noexcept
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
    __DISXX_PRIVATE__ std::expected<signed short int, std::monostate> LowestSetBitNZ(T x) noexcept
    {
		if (!x) [[unlikely]]
			return std::unexpected{std::monostate{}};
 
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

	template <std::unsigned_integral T, std::unsigned_integral U, unsigned short int _M>
	requires (_M <= sizeof(U) * 8)
	__DISXX_PRIVATE__ T Replicate(U x, unsigned short int N) noexcept
	{
		auto result{static_cast<T>(0)};
		for (unsigned short int i{0}; i < N; ++i)
			result = (result << _M) | x;
		
		return result;
	}

	template <std::integral T>
	__DISXX_PRIVATE__ std::expected<T, std::monostate> Ones(unsigned short int N) noexcept
    {
		if (N > sizeof(T) * 64) [[unlikely]]
			return std::unexpected{std::monostate{}};

		auto result{static_cast<T>(0)};
        for (unsigned short int i{0}; i < N; ++i)
            result |= (1 << i);
        return result;
    }

	template <std::integral T, std::integral U, unsigned short int _M>
	__DISXX_PRIVATE__ std::expected<T, std::monostate> ZeroExtend(U x, unsigned short int N) noexcept
	{
		if (N < _M) [[unlikely]]
			return std::unexpected{std::monostate{}};

		const auto ones{Ones<T>(N)};
		if (!ones) [[unlikely]]
			return std::unexpected{std::monostate{}};

		return static_cast<T>(x & *ones);
	}

	template <std::integral T, unsigned short int _N> requires (sizeof(T) * 8 == _N)
	__DISXX_PRIVATE__ bool IsOnes(T x) { return x == Ones<T>(_N); }

	template<std::integral T, unsigned short int _M> requires (_M <= sizeof(T) * 8)
	__DISXX_PRIVATE__ std::expected<std::pair<T, T>, std::monostate> DecodeBitMasks
    (
        bool immN, unsigned short int imms,
		unsigned short int immr, bool immediate
    ) noexcept
	{
		auto tmask{static_cast<T>(0)}, wmask{static_cast<T>(0)};

		if ((((immN << 6) | (~imms)) & (~1)) == 0b0000000) [[unlikely]]
			return std::unexpected{std::monostate{}};

		const auto len{HighestSetBitNZ<unsigned short int, 7>((immN << 6) | (~imms))};
		if (!len) [[unlikely]]
			return std::unexpected{std::monostate{}};

		if (!(2 <= std::pow(2, *len) && std::pow(2, *len) <= _M)) [[unlikely]]
			return std::unexpected{std::monostate{}};
		
		const auto ones{Ones<unsigned short int>(*len)};
		if (!ones) [[unlikely]]
			return std::unexpected{std::monostate{}};

		const auto levels{ZeroExtend<unsigned short int, unsigned short int, 6>(*ones, 6)};
		if (!levels) [[unlikely]]
			return std::unexpected{std::monostate{}};

		if (immediate && (imms & *levels) == *levels) [[unlikely]]
			return std::unexpected{std::monostate{}};
		
		auto s{imms & *levels}, r{immr & *levels};
		auto diff{s - r}, esize{1 << *len};

		const auto mask{Ones<unsigned short int>(*len - 1)};
		if (!mask) [[unlikely]]	
			return std::unexpected{std::monostate{}};

		auto d{diff & *mask};
		
		const auto sOnes{Ones<unsigned short int>(s + 1)}, dOnes{Ones<unsigned short int>(d + 1)};
		if (!sOnes || !dOnes) [[unlikely]]
			return std::unexpected{std::monostate{}};

		const auto welem{ZeroExtend<T, unsigned short int, _M>(*sOnes, esize)}, telem{ZeroExtend<T, unsigned short int, _M>(*dOnes, esize)};
		if (!welem || !telem) [[unlikely]]
			return std::unexpected{std::monostate{}};

		wmask = Replicate<T, T, _M>(std::rotr(*welem, r), _M / esize);
		tmask = Replicate<T, T, _M>(*telem, _M / esize);

		return std::make_pair(wmask, tmask);
	}

	/*
	 * Non-template functions
	 */

	__DISXX_PRIVATE__ std::expected<std::uint64_t, std::overflow_error> AdvSIMDExpandImm(unsigned short int, unsigned short int, unsigned short int) noexcept;

	__DISXX_PRIVATE__ unsigned short int SysOp(unsigned short int, unsigned short int, unsigned short int, unsigned short int) noexcept;

	__DISXX_PRIVATE__ bool SysOp128(unsigned short int, unsigned short int, unsigned short int, unsigned short int) noexcept;

	__DISXX_PRIVATE__ bool MoveWidePreferred(bool, bool, unsigned short int, unsigned short int) noexcept;

	__DISXX_PRIVATE__ bool BFXPreffered(bool, bool, unsigned short int, unsigned short int) noexcept;
} /* bits */
