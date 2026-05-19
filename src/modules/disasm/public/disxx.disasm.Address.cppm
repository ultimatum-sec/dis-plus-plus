module;

#include <disconf.hpp>

export module disxx.disasm.Address;

export import <cstdint>;
export import <format>;
export import <string>;

export namespace disxx::disasm
{
	enum class __DISXX_EXPORT__ Address : std::uint64_t {};

	[[nodiscard]] constexpr inline std::uint64_t integer(Address addr) noexcept
	{ return static_cast<std::uint64_t>(addr); }

	__DISXX_EXPORT__ Address operator++(Address &, int) noexcept;
} /* disxx::disasm */

export template <> struct std::formatter<disxx::disasm::Address> : public std::formatter<std::string>
{
  public:
	auto format(const disxx::disasm::Address &addr, std::format_context &ctx) const
	{
		return std::format_to
		(
			ctx.out(),
			"{}",
			integer(addr)
		);
	}
};
