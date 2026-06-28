module;

#include <disconf.hpp>

#include <unordered_map>
#include <string_view>
#include <sstream>
#include <regex>

export module disxx.disasm.Disassembler;

export import <expected>;
export import <concepts>;
export import <ranges>;
export import <format>;
export import <vector>;

export import disxx.utility.error.DisassemblyError;
export import disxx.disasm.Instruction;
export import disxx.disasm.Address;
export import disxx.disasm.Bytes;

template <typename T>
concept BytesRange = std::ranges::range<T> && std::same_as
<
	typename T::value_type,
	typename disxx::disasm::Bytes
>;

export namespace disxx::disasm
{
	class [[nodiscard]] __DISXX_EXPORT__ Disassembler
	{
	  public:
		using SingleResult = std::expected
		<
			Instruction,
			disxx::utility::error::DisassemblyError
		>;
		using DisassemblyResult = std::expected
		<
			std::vector<Instruction>,
			disxx::utility::error::DisassemblyError
		>;

	  public:
		explicit Disassembler(void) noexcept = default;

		explicit Disassembler(const Disassembler &) noexcept = default;
		Disassembler &operator=(const Disassembler &) noexcept = default;

		~Disassembler(void) noexcept = default;

		SingleResult DisassembleSingle(const Bytes, const Address) const noexcept(false);
		template <BytesRange T>
		DisassemblyResult DisassembleAll(std::ranges::ref_view<T>, Address) const noexcept(false);
	};

	template <BytesRange T>
	Disassembler::DisassemblyResult Disassembler::DisassembleAll(std::ranges::ref_view<T> ref, Address addr) const noexcept(false)
	{
		std::vector<Instruction> vec{};
		for (const auto &insn : ref)
			if (auto subresult{this->DisassembleSingle(insn, addr++)})
				vec.emplace_back(std::move(subresult.value()));
			else
				return std::unexpected{disxx::utility::error::DisassemblyError{static_cast<std::uint32_t>(insn)}};
		return vec;
	}
} /* disxx::disasm */
